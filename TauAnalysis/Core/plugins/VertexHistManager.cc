#include "TauAnalysis/Core/plugins/VertexHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

VertexHistManager::VertexHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<VertexHistManager::VertexHistManager>:" << std::endl;

  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;

  makeVertexXvsYhistogram_ = ( cfg.exists("makeVertexXvsYhistogram") ) ? 
    cfg.getParameter<bool>("makeVertexXvsYhistogram") : false;
}

VertexHistManager::~VertexHistManager()
{
//--- nothing to be done yet...
}

void VertexHistManager::bookHistogramsImp()
{
  //std::cout << "<VertexHistManager::bookHistogramsImp>:" << std::endl;
  
  hVertexX_ = book1D("VertexX", "VertexX", 200, -1., +1.);
  hVertexSigmaX_ = book1D("VertexSigmaX", "VertexSigmaX", 200, -0.01, +0.01);
  hVertexY_ = book1D("VertexY", "VertexY", 200, -1., +1.);
  hVertexSigmaY_ = book1D("VertexSigmaY", "VertexSigmaY", 200, -0.01, +0.01);
  if ( makeVertexXvsYhistogram_ ) hVertexXvsY_ = book2D("VertexXvsY", "VertexXvsY", 200, -1., +1., 200, -1, +1.);
  hVertexZ_ = book1D("VertexZ", "VertexZ", 100, -50., 50.);
  hVertexSigmaZ_ = book1D("VertexSigmaZ", "VertexSigmaZ", 200, -0.10, 0.10);

  hVertexNumTracks_ = book1D("VertexNumTracks", "VertexNumTracks", 101, -0.5, 100.5);
  
  hVertexChi2Prob_ = book1D("VertexChi2Prob", "VertexChi2Prob", 102, -0.01, 1.01);
}

void VertexHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<VertexHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<std::vector<reco::Vertex> > recoVertices;
  evt.getByLabel(vertexSrc_, recoVertices);
  if ( recoVertices->size() >= 1 ) {
    const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());

    hVertexX_->Fill(thePrimaryEventVertex.x(), evtWeight);
    hVertexSigmaX_->Fill(thePrimaryEventVertex.xError(), evtWeight);
    hVertexY_->Fill(thePrimaryEventVertex.y(), evtWeight);
    hVertexSigmaY_->Fill(thePrimaryEventVertex.yError(), evtWeight);
    if ( makeVertexXvsYhistogram_ ) hVertexXvsY_->Fill(thePrimaryEventVertex.x(), thePrimaryEventVertex.y(), evtWeight);
    hVertexZ_->Fill(thePrimaryEventVertex.z(), evtWeight);
    hVertexSigmaZ_->Fill(thePrimaryEventVertex.zError(), evtWeight);

    hVertexNumTracks_->Fill(thePrimaryEventVertex.tracksSize(), evtWeight);

    hVertexChi2Prob_->Fill(TMath::Prob(thePrimaryEventVertex.chi2(), TMath::Nint(thePrimaryEventVertex.ndof())), evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, VertexHistManager, "VertexHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, VertexHistManager, "VertexHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<VertexHistManager> VertexAnalyzer;

DEFINE_FWK_MODULE(VertexAnalyzer);
