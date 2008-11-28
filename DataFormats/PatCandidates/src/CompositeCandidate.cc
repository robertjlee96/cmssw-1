//
// $Id: CompositeCandidate.cc,v 1.1 2008/10/23 20:21:14 srappocc Exp $
//

#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"


using namespace pat;


/// default constructor
CompositeCandidate::CompositeCandidate() : 
  PATObject<reco::CompositeCandidate>(reco::CompositeCandidate(0, reco::CompositeCandidate::LorentzVector(0, 0, 0, 0), reco::CompositeCandidate::Point(0,0,0))) {
}


/// constructor from CompositeCandidateType
CompositeCandidate::CompositeCandidate(const reco::CompositeCandidate & aCompositeCandidate) : 
  PATObject<reco::CompositeCandidate>(aCompositeCandidate) {
}


/// destructor
CompositeCandidate::~CompositeCandidate() {
}
