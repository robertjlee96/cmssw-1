import FWCore.ParameterSet.Config as cms

process = cms.Process("ECALDQM")

process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerMapping_cfi")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi")

import RecoLocalCalo.EcalRecProducers.ecalFixedAlphaBetaFitUncalibRecHit_cfi
process.ecalUncalibHit = RecoLocalCalo.EcalRecProducers.ecalFixedAlphaBetaFitUncalibRecHit_cfi.ecalFixedAlphaBetaFitUncalibRecHit.clone()

#import RecoLocalCalo.EcalRecProducers.ecalWeightUncalibRecHit_cfi
#process.ecalUncalibHit = RecoLocalCalo.EcalRecProducers.ecalWeightUncalibRecHit_cfi.ecalWeightUncalibRecHit.clone()

process.load("RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("Geometry.EcalMapping.EcalMapping_cfi")

process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")

process.load("DQM.EcalBarrelMonitorModule.EcalBarrelMonitorModule_cfi")

process.load("DQM.EcalBarrelMonitorTasks.EcalBarrelMonitorTasks_cfi")

process.load("CalibCalorimetry.EcalLaserCorrection.ecalLaserCorrectionService_cfi")

process.load("DQMServices.Core.DQM_cfg")

process.preScaler = cms.EDFilter("Prescaler",
    prescaleFactor = cms.int32(1)
)

process.dqmInfoEB = cms.EDAnalyzer("DQMEventInfo",
    subSystemFolder = cms.untracked.string('EcalBarrel')
)

process.printAscii = cms.OutputModule("AsciiOutputModule")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("EventStreamHttpReader",
    sourceURL = cms.string('http://localhost.localdomain:40003/urn:xdaq-application:lid=44'),

    consumerPriority = cms.untracked.string('normal'),
    max_event_size = cms.int32(7000000),
    consumerName = cms.untracked.string('ECAL BARREL DQM Source'),
    max_queue_depth = cms.int32(5),
    maxEventRequestRate = cms.untracked.double(10.0),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('*')
    ),
    headerRetryInterval = cms.untracked.int32(3)
)

process.load("CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi")

process.EcalTrivialConditionRetriever.adcToGeVEBConstant = cms.untracked.double(0.00875)
process.EcalTrivialConditionRetriever.getWeightsFromFile = False
process.EcalTrivialConditionRetriever.pedWeights = cms.untracked.vdouble(0.333, 0.333, 0.333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
process.EcalTrivialConditionRetriever.pedWeightsAft = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
process.EcalTrivialConditionRetriever.amplWeights = cms.untracked.vdouble(-0.333, -0.333, -0.333, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0)
process.EcalTrivialConditionRetriever.amplWeightsAftGain = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0)
process.EcalTrivialConditionRetriever.jittWeights = cms.untracked.vdouble(0.041, 0.041, 0.041, 0.0, 1.325, -0.05, -0.504, -0.502, -0.390, 0.0)
process.EcalTrivialConditionRetriever.jittWeightsAft = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 1.098, -0.046, -0.416, -0.419, -0.337, 0.0)

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        noLineBreaks = cms.untracked.bool(True),
        noTimeStamps = cms.untracked.bool(True),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        EcalTBRawToDigi = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiTriggerType = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiTpg = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiNumTowerBlocks = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiTowerId = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiTowerSize = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiChId = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiGainZero = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiGainSwitch = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalTBRawToDigiDccBlockSize = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDev = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevTriggerType = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevTpg = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevNumTowerBlocks = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevTowerId = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevTowerSize = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevChId = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevGainZero = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevGainSwitch = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevDccBlockSize = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevMemBlock = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevMemTowerId = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevMemChId = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevMemGain = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevTCC = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalRawToDigiDevSRP = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalDCCHeaderRuntypeDecoder = cms.untracked.PSet(
            limit = cms.untracked.int32(1000)
        ),
        EcalBarrelMonitorModule = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        )
    ),
    categories = cms.untracked.vstring('EcalTBRawToDigi', 
                                       'EcalTBRawToDigiTriggerType', 
                                       'EcalTBRawToDigiTpg', 
                                       'EcalTBRawToDigiNumTowerBlocks', 
                                       'EcalTBRawToDigiTowerId', 
                                       'EcalTBRawToDigiTowerSize', 
                                       'EcalTBRawToDigiChId', 
                                       'EcalTBRawToDigiGainZero', 
                                       'EcalTBRawToDigiGainSwitch', 
                                       'EcalTBRawToDigiDccBlockSize', 
                                       'EcalRawToDigiDev', 
                                       'EcalRawToDigiDevTriggerType', 
                                       'EcalRawToDigiDevTpg', 
                                       'EcalRawToDigiDevNumTowerBlocks', 
                                       'EcalRawToDigiDevTowerId', 
                                       'EcalRawToDigiDevTowerSize', 
                                       'EcalRawToDigiDevChId', 
                                       'EcalRawToDigiDevGainZero', 
                                       'EcalRawToDigiDevGainSwitch', 
                                       'EcalRawToDigiDevDccBlockSize', 
                                       'EcalRawToDigiDevMemBlock', 
                                       'EcalRawToDigiDevMemTowerId', 
                                       'EcalRawToDigiDevMemChId', 
                                       'EcalRawToDigiDevMemGain', 
                                       'EcalRawToDigiDevTCC', 
                                       'EcalRawToDigiDevSRP', 
                                       'EcalDCCHeaderRuntypeDecoder', 
                                       'EcalBarrelMonitorModule'),
    destinations = cms.untracked.vstring('cout')
)

process.ecalDataSequence = cms.Sequence(process.preScaler*process.ecalEBunpacker*process.ecalUncalibHit*process.ecalRecHit)
process.ecalBarrelMonitorSequence = cms.Sequence(process.ecalBarrelMonitorModule*process.dqmInfoEB)

process.p = cms.Path(process.ecalDataSequence*process.ecalBarrelMonitorSequence)
process.q = cms.EndPath(process.ecalBarrelCosmicTasksSequence)
process.r = cms.EndPath(process.printAscii)

process.ecalUncalibHit.MinAmplBarrel = 12.
process.ecalUncalibHit.MinAmplEndcap = 16.
process.ecalUncalibHit.EBdigiCollection = 'ecalEBunpacker:ebDigis'
process.ecalUncalibHit.EEdigiCollection = 'ecalEBunpacker:eeDigis'

process.ecalRecHit.EBuncalibRecHitCollection = 'ecalUncalibHit:EcalUncalibRecHitsEB'
process.ecalRecHit.EEuncalibRecHitCollection = 'ecalUncalibHit:EcalUncalibRecHitsEE'

