#include <vector>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "SusyAnalysis/EventSelector/interface/ChargedFractionSelector.h"

//________________________________________________________________________________________
ChargedFractionSelector::ChargedFractionSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), jetTag_(pset.getParameter<edm::InputTag> ("jetTag")), minFraction_(
            pset.getParameter<double> ("minFraction")), maxEta_(pset.getParameter<double> ("maxEta")), minPt_(
            pset.getParameter<double> ("minPt")), minTracks_(pset.getParameter<unsigned int> ("minTracks")) {

   // Store computed fraction
   defineVariable("chFraction");
   defineVariable("numberOfJets");

}

//________________________________________________________________________________________
bool ChargedFractionSelector::select(const edm::Event& event) const {

   // reset cached variables
   resetVariables();

   // Get the jets
   edm::Handle<edm::View<pat::Jet> > jets;
   event.getByLabel(jetTag_, jets);
   if (!jets.isValid())
      edm::LogWarning("ChargedFractionSelector") << "No jet results for" << "InputTag " << jetTag_;
   //Let it throw, in case !jets.isValid()

   // Calculate sum of fractions
   float etFrac = 0.;
   int nJets = 0;
   // Loop on jets
   for (edm::View<pat::Jet>::const_iterator iJet = jets->begin(); iJet != jets->end(); ++iJet) {
      float trackPtSum(0.);
      const reco::TrackRefVector& jetTracks = iJet->associatedTracks();
      // Acceptance cuts
      if (iJet->pt() > minPt_ && fabs(iJet->eta()) < maxEta_ && jetTracks.size() >= minTracks_) {
         nJets++;
         // Loop on associated tracks
         for (reco::TrackRefVector::const_iterator iTrack = jetTracks.begin(); iTrack != jetTracks.end(); ++iTrack) {
            trackPtSum += (*iTrack)->pt();
         }
         etFrac += trackPtSum / iJet->et();
      }
   }

   // Average charge fraction: if no jets, keep default (extreme) value
   double chFraction = 0.;
   if (nJets > 0) {
      chFraction = etFrac / static_cast<float> (nJets);
      setVariable("chFraction", chFraction); // Cache variable
      setVariable("numberOfJets", static_cast<float> (nJets)); // Cache variable
   }

   // Return selection
   return chFraction > minFraction_;

}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, ChargedFractionSelector, "ChargedFractionSelector");
