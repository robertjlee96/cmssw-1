#include "SusyAnalysis/EventSelector/interface/HTPtdrEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

//__________________________________________________________________________________________________
HTPtdrEventSelector::HTPtdrEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  metTag_( pset.getParameter<edm::InputTag>("metTag") ),
  minHT_ ( pset.getParameter<double>("minHT") ),
  minPt_ ( pset.getParameter<double>("minPt") )
{ 

  // Store computed HT
  defineVariable("HT");

}

//__________________________________________________________________________________________________
bool
HTPtdrEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();
  // Get the jets
  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("HTPtdrEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }
  // Get the met
  edm::Handle< edm::View<pat::MET> > metHandle;
  event.getByLabel(metTag_, metHandle);
  if ( !metHandle.isValid() ) {
    edm::LogWarning("HTPtdrEventSelector") << "No Met results for InputTag " << metTag_;
    return false;
  }
  if ( metHandle->size()!=1 ) {
    edm::LogWarning("HTPtdrEventSelector") << "MET collection size is " 
					  << metHandle->size() << " instead of 1";
    return false;
  }

  // Sum over jet Ets (with cut on min. pt)
  float myHT = metHandle->front().et();
  for ( size_t i=1; i<std::min(static_cast<size_t>(4),jetHandle->size()); ++i ) {
    if ( (*jetHandle)[i].pt()>minPt_ ) myHT += (*jetHandle)[i].et();
  }

  setVariable("HT",myHT);

  return myHT>minHT_;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, HTPtdrEventSelector, "HTPtdrEventSelector");
