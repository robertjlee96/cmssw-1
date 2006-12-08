#ifndef JetReco_GenJet_h
#define JetReco_GenJet_h

/** \class reco::GenJet
 *
 * \short Jets made from MC generator particles
 *
 * GenJet represents Jets made from MC candidates
 * Provide energy contributions from different particle types
 * in addition to generic Jet parameters
 *
 * \author Fedor Ratnikov, UMd
 *
 * \version   Original March 31, 2006 by F.R.
 * \version   $Id: GenJet.h,v 1.8 2006/12/06 22:43:24 fedor Exp $
 ************************************************************/


#include "DataFormats/JetReco/interface/Jet.h"

#include "DataFormats/JetReco/interface/GenJetfwd.h"

namespace reco {
  class GenParticleCandidate;

class GenJet : public Jet {
public:
  struct Specific {
    Specific () :
      m_EmEnergy (0),
	 m_HadEnergy (0),
	 m_InvisibleEnergy (0),
	 m_AuxiliaryEnergy (0) {}

    /// Energy of EM particles
    double m_EmEnergy;
    /// Energy of Hadrons
    double m_HadEnergy;
    /// Invisible energy (mu, nu, ...)
    double m_InvisibleEnergy;
    /// Anything else (undecayed Sigmas etc.)
    double m_AuxiliaryEnergy;
  };

  /** Default constructor*/
  GenJet() {}

  /** Constructor from values*/
  GenJet(const LorentzVector& fP4, const Point& fVertex, const Specific& fSpecific, 
	 const Jet::Constituents& fConstituents);

  /** backward compatible, vertex=(0,0,0) */
  GenJet(const LorentzVector& fP4, const Specific& fSpecific, 
	 const Jet::Constituents& fConstituents);

  virtual ~GenJet() {};
  /** Returns energy of electromagnetic particles*/
  double emEnergy() const {return m_specific.m_EmEnergy;};
  /** Returns energy of hadronic particles*/
  double hadEnergy() const {return m_specific.m_HadEnergy;};
  /** Returns invisible energy*/
  double invisibleEnergy() const {return m_specific.m_InvisibleEnergy;};
  /** Returns other energy (undecayed Sigmas etc.)*/
  double auxiliaryEnergy() const {return m_specific.m_AuxiliaryEnergy;};

  /// convert generic constituent to specific type
  static const GenParticleCandidate* genParticle (const reco::Candidate* fConstituent);
  /// get specific constituent
  const GenParticleCandidate* getConstituent (unsigned fIndex) const;
  /// get all constituents
  std::vector <const GenParticleCandidate*> getConstituents () const;
  
  // block accessors

  const Specific& getSpecific () const {return m_specific;}

  
  /// Polymorphic clone
  virtual GenJet* clone () const;

  /// Print object
  virtual std::string print () const;
  
private:
  /// Polymorphic overlap
  virtual bool overlap( const Candidate & ) const;
  
  // Data members
  //Variables specific to to the GenJet class
  Specific m_specific;
};
}
#endif
