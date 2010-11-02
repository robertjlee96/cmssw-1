#ifndef SusyAnalysis_EventSelector_MyMHTEventSelector_h_
#define SusyAnalysis_EventSelector_MyMHTEventSelector_h_
/// Selector for MHT
///
/// Computes HT from jets after selection on pT and |eta|
///   and cuts on it. HT is defined as scalar sum of the
///   the transverse jet energies.
///
/// $Id: MyMHTEventSelector.h,v 1.6 2010/10/27 17:57:31 csander Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class MyMHTEventSelector: public SusyEventSelector {
   public:
      MyMHTEventSelector(const edm::ParameterSet&);
      virtual bool select(const edm::Event&) const;
      virtual ~MyMHTEventSelector() {
      }

   private:
      edm::InputTag jetTag_; ///< tag for input collection
      float minMHT_; ///< lower MHT cut
      float maxMHT_; ///< upper MHT cut
      float maxMHTsig_; ///< upper MHT significance cut
      float minPt_; ///< minimum Pt of jets taken into account
      float maxEta_; ///< maximum Eta of jets taken into account
      bool useJetID_;
      bool rejectEvtJetID_;

};
#endif
