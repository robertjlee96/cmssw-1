// -*- C++ -*-
//
// Class:      HLTHcalMETNoiseFilter
// 
/**\class HLTHcalMETNoiseFilter

 Description: HLT filter module for rejecting MET events due to noise in the HCAL

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Leonard Apanasevich
//         Created:  Wed Mar 25 16:01:27 CDT 2009
// $Id: HLTHcalMETNoiseFilter.cc,v 1.5 2009/09/26 18:13:42 johnpaul Exp $
//
//

#include "HLTrigger/JetMET/interface/HLTHcalMETNoiseFilter.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/METReco/interface/HcalNoiseRBX.h"

#include <iostream>

HLTHcalMETNoiseFilter::HLTHcalMETNoiseFilter(const edm::ParameterSet& iConfig)
  : noisealgo_(iConfig),
    HcalNoiseRBXCollectionTag_(iConfig.getParameter<edm::InputTag>("HcalNoiseRBXCollection")),
    severity_(iConfig.getParameter<int> ("severity")),
    numRBXsToConsider_(iConfig.getParameter<int>("numRBXsToConsider")),
    needHighLevelCoincidence_(iConfig.getParameter<bool>("needHighLevelCoincidence")),
    useLooseRatioFilter_(iConfig.getParameter<bool>("useLooseRatioFilter")),
    useLooseHitsFilter_(iConfig.getParameter<bool>("useLooseHitsFilter")),
    useLooseZerosFilter_(iConfig.getParameter<bool>("useLooseZerosFilter")),
    useLooseTimingFilter_(iConfig.getParameter<bool>("useLooseTimingFilter")),
    useTightRatioFilter_(iConfig.getParameter<bool>("useTightRatioFilter")),
    useTightHitsFilter_(iConfig.getParameter<bool>("useTightHitsFilter")),
    useTightZerosFilter_(iConfig.getParameter<bool>("useTightZerosFilter")),
    useTightTimingFilter_(iConfig.getParameter<bool>("useTightTimingFilter")),
    minRecHitE_(iConfig.getParameter<double>("minRecHitE")),
    minLowHitE_(iConfig.getParameter<double>("minLowHitE")),
    minHighHitE_(iConfig.getParameter<double>("minHighHitE"))
{
}


HLTHcalMETNoiseFilter::~HLTHcalMETNoiseFilter(){}


//
// member functions
//

bool HLTHcalMETNoiseFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace reco;

  // in this case, do not filter anything
  if(severity_==0) return true;

  // get the RBXs produced by RecoMET/METProducers/HcalNoiseInfoProducer
  edm::Handle<HcalNoiseRBXCollection> rbxs_h;
  iEvent.getByLabel(HcalNoiseRBXCollectionTag_,rbxs_h);
  if(!rbxs_h.isValid()) {
    LogDebug("") << "HLTHcalMETNoiseFilter: Could not find HcalNoiseRBXCollection product named "
		 << HcalNoiseRBXCollectionTag_ << "." << std::endl;
    return true;
  }

  // create a sorted set of the RBXs, ordered by energy
  noisedataset_t data;
  for(HcalNoiseRBXCollection::const_iterator it=rbxs_h->begin(); it!=rbxs_h->end(); ++it) {
    const HcalNoiseRBX &rbx=(*it);
    CommonHcalNoiseRBXData d(rbx, minRecHitE_, minLowHitE_, minHighHitE_);
    data.insert(d);
  }

  // data is now sorted by RBX energy
  // only consider top N=numRBXsToConsider_ energy RBXs
  int cntr=0;
  for(noisedataset_t::const_iterator it=data.begin();
      it!=data.end() && cntr<numRBXsToConsider_;
      ++it, ++cntr) {

    /*    bool passFilter=true;
    if(useLooseRatioFilter_ && !noisealgo_.passLooseRatio(*it)) passFilter=false;
    if(useLooseHitsFilter_ && !noisealgo_.passLooseHits(*it)) passFilter=false;
    if(useLooseZerosFilter_ && !noisealgo_.passLooseZeros(*it)) passFilter=false;
    if(useLooseTimingFilter_ && !noisealgo_.passLooseTiming(*it)) passFilter=false;

    if(useTightRatioFilter_ && !noisealgo_.passTightRatio(*it)) passFilter=false;
    if(useTightHitsFilter_ && !noisealgo_.passTightHits(*it)) passFilter=false;
    if(useTightZerosFilter_ && !noisealgo_.passTightZeros(*it)) passFilter=false;
    if(useTightTimingFilter_ && !noisealgo_.passTightTiming(*it)) passFilter=false;

    if(needHighLevelCoincidence_ && !noisealgo_.passHighLevelNoiseFilter(*it) && passFilter) return false;
    if(!needHighLevelCoincidence_ && passFilter) return false;
    */

    bool passFilter=true;
    if(useLooseRatioFilter_ && !noisealgo_.passLooseRatio(*it)) {
      std::cout << "Failed LooseRatioFilter.  Energy=" << it->energy() << "; Ratio=" << it->ratio() << std::endl;
      passFilter=false;
    }
    if(useLooseHitsFilter_ && !noisealgo_.passLooseHits(*it)) {
      std::cout << "Failed LooseHitsFilter.  Energy=" << it->energy() << "; Hits=" << it->numHPDHits() << std::endl;
      passFilter=false;
    }
    if(useLooseZerosFilter_ && !noisealgo_.passLooseZeros(*it)) {
      std::cout << "Failed LooseZerosFilter.  Energy=" << it->energy() << "; Zeros=" << it->numZeros() << std::endl;
      passFilter=false;
    }
    if(useLooseTimingFilter_ && !noisealgo_.passLooseTiming(*it)) {
      std::cout << "Failed LooseTimingFilter.  Energy=" << it->energy() << "; MinTime=" << it->minHighEHitTime() << "; MaxTime=" << it->maxHighEHitTime() << std::endl;
      passFilter=false;
    }

    if(useTightRatioFilter_ && !noisealgo_.passTightRatio(*it)) {
      std::cout << "Failed TightRatioFilter.  Energy=" << it->energy() << "; Ratio=" << it->ratio() << std::endl;
      passFilter=false;
    }
    if(useTightHitsFilter_ && !noisealgo_.passTightHits(*it)) {
      std::cout << "Failed TightHitsFilter.  Energy=" << it->energy() << "; Hits=" << it->numHPDHits() << std::endl;
      passFilter=false;
    }
    if(useTightZerosFilter_ && !noisealgo_.passTightZeros(*it)) {
      std::cout << "Failed TightZerosFilter.  Energy=" << it->energy() << "; Zeros=" << it->numZeros() << std::endl;
      passFilter=false;
    }
    if(useTightTimingFilter_ && !noisealgo_.passTightTiming(*it)) {
      std::cout << "Failed TightTimingFilter.  Energy=" << it->energy() << "; MinTime=" << it->minHighEHitTime() << "; MaxTime=" << it->maxHighEHitTime() << std::endl;
      passFilter=false;
    }

    if(needHighLevelCoincidence_ && !noisealgo_.passHighLevelNoiseFilter(*it) && !passFilter) {
      std::cout << "Requiring coincidence with low EMF. EMF=" << it->RBXEMF() << std::endl;
      return false;
    }
    if(!needHighLevelCoincidence_ && !passFilter) {
      std::cout << "No coincidence required." << std::endl;
      return false;
    }
  }

  // no problems found
  return true;
}
