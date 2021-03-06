import FWCore.ParameterSet.Config as cms

electronHcalTowerIsolationLcone = cms.EDProducer("EgammaTowerIsolationProducer",
    absolut = cms.bool(True),
    intRadius = cms.double(0.0),
    extRadius = cms.double(0.4),
    towerProducer = cms.InputTag("towerMaker"),
    etMin = cms.double(0.0),
    Depth = cms.int32(-1),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


