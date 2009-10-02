#ifndef _HFBS2JPSIPHI_h_
#define _HFBS2JPSIPHI_h_

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicVertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"


// ----------------------------------------------------------------------

class HFBs2JpsiPhi : public edm::EDAnalyzer {
 public:
  explicit HFBs2JpsiPhi(const edm::ParameterSet&);
  ~HFBs2JpsiPhi();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;


  RefCountedKinematicTree doVertexFit(std::vector<reco::Track> &Tracks);

  int           fVerbose; 
  std::string   fTracksLabel, fPrimaryVertexLabel;
  edm::InputTag fMuonsLabel;

  double        fMuonPt, fTrackPt, fDeltaR;

  reco::Vertex  fPV;

  edm::ESHandle<TransientTrackBuilder> fTTB;
};

#endif
