#include "RecoVertex/GaussianSumVertexFit/test/GsfTest.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/GaussianSumVertexFit/interface/GsfVertexFitter.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

#include <iostream>

using namespace reco;
using namespace edm;
using namespace std;

GsfTest::GsfTest(const edm::ParameterSet& iConfig)
  : theConfig(iConfig)
{
  trackLabel_ = iConfig.getParameter<std::string>("TrackLabel");
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile");
  gsfPSet = iConfig.getParameter<edm::ParameterSet>("GsfParameters");
  rootFile_ = TFile::Open(outputFile_.c_str(),"RECREATE"); 
  edm::LogInfo("RecoVertex/GsfTest") 
    << "Initializing KVF TEST analyser  - Output file: " << outputFile_ <<"\n";
}


GsfTest::~GsfTest() {
  delete rootFile_;
}

void GsfTest::beginJob(edm::EventSetup const& setup){
  edm::ESHandle<TrackAssociatorBase> theAssociatorForParamAtPca;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2",theAssociatorForParamAtPca);
  associatorForParamAtPca = (TrackAssociatorByChi2 *) theAssociatorForParamAtPca.product();

  tree = new SimpleVertexTree("VertexFitter", associatorForParamAtPca);
}


void GsfTest::endJob() {
  delete tree;
}

//
// member functions
//

void
GsfTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{



  try {
    edm::LogInfo("RecoVertex/GsfTest") 
      << "Reconstructing event number: " << iEvent.id() << "\n";
    
    // get RECO tracks from the event
    // `tks` can be used as a ptr to a reco::TrackCollection
    edm::Handle<reco::GsfTrackCollection> tks;
    iEvent.getByLabel(trackLabel_, tks);

    edm::LogInfo("RecoVertex/GsfTest") 
      << "Found: " << (*tks).size() << " reconstructed tracks" << "\n";
    cout << "got " << (*tks).size() << " tracks " << endl;

    // Transform Track to TransientTrack

    //get the builder:
    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
    //do the conversion:
    vector<TransientTrack> t_tks = (*theB).build(tks);

    edm::LogInfo("RecoVertex/GsfTest") 
      << "Found: " << t_tks.size() << " reconstructed tracks" << "\n";
    
    // Call the KalmanVertexFitter if more than 1 track
    if (t_tks.size() > 1) {
      //      KalmanVertexFitter kvf(kvfPSet);
      // For the analysis: compare to your SimVertex
      TrackingVertex sv = getSimVertex(iEvent);
      std::cout << "SimV Position: " << Vertex::Point(sv.position()) << "\n";

      KalmanVertexFitter kvf;
      TransientVertex tv2 = kvf.vertex(t_tks);
      std::cout << "KVF Position:  " << Vertex::Point(tv2.position()) <<tv2.normalisedChiSquared ()<<" "<<tv2.degreesOfFreedom() << "\n";

      GsfVertexFitter gsf(gsfPSet);
      TransientVertex tv = gsf.vertex(t_tks);
      std::cout << "Position: " << Vertex::Point(tv.position()) << "\n";



  edm::Handle<TrackingParticleCollection>  TPCollectionH ;
  iEvent.getByLabel("trackingtruth","TrackTruth",TPCollectionH);
  const TrackingParticleCollection tPC = *(TPCollectionH.product());
//       reco::RecoToSimCollection recSimColl=associatorForParamAtPca->associateRecoToSim(tks,
// 									      TPCollectionH,
// 									      &iEvent);

      tree->fill(tv, &sv, 0, 0.0);
    }
    
  }

  catch (std::exception & err) {
   edm::LogInfo("RecoVertex/GsfTest") 
     cout << "Exception during event number: " << iEvent.id() 
      << "\n" << err.what() << "\n";
  }

}


//Returns the first vertex in the list.

TrackingVertex GsfTest::getSimVertex(const edm::Event& iEvent) const
{
   // get the simulated vertices
  edm::Handle<TrackingVertexCollection>  TVCollectionH ;
  iEvent.getByLabel("trackingtruth","VertexTruth",TVCollectionH);
  const TrackingVertexCollection tPC = *(TVCollectionH.product());

//    Handle<edm::SimVertexContainer> simVtcs;
//    iEvent.getByLabel("g4SimHits", simVtcs);
//    std::cout << "SimVertex " << simVtcs->size() << std::endl;
//    for(edm::SimVertexContainer::const_iterator v=simVtcs->begin();
//        v!=simVtcs->end(); ++v){
//      std::cout << "simvtx "
// 	       << v->position().x() << " "
// 	       << v->position().y() << " "
// 	       << v->position().z() << " "
// 	       << v->parentIndex() << " "
// 	       << v->noParent() << " "
//               << std::endl;
//    }
   return *(tPC.begin());
}
