// Class:      L25TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25TauEfficiencyAnalyzer.h,v 1.4 2008/10/15 11:58:39 smaruyam Exp $
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TFile.h"
#include "TTree.h"

class L25TauEfficiencyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit L25TauEfficiencyAnalyzer(const edm::ParameterSet&);
      ~L25TauEfficiencyAnalyzer();
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void Setup(const edm::ParameterSet&,TTree*);
      void fill(const edm::Event&,const reco::PFTau&);   
      edm::InputTag   tauSource;
      edm::InputTag l25JetSource;
      edm::InputTag l25PtCutSource;
      edm::InputTag l25IsoSource;
      edm::InputTag outputFileName;
      std::string rootFile_;
      TFile* l25file;
      TTree* l25tree;
      float tauPt;
      float tauInvPt;
      float tauInvPt1;
      float tauInvPt3;
      float tauInvPtm;
      float tauInvPtm1;
      float tauInvPtm3;
      float tauEt;
      float tauEta;
      float tauPhi;
      float tauTjDR;
      float tauTrkC05;
      float tauTrkSig;
      float l25Et;
      float l25Phi;
      float l25Eta;
      float l25Pt;
      float l25PtCut;
      float l25InvPt;
      float l25InvPt1;
      float l25InvPt3;
      float l25Iso;
      float l25TjDR;
      float l25TrkQPx;
      int   l25Depth;
      float minDR;
      float bareEt;
      double matchingCone;
};
