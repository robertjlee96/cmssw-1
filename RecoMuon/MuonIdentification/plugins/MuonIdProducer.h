#ifndef MuonIdentification_MuonIdProducer_h
#define MuonIdentification_MuonIdProducer_h

// -*- C++ -*-
//
// Package:    MuonIdentification
// Class:      MuonIdProducer
// 
/*

 Description: reco::Muon producer that can fill various information:
              - track-segment matching
              - energy deposition
              - muon isolation
              - muon hypothesis compatibility (calorimeter)
              Acceptable inputs:
              - reco::TrackCollection
              - reco::MuonCollection
              - reco::MuonTrackLinksCollection
*/
//
// Original Author:  Dmytro Kovalskyi
// $Id: MuonIdProducer.h,v 1.4 2007/09/25 19:14:51 dmytro Exp $
//
//


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "Utilities/Timing/interface/TimerStack.h"

#include "RecoMuon/MuonIdentification/interface/MuonCaloCompatibility.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoExtractor.h"

class MuonIdProducer : public edm::EDProducer {
 public:
   enum TrackType { InnerTrack, OuterTrack, CombinedTrack };
	
   explicit MuonIdProducer(const edm::ParameterSet&);
   
   virtual ~MuonIdProducer();
   
   virtual void produce(edm::Event&, const edm::EventSetup&);

 private:
   bool          fillMuonId( edm::Event&, const edm::EventSetup&, reco::Muon& aMuon );
   void          fillArbitrationInfo( reco::MuonCollection* );
   void          fillMuonIsolation( edm::Event&, const edm::EventSetup&, reco::Muon& aMuon );
   void          init( edm::Event&, const edm::EventSetup& );
   
   // make a muon based on a track ref
   reco::Muon    makeMuon( edm::Event& iEvent, const edm::EventSetup& iSetup, 
			   const reco::TrackRef& track, TrackType type);
   // make a global muon based on the links object
   reco::Muon    makeMuon( const reco::MuonTrackLinks& links );
   
   // make a muon based on track (p4)
   reco::Muon    makeMuon( const reco::Track& track );

   // check if a silicon track satisfies the trackerMuon requirements
   bool          isGoodTrack( const reco::Track& track );
   
   // check number of common DetIds for a given trackerMuon and a stand alone
   // muon track
   int           overlap(const reco::Muon& muon, const reco::Track& track);

   unsigned int  getChamberId(const DetId&);
     
   TrackDetectorAssociator trackAssociator_;
   TrackAssociatorParameters parameters_;
   
   std::vector<edm::InputTag> inputCollectionLabels_;
   std::vector<std::string>   inputCollectionTypes_;

   // selections
   double minPt_;
   double minP_;
   int    minNumberOfMatches_;
   double maxAbsEta_;
   
   // matching
   double maxAbsDx_;
   double maxAbsPullX_;
   double maxAbsDy_;
   double maxAbsPullY_;
   
   // what information to fill
   bool fillCaloCompatibility_;
   bool fillEnergy_;
   bool fillMatching_;
   bool fillIsolation_;
   
   bool debugWithTruthMatching_;

   edm::Handle<reco::TrackCollection>             innerTrackCollectionHandle_;
   edm::Handle<reco::TrackCollection>             outerTrackCollectionHandle_;
   edm::Handle<reco::MuonCollection>              muonCollectionHandle_;
   edm::Handle<reco::MuonTrackLinksCollection>    linkCollectionHandle_;
   
   MuonCaloCompatibility muonCaloCompatibility_;
   muonisolation::MuIsoExtractor* muIsoExtractorCalo_;
   muonisolation::MuIsoExtractor* muIsoExtractorTrack_;
};
#endif
