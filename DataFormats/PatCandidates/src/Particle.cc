//
// $Id: Particle.cc,v 1.1 2008/01/15 12:59:32 lowette Exp $
//

#include "DataFormats/PatCandidates/interface/Particle.h"


using namespace pat;


/// default constructor
Particle::Particle() : PATObject<reco::LeafCandidate>(reco::LeafCandidate(0, reco::LeafCandidate::LorentzVector(0, 0, 0, 0), reco::LeafCandidate::Point(0,0,0))) {
}


/// constructor from reco::LeafCandidate
Particle::Particle(const reco::LeafCandidate & aParticle) : PATObject<reco::LeafCandidate>(aParticle) {
}


/// destructor
Particle::~Particle() {
}
