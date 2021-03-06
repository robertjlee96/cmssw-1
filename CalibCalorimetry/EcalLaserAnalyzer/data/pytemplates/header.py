import FWCore.ParameterSet.Config as cms

process = cms.Process("TESTMON")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi")
process.load("EventFilter.EcalRawToDigi.ecalMatacq_cfi");
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")


process.AdaptorConfig = cms.Service("AdaptorConfig")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("LmfSource",
    orderedRead = cms.bool(True),
    verbosity = cms.untracked.int32(0),
    fileNames = cms.vstring('input.lmf'),
    preScale = cms.uint32(1)
)

process.matacqAnalyzer = cms.EDFilter("EcalMatacqAnalyzer",
    resDir = cms.untracked.string('.'),
    eventHeaderProducer = cms.string('ecalEBunpacker'),
    eventHeaderCollection = cms.string(''),
    digiCollection = cms.string(''),
    digiProducer = cms.string('ecalMatacq')
)
process.StatusAnalyzer = cms.EDFilter("EcalStatusAnalyzer",
    dataType = cms.untracked.string('p5'),
    resDir = cms.untracked.string('.'),
    eventHeaderProducer = cms.string('ecalEBunpacker'),
    statusFile = cms.untracked.string('header.txt'),
    eventHeaderCollection = cms.string('')
)


process.p = cms.Path(process.ecalEBunpacker*process.ecalMatacq*process.StatusAnalyzer*process.matacqAnalyzer)
