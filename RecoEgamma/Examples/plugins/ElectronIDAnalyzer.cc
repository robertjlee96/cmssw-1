#include "RecoEgamma/Examples/plugins/ElectronIDAnalyzer.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

ElectronIDAnalyzer::ElectronIDAnalyzer(const edm::ParameterSet& conf) : conf_(conf) {
  electronProducer_=conf.getParameter<std::string>("electronProducer") ;
  electronLabelRobustLoose_=conf.getParameter<std::string>("electronLabelRobustLoose") ;
  electronLabelRobustTight_=conf.getParameter<std::string>("electronLabelRobustTight") ; 
  electronLabelLoose_=conf.getParameter<std::string>("electronLabelLoose") ;
  electronLabelTight_=conf.getParameter<std::string>("electronLabelTight") ;
}  

void ElectronIDAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& c) {

  //Read selectrons
  edm::Handle<reco::GsfElectronCollection> electrons ;
  e.getByLabel(electronProducer_,electrons) ;

  //Read eID results
  std::vector<edm::Handle<edm::ValueMap<float> > > eIDValueMap(4); 
  //Robust-Loose 
  e.getByLabel( electronLabelRobustLoose_ , eIDValueMap[0] ); 
  const edm::ValueMap<float> & eIDmapRL = * eIDValueMap[0] ;
  //Robust-Tight 
  e.getByLabel( electronLabelRobustTight_ , eIDValueMap[1] ); 
  const edm::ValueMap<float> & eIDmapRT = * eIDValueMap[1] ;
  //Loose 
  e.getByLabel( electronLabelLoose_ , eIDValueMap[2] ); 
  const edm::ValueMap<float> & eIDmapL = * eIDValueMap[2] ;
  //Tight 
  e.getByLabel( electronLabelTight_ , eIDValueMap[3] ); 
  const edm::ValueMap<float> & eIDmapT = * eIDValueMap[3] ;

  // Loop over electrons
  for (unsigned int i = 0; i < electrons->size(); i++){
    edm::Ref<reco::GsfElectronCollection> electronRef(electrons,i);
    std::cout << "Event " << e.id() 
    	      << " , electron " << i+1 
	      << " , Robust Loose = " << eIDmapRL[electronRef] 
	      << " , Robust Tight = " << eIDmapRT[electronRef] 
	      << " , Loose = " << eIDmapL[electronRef] 
	      << " , Tight = " << eIDmapT[electronRef] 
	      << std::endl;
  }

}
