#include "TauAnalysis/Core/plugins/BoolEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

BoolEventSelector::BoolEventSelector(const edm::ParameterSet& cfg)
{ 
  //std::cout << "<BoolEventSelector::BoolEventSelector>:" << std::endl;

  src_ = cfg.getParameter<edm::InputTag>("src");
  //std::cout << " src = " << src_ << std::endl;

  failSilent_ = cfg.exists("failSilent") ? cfg.getParameter<bool>("failSilent") : false;
}

bool BoolEventSelector::operator()(edm::Event& evt, const edm::EventSetup&)
{
  edm::Handle<bool> booleanFlag;
  evt.getByLabel(src_, booleanFlag);

  if ( booleanFlag.isValid() ) {
    return (*booleanFlag);
  } else {
    if ( !failSilent_ ) 
      edm::LogError ("BoolEventSelector::operator()") 
	<< " Failed to retrieve boolean flag for src = " << src_.label() << " from the Event !!";
    return false;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, BoolEventSelector, "BoolEventSelector");      

#include "CommonTools/UtilAlgos/interface/EventSelectorAdapter.h"

typedef EventSelectorAdapter<BoolEventSelector> BoolEventFilter;

DEFINE_FWK_MODULE(BoolEventFilter);
