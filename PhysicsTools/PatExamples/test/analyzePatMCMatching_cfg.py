import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

#------------------------------------------------------------------------------------------------------
# To configure the Matching, we have to configure the PAT-Workflow starting from the patDefaultSequence:
#------------------------------------------------------------------------------------------------------

from PhysicsTools.PatAlgos.patTemplate_cfg import *

## prepare several clones of match associations for status 1, 3 and in flight muons (status -1)
process.muMatch3 = process.muonMatch.clone(mcStatus = [3]) # hard scattering
process.muMatch1 = process.muonMatch.clone(mcStatus = [1]) # stable


## add the new matches to the default sequence
process.patDefaultSequence.replace(process.muonMatch,
                                   process.muMatch1 +
                                   process.muMatch3
)

process.patMuons.genParticleMatch = cms.VInputTag(
    cms.InputTag("muMatch3"),
    cms.InputTag("muMatch1")
)


#-----------------------------------------
# As usual add those two usefull things:
#----------------------------------------

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzePatMCMatching.root')
)

process.MessageLogger = cms.Service("MessageLogger")


#----------------------------------------------------------------------
# Finally let's analyze the matching and run all that in correct order:
#----------------------------------------------------------------------

process.analyzePatMCMatching = cms.EDAnalyzer("PatMCMatching",
  muonSrc     = cms.untracked.InputTag("cleanPatMuons")                                             
)

process.out.outputCommands = cms.untracked.vstring('keep *') 

process.p = cms.Path(process.patDefaultSequence + process.analyzePatMCMatching)

