#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/RooStatsCms/interface/ClopperPearsonBinomialInterval.h"
#include "TGraphAsymmErrors.h" 
#include "TH1.h"
#include <numeric>
#include <algorithm>
#include <string>
using namespace std;
using namespace reco;
using namespace edm;

/*
bool IsMuMatchedToHLTMu ( const reco::Candidate * dau, std::vector<reco::Particle> HLTMu , double DR, double DPtRel ) {
  unsigned int dim =  HLTMu.size();
  unsigned int nPass=0;
  if (dim==0) return false;
  for (unsigned int k =0; k< dim; k++ ) {
   if (  (deltaR(HLTMu[k], *dau) < DR)   && (fabs(HLTMu[k].pt() - dau->pt())/ HLTMu[k].pt()<DPtRel)){     nPass++ ;
    }
  }
  return (nPass>0);
}

bool IsMuMatchedToHLTSingleMu ( const reco::Candidate * dau, reco::Particle HLTMu , double DR, double DPtRel ) {
  unsigned int nPass=0;
  if (  (deltaR(HLTMu, *dau) < DR)   && (fabs(HLTMu.pt() - dau->pt())/ HLTMu.pt()<DPtRel)) {
    nPass++;
  }
  return (nPass>0);
}

*/


class MuTriggerAnalyzer : public edm::EDAnalyzer {
public:
  MuTriggerAnalyzer(const edm::ParameterSet& pset );

private:
  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup);
  virtual void endJob();
  bool IsMuMatchedToHLTMu ( const reco::Muon & , std::vector<reco::Particle> ,double ,double );
  
  edm::InputTag trigTag_;   
  edm::InputTag  trigEv_;
  edm::InputTag muonTag_;
  double ptMuCut_, etaMuCut_;
  std::string hltPath_;
  std::string L3FilterName_;
  edm::Handle<edm::TriggerResults> triggerResults_;
  edm::TriggerNames const* trigNames_;
  edm::Handle< trigger::TriggerEvent > handleTriggerEvent_;

  double maxDPtRel_, maxDeltaR_ ;
  const int nbins_;
  const double ptMax_;
  //std::vector<int> num_, den_;
  //TH1D * hL2MuonPtS_;
  TH1D * hTrigMuonPtNumS_;
  TH1D * hTrigMuonPtDenS_;
  TH1D * deltaR_;
  TH1D * deltaPtOverPt_;

  //  TGraphAsymmErrors * graph_;
}; 

bool MuTriggerAnalyzer::IsMuMatchedToHLTMu ( const reco::Muon & mu, std::vector<reco::Particle> HLTMu , double DR, double DPtRel ) {
  size_t dim =  HLTMu.size();
  size_t nPass=0;
  
  // filling the denumerator;
  double muRecoPt= mu.pt();
  hTrigMuonPtDenS_-> Fill(muRecoPt);         
 
  if (dim==0) return false;
  for (size_t k =0; k< dim; k++ ) {
    if (  (deltaR(HLTMu[k], mu) < DR)   && (fabs(HLTMu[k].pt() - mu.pt())/ HLTMu[k].pt()<DPtRel)){     
      nPass++ ;
      std::cout<< " matching a muon " << std::endl;  
      std::cout  << "muon reco  pt : " << muRecoPt<< std::endl;     
      std::cout  << "muon reco  eta " <<  mu.eta() << std::endl;   
      std::cout << "muon trigger  pt "<< HLTMu[k].pt() << std::endl; 
      // filling the numerator, at the same bin as the denum..... 
      hTrigMuonPtNumS_-> Fill(muRecoPt);
      deltaR_-> Fill(deltaR(HLTMu[k], mu));
      deltaPtOverPt_-> Fill(fabs(HLTMu[k].pt() - mu.pt())/ HLTMu[k].pt());

         
      std::cout << "muon trigger  eta : "<< HLTMu[k].eta() << std::endl;   
      std::cout  <<"deltaR((HLTMu[k], mu)): "<< deltaR(HLTMu[k], mu) << std::endl;
      std::cout  <<"deltaPtOverPt: "<< fabs(HLTMu[k].pt() - mu.pt())/ HLTMu[k].pt() << std::endl;
    }
  }
  
  return (nPass>0);
}



