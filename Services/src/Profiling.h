#ifndef FWCore_Services_Profiling_h
#define FWCore_Services_Profiling_h
// -*- C++ -*-
//
// Package:     Services
// Class  :     SimpleProfiling
// 
//
// Original Author:  Jim Kowalkowski
// $Id$
//

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"

namespace edm {
  class ConfigurationDescriptions;

  namespace service {
    class SimpleProfiling
    {
    public:
      SimpleProfiling(const ParameterSet&,ActivityRegistry&);
      ~SimpleProfiling();

      static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
      
      void postBeginJob();
      void postEndJob();
      
    private:
    };
  }
}



#endif
