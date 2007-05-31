#ifndef FastSimulation_ParamL3MuonProducer_ParamL3MuonProducer_h
#define FastSimulation_ParamL3MuonProducer_ParamL3MuonProducer_h

//
// Package:    ParamL3MuonProducer
// Class:      ParamL3MuonProducer
// 
/**\class ParamL3MuonProducer ParamL3MuonProducer.cc FastSimulation/ParamL3MuonProducer/src/ParamL3MuonProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
//  Author:  Andrea Perrotta
// Created:  Wed May 02 12:37:24 CET 2007
// $Id: ParamL3MuonProducer.h,v 1.1 2007/05/03 14:05:37 aperrott Exp $
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// FastSimulation headers
class FSimEvent;
class SimpleL1MuGMTCand;
class FML1EfficiencyHandler;
class FML1PtSmearer;
class FML3EfficiencyHandler; 
class FML3PtSmearer;
class FMGLfromL3EfficiencyHandler; 
class FMGLfromL3TKEfficiencyHandler; 
class FMGLfromTKEfficiencyHandler; 

class RandomEngine;

// Data Formats
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

//
// class declaration
//

class ParamL3MuonProducer : public edm::EDProducer {
   public:

      explicit ParamL3MuonProducer(const edm::ParameterSet&);
      ~ParamL3MuonProducer();

   private:

      const RandomEngine * random;

      typedef std::vector<SimpleL1MuGMTCand*> FML1Muons;
      typedef std::vector<L1MuGMTCand> L1MuonsContainer;

      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void readParameters(const edm::ParameterSet& );
      void reconstruct();
      void loadL1Muons(L1MuonsContainer & c) const;
      void loadL3Muons(reco::MuonCollection & c) const;
      void loadGLMuons(reco::MuonCollection & c) const;
    
  // ---------- member data ---------------------------
      edm::ParameterSet  vertexGenerator_; 
      edm::ParameterSet  particleFilter_;
      FSimEvent* mySimEvent;

      FML1Muons  mySimpleL1MuonCands;
      FML1EfficiencyHandler * myL1EfficiencyHandler;
      FML1PtSmearer * myL1PtSmearer;

      reco::MuonCollection  mySimpleL3MuonCands;
      FML3EfficiencyHandler * myL3EfficiencyHandler;
      FML3PtSmearer * myL3PtSmearer;

      reco::MuonCollection  mySimpleGLMuonCands;
      FMGLfromL3EfficiencyHandler * myGLfromL3EfficiencyHandler;
      FMGLfromL3TKEfficiencyHandler * myGLfromL3TKEfficiencyHandler;
      FMGLfromTKEfficiencyHandler * myGLfromTKEfficiencyHandler;
      FML3PtSmearer * myGLPtSmearer;
      
  // ----------- parameters ---------------------------- 
      bool debug_;
      bool doL1_ , doL3_ , doGL_;
      double minEta_ ,  maxEta_;
  // ----------- counters ------------------------------
      int   nMuonTot , nL1MuonTot , nL3MuonTot , nGLMuonTot;
};

#endif
