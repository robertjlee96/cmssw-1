#ifndef TauAnalysis_CandidateTools_PFMEtSignCovMatrixProducer_h  
#define TauAnalysis_CandidateTools_PFMEtSignCovMatrixProducer_h

/** \class PFMEtSignCovMatrixProducer
 *
 * Produce covariance matrix representing expected PFMET resolution,
 * computed on event-by-event bases by the (PF)MET significance algorithm
 * (see CMS AN-10/400 for description of the (PF)MEt significance computation)
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PFMEtSignCovMatrixProducer.h,v 1.2 2012/08/28 15:00:24 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "JetMETCorrections/METPUSubtraction/interface/PFMEtSignInterfaceBase.h"

class PFMEtSignCovMatrixProducer : public edm::EDProducer 
{
 public:
  
  PFMEtSignCovMatrixProducer(const edm::ParameterSet&);
  ~PFMEtSignCovMatrixProducer();

  void produce(edm::Event&, const edm::EventSetup&);

 private:

  std::string moduleLabel_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag src_;

  PFMEtSignInterfaceBase pfMEtSignInterface_;
};

#endif


