#ifndef HLTHcalNZSFilter_h
#define HLTHcalNZSFilter_h
// -*- C++ -*-
//
// Package:    HLTHcalNZSFilter
// Class:      HLTHcalNZSFilter
// 
/**\class HLTHcalNZSFilter HLTHcalNZSFilter.cc filter/HLTHcalNZSFilter/src/HLTHcalNZSFilter.cc

Description: Filter to select HCAL non-ZS events

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Bryan DAHMES
//         Created:  Tue Jan 22 13:55:00 CET 2008
// $Id: HLTHcalNZSFilter.h,v 1.2 2009/08/06 07:12:54 fwyzard Exp $
//
//


// include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"

#include <string>

//
// class declaration
//

class HLTHcalNZSFilter : public HLTFilter {
public:
  explicit HLTHcalNZSFilter(const edm::ParameterSet&);
  virtual ~HLTHcalNZSFilter();
  
private:
  virtual void beginJob(void);
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob(void);
  
  // ----------member data ---------------------------
  
  edm::InputTag dataInputTag_;
  bool          summary_;
  int           eventsNZS_; 

};

#endif
