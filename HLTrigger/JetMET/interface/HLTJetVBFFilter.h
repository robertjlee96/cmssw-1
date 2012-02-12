#ifndef HLTJetVBFFilter_h
#define HLTJetVBFFilter_h

/** \class HLTJetVBFFilter
 *
 *  \author Monica Vazquez Acosta (CERN)
 *
 */

#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"

namespace edm {
   class ConfigurationDescriptions;
}

//
// class decleration
//

template<typename T>
class HLTJetVBFFilter : public HLTFilter {

   public:
      explicit HLTJetVBFFilter(const edm::ParameterSet&);
      ~HLTJetVBFFilter();
      static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
      virtual bool hltFilter(edm::Event&, const edm::EventSetup&, trigger::TriggerFilterObjectWithRefs & filterproduct);

   private:
      edm::InputTag inputTag_; // input tag identifying jets
      double minPtLow_;
      double minPtHigh_;
      bool   etaOpposite_;
      double minDeltaEta_;
      double minInvMass_;
      double maxEta_;
      bool   leadingJetOnly_;
      int    triggerType_;
};

#endif //HLTJetVBFFilter_h
