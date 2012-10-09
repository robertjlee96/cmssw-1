#ifndef TauAnalysis_MCEmbeddingTools_ParticleReplacerBase_h
#define TauAnalysis_MCEmbeddingTools_ParticleReplacerBase_h

/** \class ParticleReplacerBase
 *
 * Base class for particle replacer algorithms
 *
 * \author Matti Kortelainen
 *
 * \version $Revision: 1.13 $
 *
 * $Id: ParticleReplacerBase.h,v 1.13 2012/10/07 13:09:35 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "HepMC/GenEvent.h"

class ParticleReplacerBase 
{
 public:
  explicit ParticleReplacerBase(const edm::ParameterSet&);
  virtual ~ParticleReplacerBase() {}

  virtual void beginJob() {}
  virtual void beginRun(edm::Run& run, const edm::EventSetup& es) {}
  virtual void endRun() {}
  virtual void endJob() {}

  virtual std::auto_ptr<HepMC::GenEvent> produce(const std::vector<reco::Particle>&, const reco::Vertex* evtVtx = 0, const HepMC::GenEvent* genEvt = 0) = 0;

  unsigned int tried_;
  unsigned int passed_;

 protected:
  const double tauMass_;

  int verbosity_;
};

#endif
