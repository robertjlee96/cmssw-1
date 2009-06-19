#include <ostream>

#include "IOMC/ParticleGuns/interface/FlatRandomOneOverPtGunProducer.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace edm;

FlatRandomOneOverPtGunProducer::FlatRandomOneOverPtGunProducer(const edm::ParameterSet& pset) : 
  BaseFlatGunProducer(pset) {


  edm::ParameterSet defpset ;
  edm::ParameterSet pgun_params = 
    pset.getParameter<ParameterSet>("PGunParameters") ;
  
  fMinOneOverPt = pgun_params.getParameter<double>("MinOneOverPt");
  fMaxOneOverPt = pgun_params.getParameter<double>("MaxOneOverPt");
  
  produces<HepMCProduct>();
  produces<GenEventInfoProduct>();

  edm::LogInfo("ParticleGun") << "FlatRandomOneOverPtGunProducer: initialized with minimum and maximum 1/pt " << fMinOneOverPt << ":" << fMaxOneOverPt;
}

FlatRandomOneOverPtGunProducer::~FlatRandomOneOverPtGunProducer() {
  // no need to cleanup GenEvent memory - done in HepMCProduct
}

void FlatRandomOneOverPtGunProducer::produce(Event &e, const EventSetup& es) {

  if ( fVerbosity > 0 ) {
    LogDebug("ParticleGun") << " FlatRandomOneOverPtGunProducer : Begin New Event Generation"; 
  }
  // event loop (well, another step in it...)
          
  // no need to clean up GenEvent memory - done in HepMCProduct
  // 
   
  // here re-create fEvt (memory)
  //
  fEvt = new HepMC::GenEvent() ;
   
  // now actualy, cook up the event from PDGTable and gun parameters
  //
  // 1st, primary vertex
  //
  HepMC::GenVertex* Vtx = new HepMC::GenVertex(HepMC::FourVector(0.,0.,0.));

  // loop over particles
  //
  int barcode = 1 ;
  for (unsigned int ip=0; ip<fPartIDs.size(); ++ip) {

    double pt     = fRandomGenerator->fire(fMinOneOverPt, fMaxOneOverPt) ;
    double eta    = fRandomGenerator->fire(fMinEta, fMaxEta) ;
    double phi    = fRandomGenerator->fire(fMinPhi, fMaxPhi) ;
    if (pt != 0) pt = 1./pt;
    //    LogDebug("ParticleGun") << "FlatRandomOneOverPtGunProducer: Event generated with pt:eta:phi " << pt << " " << eta << " " << phi;
    int PartID = fPartIDs[ip] ;
    const HepPDT::ParticleData* 
      PData = fPDGTable->particle(HepPDT::ParticleID(abs(PartID))) ;
    double mass   = PData->mass().value() ;
    double theta  = 2.*atan(exp(-eta)) ;
    double mom    = pt/sin(theta) ;
    double px     = pt*cos(phi) ;
    double py     = pt*sin(phi) ;
    double pz     = mom*cos(theta) ;
    double energy2= mom*mom + mass*mass ;
    double energy = sqrt(energy2) ; 
    HepMC::FourVector p(px,py,pz,energy) ;
    HepMC::GenParticle* Part = new HepMC::GenParticle(p,PartID,1);
    Part->suggest_barcode( barcode ) ;
    barcode++ ;
    Vtx->add_particle_out(Part);

    if ( fAddAntiParticle ) {
      HepMC::FourVector ap(-px,-py,-pz,energy) ;
      int APartID = -PartID ;
      if ( PartID == 22 || PartID == 23 ) {
	APartID = PartID ;
      }	  
      HepMC::GenParticle* APart = new HepMC::GenParticle(ap,APartID,1);
      APart->suggest_barcode( barcode ) ;
      barcode++ ;
      Vtx->add_particle_out(APart) ;
    }

  }

  fEvt->add_vertex(Vtx) ;
  fEvt->set_event_number(e.id().event()) ;
  fEvt->set_signal_process_id(20) ; 
        
  if ( fVerbosity > 0 ) {
    fEvt->print() ;  
  }

  std::auto_ptr<HepMCProduct> BProduct(new HepMCProduct()) ;
  BProduct->addHepMCData( fEvt );
  e.put(BProduct);

  std::auto_ptr<GenEventInfoProduct> genEventInfo(new GenEventInfoProduct(fEvt));
  e.put(genEventInfo);
    
  if ( fVerbosity > 0 ) {
    // for testing purpose only
    // fEvt->print() ; // prints empty info after it's made into edm::Event
    LogDebug("ParticleGun") << " FlatRandomOneOverPtGunProducer : Event Generation Done ";
  }
}
//#include "FWCore/Framework/interface/MakerMacros.h"
//DEFINE_FWK_MODULE(FlatRandomOneOverPtGunProducer);
