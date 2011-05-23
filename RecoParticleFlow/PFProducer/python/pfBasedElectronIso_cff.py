import FWCore.ParameterSet.Config as cms

from CommonTools.ParticleFlow.ParticleSelectors.pfCandsForIsolation_cff  import *
from CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff import *
from CommonTools.ParticleFlow.Isolation.pfElectronIsolationFromDeposits_cff import *

pfSelectedElectrons = cms.EDFilter(
    "GenericPFCandidateSelector",
    src = cms.InputTag("particleFlowTmp"),
    cut = cms.string("abs(pdgId())==11")
)

pfPileUp.PFCandidates = cms.InputTag("particleFlowTmp")
pfNoPileUp.bottomCollection = cms.InputTag("particleFlowTmp") 

pfBasedElectronIsoSequence = cms.Sequence(
    pfCandsForIsolationSequence +
    pfSelectedElectrons +
    pfElectronIsolationSequence+
    pfElectronIsolationFromDepositsSequence
    ) 
