import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.CondDBCommon.connect = 'sqlite_file:DropBoxMetadata.db'


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
                            firstRun = cms.untracked.uint32(10)
                            )

# process.PoolDBOutputService.DBParameters.messageLevel = 3


process.mywriter = cms.EDAnalyzer("ProduceDropBoxMetadata",
                                  write = cms.untracked.bool(True),
                                  toWrite = cms.VPSet(cms.PSet(record   = cms.untracked.string("BeamSpotObjectsRcdByRun"), 
                                                               destDB              = cms.untracked.string("oracle://cms_orcon_prod/CMS_COND_31X_BEAMSPOT"),
                                                               destDBValidation    = cms.untracked.string("oracle://cms_orcoff_prep/CMS_COND_BEAMSPOT"),
                                                               tag                 = cms.untracked.string("BeamSpotObjects_PCL_byRun_v0_offline"),
                                                               Timetype            = cms.untracked.string("runnumber"),
                                                               IOVCheck            = cms.untracked.string("All"),
                                                               DuplicateTagHLT     = cms.untracked.string("BeamSpotObjects_PCL_byRun_v0_hlt"),
                                                               DuplicateTagEXPRESS = cms.untracked.string(""),
                                                               DuplicateTagPROMPT  = cms.untracked.string("BeamSpotObjects_PCL_byRun_v0_prompt"),
                                                               )
                                                      ),
                                  read = cms.untracked.bool(True),
                                  toRead = cms.untracked.vstring("BeamSpotObjectsRcdByRun") # same strings as fType
                                  )


process.p = cms.Path(process.mywriter)

from CondCore.DBCommon.CondDBCommon_cfi import CondDBCommon
CondDBCommon.connect = "sqlite_file:DropBoxMetadata.db"

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                  CondDBCommon,
                                  toPut = cms.VPSet(cms.PSet(record = cms.string('DropBoxMetadataRcd'),
                                                             tag = cms.string('DropBoxMetadata'),
                                                             timetype   = cms.untracked.string('runnumber')
                                                             )
                                                             ),
                                  loadBlobStreamer = cms.untracked.bool(False),
                                  #    timetype   = cms.untracked.string('lumiid')
                                  #    timetype   = cms.untracked.string('runnumber')
                                  )

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START311_V1A::All'
#process.GlobalTag.connect   = 'sqlite_file:/afs/cern.ch/user/c/cerminar/public/Alca/GlobalTag/GR_R_311_V2.db'

process.GlobalTag.toGet = cms.VPSet(
  cms.PSet(record = cms.string("DropBoxMetadataRcd"),
           tag = cms.string("DropBoxMetadata"),
           connect = cms.untracked.string("sqlite_file:DropBoxMetadata_read.db")
           #connect = cms.untracked.string("sqlite_file:PFCalibration.db")
          )
)
