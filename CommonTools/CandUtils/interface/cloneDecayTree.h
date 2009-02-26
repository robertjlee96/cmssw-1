#ifndef CandUtils_cloneDecayTree_h
#define CandUtils_cloneDecayTree_h
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include <memory>

std::auto_ptr<reco::Candidate> cloneDecayTree( const reco::Candidate & );

#endif
