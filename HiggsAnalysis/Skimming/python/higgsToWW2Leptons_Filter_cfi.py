import FWCore.ParameterSet.Config as cms

higgsToWW2LeptonsFilter = cms.EDFilter("HiggsToWW2LeptonsSkim",
    ElectronCollectionLabel = cms.InputTag("pixelMatchGsfElectrons"),
    SingleLeptonPtMin = cms.double(20.0),
    nLeptons=cms.int32(2),
    etaMin = cms.double(-2.4),
    GlobalMuonCollectionLabel = cms.InputTag("globalMuons"),
    DiLeptonPtMin = cms.double(10.0),
    etaMax = cms.double(2.4),
    beTight = cms.bool(True),
    dilepM = cms.double(6),
    eleHadronicOverEm = cms.double(0.5)

)


