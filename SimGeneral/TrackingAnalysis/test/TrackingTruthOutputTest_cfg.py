import FWCore.ParameterSet.Config as cms

process = cms.Process('rackingTruthOutputTest')

process.load('FWCore/MessageService/MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200)
)

process.source = cms.Source(
  'PoolSource',
  fileNames = cms.untracked.vstring('file:TrackingTruthPlayback.root')
)

process.myOutputTest = cms.EDAnalyzer(
  'TrackingTruthOutputTest',
  trackingTruth = cms.untracked.InputTag('mergedtruth', 'MergedTrackTruth'),
  dumpVertexes = cms.untracked.bool(False),
  dumpOnlyBremsstrahlung = cms.untracked.bool(True)  
)

process.p = cms.EndPath(process.myOutputTest)
