#include <vector>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "SusyAnalysis/EventSelector/interface/uncorrectionTypeMET.h"

#include "SusyAnalysis/EventSelector/interface/MeffSelector.h"

//________________________________________________________________________________________
MeffSelector::MeffSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), jetTag_(pset.getParameter<edm::InputTag> ("jetTag")), metTag_(pset.getParameter<
            edm::InputTag> ("metTag")), minMeff_(pset.getParameter<double> ("minMeff")) {
   // uncorrection type
   uncorrType_ = pat::uncorrectionTypeMET(pset.getParameter<std::string> ("uncorrType"));

   // Define variables to store
   defineVariable("Meff");

}

//________________________________________________________________________________________
bool MeffSelector::select(const edm::Event& event) const {

   // Reset cached variables
   resetVariables();

   // Get the MET result
   edm::Handle<edm::View<pat::MET> > metHandle;
   event.getByLabel(metTag_, metHandle);
   if (!metHandle.isValid()) {
      edm::LogWarning("MeffEventSelector") << "No Met results for InputTag " << metTag_;
      return false;
   }
   if (metHandle->size() != 1) {
      edm::LogWarning("MeffEventSelector") << "MET collection size is " << metHandle->size() << " instead of 1";
      return false;
   }

   // Get the jets
   edm::Handle<edm::View<pat::Jet> > jetHandle;
   event.getByLabel(jetTag_, jetHandle);
   if (!jetHandle.isValid()) {
      edm::LogWarning("MetJetEventSelector") << "No Jet results for InputTag " << jetTag_;
      return false;
   }

   // Compute Meff
   //   float meff = (*metHandle)[0].et();
   float meff = uncorrType_ == pat::MET::uncorrMAXN ? metHandle->front().et() : metHandle->front().uncorrectedPt(
            uncorrType_);
   for (edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin(); iJet != jetHandle->end(); ++iJet) {
      meff += iJet->et();
   }

   setVariable("Meff", meff);

   // Select
   return meff > minMeff_;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MeffSelector, "MeffSelector");

