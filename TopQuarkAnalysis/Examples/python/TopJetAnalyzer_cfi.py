import FWCore.ParameterSet.Config as cms

#
# module to make simple analyses of jets
#
analyzeJet = cms.EDAnalyzer("TopJetAnalyzer",
    input = cms.InputTag("selectedLayer1Jets")
)