MuTriggerAnalyzer::MuTriggerAnalyzer(const edm::ParameterSet& cfg ) :
  trigTag_(cfg.getParameter<edm::InputTag> ("TrigTag")),
  trigEv_(cfg.getParameter<edm::InputTag> ("triggerEvent")),
  muonTag_(cfg.getUntrackedParameter<edm::InputTag>("muons")),
  ptMuCut_(cfg.getUntrackedParameter<double>("ptMuCut")),
  etaMuCut_(cfg.getUntrackedParameter<double>("etaMuCut")),
  hltPath_(cfg.getParameter<std::string >("hltPath")),
  L3FilterName_(cfg.getParameter<std::string >("L3FilterName")),
  maxDPtRel_(cfg.getParameter<double>("maxDPtRel")),
  maxDeltaR_(cfg.getParameter<double>("maxDeltaR")),
  nbins_(cfg.getParameter<double>("ptMax_")), 
  ptMax_(cfg.getParameter<double>("ptMax_")){
   Service<TFileService> fs;
  //hTrigMuonPtS_ = fs->make<TH1D>("hTrigMuonPtS", "hTrigMuonPtS", nbins_ + 1, 0, ptMax_); 
  hTrigMuonPtNumS_ = fs->make<TH1D>("hTrigMuonPtNumS", "hTrigMuonPtNumS", nbins_ + 1, 0, ptMax_); 
  hTrigMuonPtDenS_ = fs->make<TH1D>("hTrigMuonPtDenS", "hTrigMuonPtDenS", nbins_ +1 , 0, ptMax_); 
  deltaR_ = fs->make<TH1D>("deltaR", "deltaR", nbins_+1, 0, maxDeltaR_); 
  deltaPtOverPt_ = fs->make<TH1D>("deltaPtOverPt", "deltaPtOverPt", nbins_ + 1, 0, maxDPtRel_); 
  // graph_ = fs->make<TGraphAsymmErrors>(nbins_);
  //graph_->SetName("TriggerEfficiency");
  //graph_->SetTitle("Trigger efficiency");
}

void MuTriggerAnalyzer::endJob() {
  //ClopperPearsonBinomialInterval cp;
  //const double alpha = (1-0.682);
  //cp.init(alpha);
  //double * x = new double[nbins_];
  //  double * eff = new double[nbins_];
  //double * exl = new double[nbins_];
  //double * exh = new double[nbins_];
  //double * eeffl = new double[nbins_];
  //double * eeffh = new double[nbins_];
  //  double eff=0;
  for (int i = 0; i < nbins_+1; ++i){
    //x[i] = (double(i) + 0.5) * ptMax_ / nbins_; 
    //exl[i] = exh[i] = 0;
   
    //  if (den_[i] !=0 ){  
    // eff = double (num_[i]) / double(den_[i ]);
    //  hTrigMuonPtS_->SetBinContent(i, eff);
    // hTrigMuonPtS_->SetBinError(i, 0);
      //cp.calculate(num_[i], den_[i]);
      //eeffl[i] = eff[i] - cp.lower();
      //eeffh[i] = cp.upper() - eff[i];
    // } else {
     
    //   hTrigMuonPtS_->SetBinContent(i, 0);
    // hTrigMuonPtS_->SetBinError(i, 0);
      //eeffl[i] = 0;
      //eeffh[i] = 1;
    //}
    ///graph_->SetPoint(i, x[i], eff[i]);
    ///graph_->SetPointError(i, 0, 0, eeffl[i], eeffh[i]);
    std::cout << "number of reco muon in bin " << i << " = " << hTrigMuonPtDenS_->GetBinContent(i) << std::endl;
    
    std::cout << "number of hlt muon in bin " << i << " = " << hTrigMuonPtNumS_->GetBinContent(i) << std::endl;

 
  }
  //std::cout << "booking vectors" << std::endl;
  //std::cout << "making plot" << std::endl;
  //graph_->SetMarkerColor(kBlue);
  //graph_->SetMarkerStyle(21);
  //graph_->SetLineWidth(1);
  //graph_->SetLineColor(kBlue);
  //std::cout << "deleting vectors" << std::endl;
  //delete [] x; delete [] eff; delete [] exl; delete [] exh; delete [] eeffl; delete [] eeffh; 
}

