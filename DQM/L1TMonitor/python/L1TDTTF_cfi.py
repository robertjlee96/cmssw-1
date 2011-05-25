import FWCore.ParameterSet.Config as cms

l1tDttf = cms.EDAnalyzer("L1TDTTF",
    verbose = cms.untracked.bool(False),
    DQMStore = cms.untracked.bool(True),
    l1tSystemFolder = cms.untracked.string('L1T/L1TDTTF'),
    disableROOToutput = cms.untracked.bool(True),
    online = cms.untracked.bool(True),
    l1tInfoFolder = cms.untracked.string('L1T/EventInfo'),
    dttpgSource = cms.InputTag("dttfDigis","","DQM"),
    gmtSource = cms.InputTag("gtDigis","","DQM"),
    MuonCollection = cms.InputTag("muons")
)


