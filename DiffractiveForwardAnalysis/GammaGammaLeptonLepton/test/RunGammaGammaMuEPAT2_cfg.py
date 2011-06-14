import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("gamgam2leplepanalysis")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.categories.append('PATLayer0Summary')
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(
#    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
#)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# DB efficiency stuff - uncomment if running on MC
##process.load("CondCore.DBCommon.CondDBCommon_cfi")
##process.CondDBCommon.connect = 'sqlite_file:ExclJPsiZMuonChargePhysicsPerformance7TeVRun2010B.db'
##process.load("MuonAnalysis.TagAndProbe.ExclMuonPerformaceMergeESSource_cfi")
##process.load("MuonAnalysis.TagAndProbe.ExclMuonPerformaceMergeESProducer_cfi")
# End of DB stuff

# source
process.source = cms.Source("PoolSource", 
                            fileNames = cms.untracked.vstring(
                                'file:/tmp/jjhollar/CalcHEP_GamGamWW_ElEl_SM_RECO.root'
                                                                )
                            )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# Load configuration stuff
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('GR_R_42_V2::All')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

# Load CASTOR FastSim
#process.load("FastSimulation.ForwardDetectors.CastorFastReco_cff")

# Load analysis modules
process.load("DiffractiveForwardAnalysis.GammaGammaLeptonLepton.PATGammaGammaMuE_cfi")

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.2)
                                    )

process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(15),
                                           maxd0 = cms.double(2)
                                           )

process.muonFilter=cms.EDFilter("CandViewCountFilter",
                                src =cms.InputTag("muons"), minNumber = cms.uint32(1))


# Trigger
process.load("DiffractiveForwardAnalysis.GammaGammaLeptonLepton.HLTFilter_cfi")
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = ['HLT_Mu10_Ele10_CaloIdL_*']

process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring("drop *")
                               )

# PAT Layer 0+1
from Configuration.EventContent.EventContent_cff import *
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.tools.coreTools import *
process.load("PhysicsTools.PatAlgos.patSequences_cff")
removeCleaning(process)
removeMCMatching(process, ['All'])
#restrictInputToAOD(process, ['All'])

# Output definition - this is for testing "PATtuples"
#process.output = cms.OutputModule("PoolOutputModule",
#                                  outputCommands = cms.untracked.vstring("drop *"),    
#                                  outputCommands = cms.untracked.vstring('drop *',
#                                                                         'keep *_selectedPatPhotons_*_*',
#                                                                         'keep *_selectedPatElectrons_*_*',
#                                                                         'keep *_selectedPatMuons_*_*',
#                                                                         'keep *_selectedPatTaus_*_*',
#                                                                         'keep *_selectedPatJets_*_*',
#                                                                         'keep *_layer1METs_*_*',
#                                                                         'keep *_selectedPatPFParticles_*_*',
#                                                                         'keep *_CastorFastTowerReco_*_*',
#                                                                         *patExtraAodEventContent ),
#                                  fileName = cms.untracked.string('file:/tmp/jjhollar/mumu.pattuple.root'),
#                                  dataset = cms.untracked.PSet(
#    dataTier = cms.untracked.string('PAT'),
#    filterName = cms.untracked.string('')
#    ),
#)

#offline vertices with deterministic annealing. Should become the default as of 4_2_0_pre7. Requires > V01-04-04      RecoVertex/PrimaryVertexProducer
#process.load("RecoVertex.Configuration.RecoVertex_cff")
#from RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi import *
#process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi")
#process.offlinePrimaryVerticesDA = process.offlinePrimaryVertices.clone()

#process.output.outputCommands.extend(AODEventContent.outputCommands)

# Set to True if running on MC  
##process.gamgammueanalysis.ReadMCEffCorrections = True
process.gamgammueanalysis.outfilename = "/tmp/jjhollar/SignalMuEAnalyzer.root"

# Put it all together
process.p = cms.Path(
#    process.mcgamgammumuanalysis
#    process.CastorFastReco
#    process.hltFilter 
#    + process.offlinePrimaryVerticesDA
    process.patDefaultSequence  
    + process.gamgammueanalysis
#   The output module here is only needed for making 'PATtuples'/skims
#   + process.output
    )

#print process.dumpPython()
