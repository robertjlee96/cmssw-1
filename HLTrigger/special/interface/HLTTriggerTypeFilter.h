#ifndef HLTTriggerTypeFilter_h
#define HLTTriggerTypeFilter_h
// -*- C++ -*-
//
// Package:    HLTTriggerTypeFilter
// Class:      HLTTriggerTypeFilter
// 
/**\class HLTTriggerTypeFilter HLTTriggerTypeFilter.cc 

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Giovanni FRANZONI
//         Created:  Tue Jan 22 13:55:00 CET 2008
// $Id: TriggerTypeFilter.cc,v 1.7 2008/09/02 08:25:39 gruen Exp $
//
//


// include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"


//
// class declaration
//

class HLTTriggerTypeFilter : public HLTFilter {
public:
  explicit HLTTriggerTypeFilter(const edm::ParameterSet&);
  ~HLTTriggerTypeFilter();
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------  
  unsigned short  SelectedTriggerType_;
  
};

#endif
