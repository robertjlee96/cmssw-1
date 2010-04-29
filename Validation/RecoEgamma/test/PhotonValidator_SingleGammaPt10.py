import FWCore.ParameterSet.Config as cms

process = cms.Process("TestPhotonValidator")
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi")
process.load("SimGeneral.MixingModule.mixNoPU_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("DQMServices.Components.MEtoEDMConverter_cfi")
process.load("Validation.RecoEgamma.photonValidationSequence_cff")
process.load("Validation.RecoEgamma.photonPostprocessing_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_37Y_V1::All'

process.DQMStore = cms.Service("DQMStore");
process.load("DQMServices.Components.DQMStoreStats_cfi")
from DQMServices.Components.DQMStoreStats_cfi import *
dqmStoreStats.runOnEndJob = cms.untracked.bool(True)



process.maxEvents = cms.untracked.PSet(
#input = cms.untracked.int32(10)
)



from Validation.RecoEgamma.photonValidationSequence_cff import *
from Validation.RecoEgamma.photonPostprocessing_cfi import *

photonValidation.OutputMEsInRootFile = True
photonValidation.OutputFileName = 'PhotonValidationRelVal370pre2_SingleGammaPt10.root'

photonPostprocessing.batch = cms.bool(True)
photonPostprocessing.InputFileName = photonValidation.OutputFileName

process.source = cms.Source("PoolSource",
noEventSort = cms.untracked.bool(True),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            
    fileNames = cms.untracked.vstring(
# official RelVal 370pre2 single Photons pt=10GeV
        '/store/relval/CMSSW_3_7_0_pre2/RelValSingleGammaPt10/GEN-SIM-RECO/MC_37Y_V1-v1/0016/9CD40168-8352-DF11-96C4-0026189438E7.root',
        '/store/relval/CMSSW_3_7_0_pre2/RelValSingleGammaPt10/GEN-SIM-RECO/MC_37Y_V1-v1/0016/8A919C6E-8252-DF11-8673-002618943949.root'

    ),
                            
    secondaryFileNames = cms.untracked.vstring(
# official RelVal 370pre2 single Photons pt=10GeV    
        '/store/relval/CMSSW_3_7_0_pre2/RelValSingleGammaPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_37Y_V1-v1/0018/0E087482-F652-DF11-B530-00261894380A.root',
        '/store/relval/CMSSW_3_7_0_pre2/RelValSingleGammaPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_37Y_V1-v1/0016/FAC8766A-8252-DF11-998C-0026189438B8.root',
        '/store/relval/CMSSW_3_7_0_pre2/RelValSingleGammaPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_37Y_V1-v1/0016/B249016A-8352-DF11-8D4A-002618943969.root',
        '/store/relval/CMSSW_3_7_0_pre2/RelValSingleGammaPt10/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_37Y_V1-v1/0016/8A851E6C-8252-DF11-9C8C-00261894395A.root'
    )
 )


photonPostprocessing.rBin = 48

## For single gamma pt =10
photonValidation.eMax  = 100
photonValidation.etMax = 50
photonValidation.etScale = 0.20
photonPostprocessing.eMax  = 100
photonPostprocessing.etMax = 50



process.FEVT = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("keep *_MEtoEDMConverter_*_*"),
    fileName = cms.untracked.string('pippo.root')
)



process.p1 = cms.Path(process.tpSelection*process.photonValidationSequence*process.photonPostprocessing*process.dqmStoreStats)
process.schedule = cms.Schedule(process.p1)



