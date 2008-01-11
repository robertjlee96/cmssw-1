// -*- C++ -*-
//
// Package:    EvtGenInterface
// Class:      EvtGenProducer
// 
/**\class EvtGenProducer GeneratorInterface/EvtGenInterface/src/EvtGenProducer.cc


 Description: EvtGen interface - decays B mesons (left stable by Pythia) by EvtGen

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Nello Nappi
//         Created:  Fri May 11 15:19:32 CEST 2007
// $Id: EvtGenProducer.cc,v 1.3 2007/09/28 15:13:44 nappic Exp $
//
//
#include "FWCore/PluginManager/interface/PluginManager.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

// #include "HepMC/IO_HEPEVT.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "Utilities/General/interface/FileInPath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/Random.h"

#include "GeneratorInterface/EvtGenInterface/interface/EvtGenProducer.h"
#include "GeneratorInterface/EvtGenInterface/interface/myEvtRandomEngine.h"

#include <iostream>
#include "HepMC/PythiaWrapper6_2.h"

#define PYGIVE pygive_
extern "C" {
  void PYGIVE(const char*,int length);
}

EvtGenProducer::EvtGenProducer(edm::ParameterSet const & p)
{   

  // create random engine and initialize seed using Random Number 
  // Generator Service 
  // as configured in the configuration file

  edm::Service<edm::RandomNumberGenerator> rngen;
  
  if ( ! rngen.isAvailable()) {             
    throw cms::Exception("Configuration")
      << "The EvtGenProducer module requires the RandomNumberGeneratorService\n"
      "which is not present in the configuration file.  You must add the service\n"
      "in the configuration file if you  want to run EvtGenProducer";
    }

  CLHEP::HepRandomEngine& m_engine = rngen->getEngine();
  m_flat = new CLHEP::RandFlat(m_engine, 0., 1.);
  myEvtRandomEngine* the_engine = new myEvtRandomEngine(&m_engine); 

  // Get data from parameter set
  std::string decay_table = p.getParameter<std::string>("decay_table");
  std::string pdt = p.getParameter<std::string>("particle_property_file");
  std::string user_decay = p.getUntrackedParameter<std::string>("user_decay_file","none");
  pythia_params = p.getParameter< std::vector<std::string> >("processParameters");
  // any number of alias names for forced decays can be specified using dynamic std vector of strings 
  std::vector<std::string> forced_names = p.getParameter< std::vector<std::string> >("list_forced_decays");
    
  produces<edm::HepMCProduct>();   // declare 
    
  m_EvtGen = new EvtGen (decay_table.c_str(),pdt.c_str(),the_engine);  
  // 4th parameter should be rad cor - set to PHOTOS (default)
 
  if (user_decay != "none") m_EvtGen->readUDecay( user_decay.c_str() );

  std::vector<std::string>::const_iterator i;
  nforced=0;

  for (i=forced_names.begin(); i!=forced_names.end(); ++i)   
    // i will point to strings containing
    // names of particles with forced decay
    {
      nforced++;
      EvtId found = EvtPDL::getId(*i);      
      if (found.getId()==-1)
	{
	  throw cms::Exception("Configuration")
	    << "name in part list for forced decays not found: " << *i; 
	}
      if (found.getId()==found.getAlias())
	{
	  throw cms::Exception("Configuration")
	    << "name in part list for forced decays is not an alias: " << *i; 
	}
      forced_Evt.push_back(found);                      // forced_Evt is the list of EvtId's
      forced_Hep.push_back(EvtPDL::getStdHep(found));   // forced_Hep is the list of stdhep codes
    }

}

EvtGenProducer::~EvtGenProducer() 
{  
}

void EvtGenProducer::beginJob(const edm::EventSetup & es)
{
  ntotal = 0;
  nevent = 0;
  std::cout << " EvtGenProducer starting ... " << std::endl;
}
 
void EvtGenProducer::endJob()
{ 
  std::cout << " EvtGenProducer terminating ... " << std::endl; 
}
 
void EvtGenProducer::produce(edm::Event & e, const edm::EventSetup & es)
{
  nevent++;
  // std::cout << "nevent = " << nevent << std::endl ;
  
  int idHep,ipart,status;
  EvtId idEvt;
  std::auto_ptr<edm::HepMCProduct> new_product( new edm::HepMCProduct() ); 

  edm::Handle< edm::HepMCProduct > EvtHandle ;
  e.getByLabel( "source", EvtHandle ) ;
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  HepMC::GenEvent* newEvt = new HepMC::GenEvent( *Evt );

  // Do here initialization of EvtPythia then restore original settings
  if (nevent == 1) {
    EvtPythia::pythiaInit(0);
    for( std::vector<std::string>::const_iterator itPar = pythia_params.begin(); itPar != pythia_params.end(); ++itPar ) {
      call_pygive(*itPar);
    }
  } 

  // First pass through undecayed Pythia particles to decay particles known to EvtGen left stable by Pythia
  // except candidates to be forced which will be searched later to include EvtGen decay products 
  nlist = 0;

  // Notice dynamical use of newEvt
  for (HepMC::GenEvent::particle_const_iterator p= newEvt->particles_begin(); p != newEvt->particles_end(); ++p)
    {
      status = (*p)->status();
 
      if(status==1) {           // only not decayed (status = 1) particles
          

	  idHep = (*p)->pdg_id();
	  int do_force=0;
	  for(int i=0;i<nforced;i++)           // First check if part with forced decay
	    {                                  // In that case do not decay immediately 
	      if(idHep == forced_Hep[i])       // (only 1 per event will be forced)	 
		{                              // Fill list
		  update_candlist(i,*p);
		  do_force=1;
		}
	    }
	  if(do_force==0)         // particles with decays not forced are decayed immediately 
	    {
	      idEvt = EvtPDL::evtIdFromStdHep(idHep);
	      ipart = idEvt.getId();
	      if (ipart==-1) continue;                          // particle not known to EvtGen       
	      if (EvtDecayTable::getNMode(ipart)==0) continue;  // particles stable for EvtGen
	      decay(*p,idEvt,newEvt,true);                      // generate decay
	    }
	}
    }

  if(nlist!=0)   
     {
      // decide randomly which one to decay as alias
      int which = (int)(nlist*m_flat->fire()); 
      if (which == nlist) which = nlist-1;
  
	  for(int k=0;k < nlist; k++)
	    {
	      if(k == which) {		
		decay(listp[k],forced_Evt[index[k]],newEvt,false);  // decay as alias
	      }	
	      else
		{
		  int id_non_alias = forced_Evt[index[k]].getId();
		  EvtId non_alias(id_non_alias,id_non_alias); // create new EvtId with id = alias
		  decay(listp[k],non_alias,newEvt,false);     // decay as standard (non alias)
		}
	    }
     }

  new_product->addHepMCData( newEvt );
  e.put( new_product );
  
}

void EvtGenProducer::decay(HepMC::GenParticle* partHep, EvtId idEvt, HepMC::GenEvent* theEvent, bool del_daug )
{
  // Set spin type
  EvtSpinType::spintype stype = EvtPDL::getSpinType(idEvt);
  EvtParticle* partEvt;
    switch (stype){
    case EvtSpinType::SCALAR: 
      partEvt = new EvtScalarParticle();
      break;
    case EvtSpinType::STRING:
      partEvt = new EvtStringParticle();    
      break;
    case EvtSpinType::DIRAC: 
      partEvt = new EvtDiracParticle();
      break;
    case EvtSpinType::VECTOR:
      partEvt = new EvtVectorParticle();
      break;
    case EvtSpinType::RARITASCHWINGER:
      partEvt = new EvtRaritaSchwingerParticle();
      break;
    case EvtSpinType::TENSOR:
      partEvt = new EvtTensorParticle();
      break;
    case EvtSpinType::SPIN5HALF: case EvtSpinType::SPIN3: case EvtSpinType::SPIN7HALF: case EvtSpinType::SPIN4:
      partEvt = new EvtHighSpinParticle();
      break;
    default:
      std::cout << "Unknown spintype in EvtSpinType!" << std::endl;   
      return;
    }

    // Generate decay
    EvtVector4R momEvt;
    HepMC::FourVector momHep = partHep->momentum();
    momEvt.set(momHep.t(),momHep.x(),momHep.y(),momHep.z());
    EvtVector4R posEvt;
    HepMC::GenVertex* initVert = partHep->production_vertex();
    HepMC::FourVector posHep = initVert->position();
    posEvt.set(posHep.t(),posHep.x(),posHep.y(),posHep.z());
    partEvt->init(idEvt,momEvt);
    partEvt->setDiagonalSpinDensity();     
    partEvt->decay();
                       
    // extend the search of candidates to be forced to EvtGen decay products and delete their daughters  ** 
    // otherwise they wouldn't get their chance to take part in the forced decay lottery                 **
    if (del_daug) go_through_daughters(partEvt);    // recursive function go_through_daughters will do   **

    // Change particle in stdHEP format
    static EvtStdHep evtstdhep;
    
    evtstdhep.init();
    partEvt->makeStdHep(evtstdhep);

    if (ntotal < 1000 && ntotal%10 == 0) {     // DEBUG
   
      partEvt->printParticle();                
      partEvt->printTree();
      std::cout << evtstdhep << "\n"  <<
      "--------------------------------------------" << std::endl;
    } 

    ntotal++;
    partEvt->deleteTree();

    // ********* Now add to the HepMC Event **********

    // Then loop on evtstdhep to add vertexes... 
    HepMC::GenVertex* theVerts[100];
    for (int ivert = 0; ivert < 100; ivert++) { 
      theVerts[ivert] = 0;
    }

    for (int ipart = 0; ipart < evtstdhep.getNPart(); ipart++) {
      int theMum = evtstdhep.getFirstMother(ipart);
      if (theMum != -1 && !theVerts[theMum]) {
        EvtVector4R theVpos = evtstdhep.getX4(ipart) + posEvt;
	theVerts[theMum] = 
	  new HepMC::GenVertex(HepMC::FourVector(theVpos.get(1),
						 theVpos.get(2),
						 theVpos.get(3),
						 theVpos.get(0)),0);
      }
    }

    // ...then particles
    partHep->set_status(2);
    theVerts[0]->add_particle_in( partHep );

    for (int ipart2 = 1; ipart2 < evtstdhep.getNPart(); ipart2++) {
      int idHep = evtstdhep.getStdHepID(ipart2);
      HepMC::GenParticle* thePart = 
	new HepMC::GenParticle( HepMC::FourVector(evtstdhep.getP4(ipart2).get(1),
						  evtstdhep.getP4(ipart2).get(2),
						  evtstdhep.getP4(ipart2).get(3),
						  evtstdhep.getP4(ipart2).get(0)),
				idHep,
				evtstdhep.getIStat(ipart2));
      int theMum2 = evtstdhep.getFirstMother(ipart2);
      if (theMum2 != -1 && theVerts[theMum2]) theVerts[theMum2]->add_particle_out( thePart );
      if (theVerts[ipart2]) theVerts[ipart2]->add_particle_in( thePart );
       
    }
    
    for (int ipart3 = 0; ipart3 < evtstdhep.getNPart(); ipart3++) {
      if (theVerts[ipart3]) theEvent->add_vertex( theVerts[ipart3] );
    }
    
}        

void
EvtGenProducer::call_pygive(const std::string& iParm ) {
  
  //call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );  
 
}

void 
EvtGenProducer::go_through_daughters(EvtParticle* part) {

  int NDaug=part->getNDaug();
  if(NDaug)
    {
      EvtParticle* Daughter;
      for (int i=0;i<NDaug;i++)
	{
	  Daughter=part->getDaug(i);
          int idHep = EvtPDL::getStdHep(Daughter->getId());
	  int found=0;
	  for(int k=0;k<nforced;k++)         
	    {
              if(idHep == forced_Hep[k])
		{ 
		  found = 1;
		  Daughter->deleteDaughters();
		}
	    }
	  if (!found) go_through_daughters(Daughter);
	}
    }
}

void 
EvtGenProducer::update_candlist( int theIndex, HepMC::GenParticle *thePart )
{
  if(nlist<10)                 // not nice ... but is 10 a reasonable maximum?
     {
       bool isThere = false;
       if (nlist) {
	 for (int check=0; check < nlist; check++) {
           if (listp[check]->barcode() == thePart->barcode()) isThere = true;
	 }
       }
       if (!isThere) { 
	 listp[nlist] = thePart;
	 index[nlist++] = theIndex;
       }
     }
  else
    {
      throw cms::Exception("runtime")
	<< "more than 10 candidates to be forced ";
      return; 
    }  
  return;
}

