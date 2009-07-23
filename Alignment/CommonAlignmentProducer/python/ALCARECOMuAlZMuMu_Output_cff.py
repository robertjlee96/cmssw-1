import FWCore.ParameterSet.Config as cms

# AlCaReco for muon based alignment using ZMuMu events
OutALCARECOMuAlZMuMu_noDrop = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlZMuMu')
    ),
    outputCommands = cms.untracked.vstring(
        'keep *_ALCARECOMuAlZMuMu_*_*', 
        'keep *_muonCSCDigis_*_*', 
        'keep *_muonDTDigis_*_*', 
        'keep *_muonRPCDigis_*_*', 
        'keep *_dt1DRecHits_*_*', 
        'keep *_dt2DSegments_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*',
	'keep *_MEtoEDMConverter_*_*')
)

import copy
OutALCARECOMuAlZMuMu = copy.deepcopy(OutALCARECOMuAlZMuMu_noDrop)
OutALCARECOMuAlZMuMu.outputCommands.insert(0, "drop *")

