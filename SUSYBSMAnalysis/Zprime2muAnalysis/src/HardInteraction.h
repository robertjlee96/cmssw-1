#ifndef HARDINTERACTION_H
#define HARDINTERACTION_H

#include <vector>

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "FWCore/Framework/interface/Event.h"

namespace edm {
  class ParameterSet;
}

// A class for extracting and storing the generator-level particles
// that were in the hard interaction (+ the final-state leptons after
// bremsstrahlung) from the MC record, assuming the event was of the
// form q qbar -> resonance -> l+l-, and that there was only one hard
// interaction per event. We store pointers to all these particles.

struct HardInteraction {
  HardInteraction(const edm::ParameterSet cfg);
  HardInteraction(bool doingElec, bool allowFakeRes);
  ~HardInteraction();

  // Determine whether the passed in pdgId is one of the resonances of
  // interest (currently one of Z0 (inc. DY), Z', or G*).
  bool IsResonance(int pdgId) const;

  // Clear out the structure: reset pointers to null, empty
  // bremPhotons, reset flags.
  void Clear();

  // Return whether all the pointers are valid.
  bool IsValid() const;

  // Store pointers to all the particles from the genParticles collection.
  void Fill(const reco::GenParticleCollection& genParticles);
  
  // The same, but get the genParticles from the event.
  void Fill(const edm::Event& event);

  // The tag for the GenParticleCollection.
  const edm::InputTag src;

  // Whether doing electrons.
  const bool doingElectrons;

  // The lepton flavor to look for (e.g. 13 for muons, 11 for electrons).
  const int leptonFlavor;

  // The lepton mass, for convenience later.
  const double leptonMass;

  // Whether to allow the building of the resonance from the doc-line
  // leptons if the resonance is not found in the event record (this
  // is the case for at least some COMPHEP samples).
  const bool allowFakeResonance;

  // Pointers to the particles in the hard interaction (as well as the
  // final-state leptons after brem.)
  const reco::Candidate* quark;
  const reco::Candidate* antiquark;
  const reco::Candidate* resonance;
  const reco::Candidate* lepPlus;
  const reco::Candidate* lepMinus;
  const reco::Candidate* lepPlusNoIB;
  const reco::Candidate* lepMinusNoIB;
  std::vector<const reco::Candidate*> bremPhotons;

  // Flag declaring whether we built the resonance ourselves, and
  // therefore own and should delete its pointer at destruction.
  bool resonanceIsFake;

  // The PDG ids of particles considered to be resonances we want. If
  // empty, a set of defaults will be used.
  std::vector<int> resonanceIds;
};

#endif
