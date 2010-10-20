#ifndef PhysicsTools_CandUtils_pdgIdUtils_h
#define PhysicsTools_CandUtils_pdgIdUtils_h
#include "DataFormats/Candidate/interface/Candidate.h"

namespace reco {

  inline bool isElectron(const Candidate & part) { 
    return std::abs(part.pdgId())==11;
  }

  inline bool isMuon(const Candidate & part) { 
    return std::abs(part.pdgId())==13;
  }

  inline bool isTau(const Candidate & part) { 
    return std::abs(part.pdgId())==15;
  }

  inline bool isLepton(const Candidate & part) { 
    return std::abs(part.pdgId())==11 || 
      std::abs(part.pdgId())==13 || 
      std::abs(part.pdgId())==15; 
  }

  inline bool isNeutrino(const Candidate & part) { 
    return std::abs(part.pdgId())==12 || 
      std::abs(part.pdgId())==14 || 
      std::abs(part.pdgId())==16; 
  }

  inline int flavour(const Candidate & part) {
    int id = part.pdgId();
    return id/std::abs(id);
  }

}

#endif
