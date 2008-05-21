#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQM/HLTEvF/interface/FourVectorHLT.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

#include "DataFormats/HLTReco/interface/TriggerObject.h"
//#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronIsolationAssociation.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

using namespace edm;

FourVectorHLT::FourVectorHLT(const edm::ParameterSet& iConfig)
{
  
  LogDebug("FourVectorHLT") << "constructor...." ;
  
  
  dbe_ = NULL;
  if (iConfig.getUntrackedParameter < bool > ("DQMStore", false)) {
    dbe_ = Service < DQMStore > ().operator->();
    dbe_->setVerbose(0);
  }
  
  
  dirname_="HLT/FourVectorHLT" + 
    iConfig.getParameter<std::string>("@module_label");
  
  if (dbe_ != NULL) {
    dbe_->setCurrentFolder(dirname_);
  }
  
  
  // plotting paramters
  ptMin_ = iConfig.getUntrackedParameter<double>("ptMin",0.);
  ptMax_ = iConfig.getUntrackedParameter<double>("ptMax",1000.);
  nBins_ = iConfig.getUntrackedParameter<unsigned int>("Nbins",40);

  // this is the list of paths to look at.
  std::vector<edm::ParameterSet> filters = 
    iConfig.getParameter<std::vector<edm::ParameterSet> >("filters");
  for(std::vector<edm::ParameterSet>::iterator 
	filterconf = filters.begin() ; filterconf != filters.end(); 
      filterconf++) {
    std::string me  = filterconf->getParameter<std::string>("name");
    int objectType = filterconf->getParameter<unsigned int>("type");
    float ptMin = filterconf->getUntrackedParameter<double>("ptMin");
    float ptMax = filterconf->getUntrackedParameter<double>("ptMax");
    hltPaths_.push_back(PathInfo(me, objectType, ptMin, ptMax));
  }
  
  triggerSummaryLabel_ = 
    iConfig.getParameter<edm::InputTag>("triggerSummaryLabel");
  
  
}


FourVectorHLT::~FourVectorHLT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
FourVectorHLT::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace trigger;
   ++nev_;
   LogDebug("FourVectorHLT")<< "FourVectorHLT: analyze...." ;
   

  edm::Handle<trigger::TriggerEvent> triggerObj;
  iEvent.getByLabel(triggerSummaryLabel_,triggerObj); 
  if(!triggerObj.isValid()) { 
    edm::LogWarning("FourVectorHLT") << "Summary HLT results not found, "
      "skipping event"; 
    return;
  }


  const trigger::TriggerObjectCollection & toc(triggerObj->getObjects());
  for(PathInfoCollection::iterator v = hltPaths_.begin();
      v!= hltPaths_.end(); ++v ) {
    const int index = triggerObj->filterIndex(v->getName());
    if ( index >= triggerObj->sizeFilters() ) {
      //std::cout << "index is " << index << std::endl;
      continue; // not in this event
    }
    const trigger::Keys & k = triggerObj->filterKeys(index);
    //std::cout << "keys size: " << k.size() << std::endl;
    for (trigger::Keys::const_iterator ki = k.begin(); ki !=k.end(); ++ki ) {
        v->getEtHisto()->Fill(toc[*ki].pt());
        v->getEtaHisto()->Fill(toc[*ki].eta());
        v->getPhiHisto()->Fill(toc[*ki].phi());
        v->getEtaVsPhiHisto()->Fill(toc[*ki].eta(), toc[*ki].phi());
    }  
  }
}


// -- method called once each job just before starting event loop  --------
void 
FourVectorHLT::beginJob(const edm::EventSetup&)
{
  nev_ = 0;
  DQMStore *dbe = 0;
  dbe = Service<DQMStore>().operator->();
  
  if (dbe) {
    dbe->setCurrentFolder(dirname_);
    dbe->rmdir(dirname_);
  }
  
  
  if (dbe) {
    dbe->setCurrentFolder(dirname_);
    
    for(PathInfoCollection::iterator v = hltPaths_.begin();
	v!= hltPaths_.end(); ++v ) {
      MonitorElement *et, *eta, *phi, *etavsphi=0;
      std::string histoname(v->getName()+"_et");
      std::string title(v->getName()+" E_t");
      et =  dbe->book1D(histoname.c_str(),
			title.c_str(),nBins_,
			v->getPtMin(),
			v->getPtMax());
      
      histoname = v->getName()+"_eta";
      title = v->getName()+" #eta";
      eta =  dbe->book1D(histoname.c_str(),
			 title.c_str(),nBins_,-2.7,2.7);

      histoname = v->getName()+"_phi";
      title = v->getName()+" #phi";
      phi =  dbe->book1D(histoname.c_str(),
			 histoname.c_str(),nBins_,-3.14,3.14);
 

      histoname = v->getName()+"_etaphi";
      title = v->getName()+" #eta vs #phi";
      etavsphi =  dbe->book2D(histoname.c_str(),
			      title.c_str(),
			      nBins_,-2.7,2.7,
			      nBins_,-3.14, 3.14);
      
      v->setHistos( et, eta, phi, etavsphi);
    } 
  }
}

// - method called once each job just after ending the event loop  ------------
void 
FourVectorHLT::endJob() 
{
   LogInfo("FourVectorHLT") << "analyzed " << nev_ << " events";
   return;
}

