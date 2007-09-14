#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimMuon/RPCDigitizer/src/RPCDigiProducer.h"
#include "SimMuon/RPCDigitizer/src/RPCDigitizer.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

RPCDigiProducer::RPCDigiProducer(const edm::ParameterSet& ps) {
  theDigitizer = new RPCDigitizer(ps);
  produces<RPCDigiCollection>();
}


RPCDigiProducer::~RPCDigiProducer() {
  delete theDigitizer;
}


void RPCDigiProducer::produce(edm::Event& e, const edm::EventSetup& eventSetup) {

  edm::Handle<CrossingFrame<PSimHit> > cf;
  e.getByLabel("mix", "MuonRPCHits", cf);

  // test access to SimHits
  const std::string hitsName("MuonRPCHits");

  std::auto_ptr<MixCollection<PSimHit> > 
    hits( new MixCollection<PSimHit>(cf.product()) );

  // Create empty output
  std::auto_ptr<RPCDigiCollection> pDigis(new RPCDigiCollection());

  // find the geometry & conditions for this event
  edm::ESHandle<RPCGeometry> hGeom;
  eventSetup.get<MuonGeometryRecord>().get( hGeom );
  const RPCGeometry *pGeom = &*hGeom;
  theDigitizer->setGeometry( pGeom );


  // find the magnetic field
  //edm::ESHandle<MagneticField> magfield;
  ///eventSetup.get<IdealMagneticFieldRecord>().get(magfield);
  //theDigitizer->setMagneticField(&*magfield);

  // run the digitizer

  theDigitizer->doAction(*hits, *pDigis);

  // store them in the event
  e.put(pDigis);
}