void MuTriggerAnalyzer::analyze (const Event & ev, const EventSetup &) {
      bool singleTrigFlag1 = false;
          //      bool overlap = false;
  
      // Trigger
      Handle<TriggerResults> triggerResults;
      TriggerNames trigNames;
      if (!ev.getByLabel(trigTag_, triggerResults)) {
	LogWarning("") << ">>> TRIGGER collection does not exist !!!";
	return;
      }
      ev.getByLabel(trigTag_, triggerResults); 
      trigNames.init(*triggerResults);
      bool trigger_fired = false;
      for (unsigned int i=0; i<triggerResults->size(); i++) {
        std::string trigName = trigNames.triggerName(i);
	if ( trigName == hltPath_ && triggerResults->accept(i)) {
	  trigger_fired = true;
	}
      }   
      edm::Handle< trigger::TriggerEvent > handleTriggerEvent;
      LogTrace("") << ">>> Trigger bit: " << trigger_fired << " (" << hltPath_ << ")";
      if ( ! ev.getByLabel( trigEv_, handleTriggerEvent ))  {
	LogWarning( "errorTriggerEventValid" ) << "trigger::TriggerEvent product with InputTag " << trigEv_.encode() << " not in event";
	return;
      }
      ev.getByLabel( trigEv_, handleTriggerEvent );
      const trigger::TriggerObjectCollection & toc(handleTriggerEvent->getObjects());
      size_t nMuHLT =0;
      std::vector<reco::Particle>  HLTMuMatched; 
      for ( size_t ia = 0; ia < handleTriggerEvent->sizeFilters(); ++ ia) {
	std::string fullname = handleTriggerEvent->filterTag(ia).encode();
	std::string name;
	size_t p = fullname.find_first_of(':');
	if ( p != std::string::npos) {
	  name = fullname.substr(0, p);
	}
	else {
	  name = fullname;
	}
	if ( &toc !=0 ) {
	  const trigger::Keys & k = handleTriggerEvent->filterKeys(ia);
	  for (trigger::Keys::const_iterator ki = k.begin(); ki !=k.end(); ++ki ) {
	    if (name == L3FilterName_  ) { 
	      HLTMuMatched.push_back(toc[*ki].particle());
	      nMuHLT++;     
	    }
	  }    
	}
      }
      
	//  looping on muon....
      Handle<View<Muon> >   muons;     
      if (!ev.getByLabel(muonTag_, muons)) {           
	LogError("") << ">>> muon collection does not exist !!!";     
	return;     
      }
      
      ev.getByLabel(muonTag_, muons);  
      //saving only muons with pt> ptMuCut and eta<etaMuCut  
      std::vector<reco::Muon>  highPtGlbMuons; 
    
      for (unsigned int i=0; i<muons->size(); i++ ){	
        const reco::Muon & mu = muons->at(i);
	double pt = mu.pt();
	double eta = mu.eta();
	if (pt> ptMuCut_ && fabs(eta)< etaMuCut_) {
	  if (mu.isGlobalMuon()) highPtGlbMuons.push_back(mu);    
	}
      }
      unsigned int nHighPtGlbMu = highPtGlbMuons.size();
      std::cout << "I've got " << nHighPtGlbMu << " nHighPtGlbMu" << std::endl;
      // unsigned int nHighPtStaMu = highPtStaMuons.size();

	// loop on high pt muons if there's at least two with opposite charge build a Z, more then one z candidate is foreseen.........
	// stop the loop after 10 cicles....  
       	(nHighPtGlbMu> 10)?   nHighPtGlbMu=10 : 1; 

	if (nHighPtGlbMu>0 ){
	  
           for(unsigned int i =0 ; i < nHighPtGlbMu ; i++) {
	    reco::Muon muon1 = highPtGlbMuons[i];
	    math::XYZTLorentzVector mu1(muon1.p4());
	    //      double pt1= muon1.pt();
        
	    //iso2 = muIso ( muon2 );
	    singleTrigFlag1 = IsMuMatchedToHLTMu ( muon1,  HLTMuMatched ,maxDeltaR_, maxDPtRel_ );
	    
	  }
	  
	}      
      
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( MuTriggerAnalyzer );












