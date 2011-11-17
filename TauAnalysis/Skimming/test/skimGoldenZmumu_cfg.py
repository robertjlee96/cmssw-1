import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim Z --> mu+ mu- candidate events passing "golden" VTBF selection
#--------------------------------------------------------------------------------

process = cms.Process("skimGoldenZmumu2")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

isMC = True
##isMC = False

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
# (only relevant for HPS tau reconstruction algorithm)
if isMC:
    process.GlobalTag.globaltag = cms.string('START42_V13::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_42_V20::All')
#--------------------------------------------------------------------------------    

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_4_2_x/skims/selEvents_data_runs160329to163869_selZmumuCands_met80to100_AOD.root',
        'file:/data1/veelken/CMSSW_4_2_x/skims/selEvents_data_runs165071to167913_selZmumuCands_met80to100_AOD.root'
    )
)

# import event content definition:
# keep full FEVT (RAW + RECO) event content
# plus collections of goodMuons, goldenZmumuCandidates and "tag" & "probe" flags
from TauAnalysis.Skimming.goldenZmmEventContent_cff import *

# load definition of VBTF Z --> mu+ mu- event selection
# (with no isolation cuts applied on one of the two muons)
process.load("TauAnalysis.Skimming.goldenZmmSelectionVBTFnoMuonIsolation_cfi")

# load definitions of data-quality filters
process.load("TauAnalysis.TauIdEfficiency.filterDataQuality_cfi")
if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#--------------------------------------------------------------------------------
# select events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimPath = cms.Path(
    process.dataQualityFilters 
   + process.goldenZmumuSelectionSequence
)

# add event counter for Mauro's "self baby-sitting" technology
process.processedEventsSkimming = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.processedEventsSkimming)

#--------------------------------------------------------------------------------
# save events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    goldenZmumuEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('goldenZmumuSkimPath')
    ),
    fileName = cms.untracked.string('goldenZmumuEvents_data_runs160329to167913_selZmumuCands_met80to100_AOD.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.goldenZmumuSkimOutputModule)

rocessDumpFile = open('skimGoldenZmumu.dump' , 'w')
print >> processDumpFile, process.dumpPython()

