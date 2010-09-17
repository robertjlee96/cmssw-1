import FWCore.ParameterSet.Config as cms


dqmProvInfo = cms.EDAnalyzer("DQMProvInfo",
    subSystemFolder = cms.untracked.string('Info'),
    provInfoFolder = cms.untracked.string('ProvInfo'),
    globaltag = cms.string("DEFAULT:GT")
)
