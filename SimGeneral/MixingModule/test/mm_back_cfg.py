# The following comments couldn't be translated into the new config version:

# this is an example of how to run playback with input pileup
# the file Cum_store.root is supposed to have been run with 
#     module rndmStore = RandomEngineStateProducer { } or equivalent

import FWCore.ParameterSet.Config as cms

process = cms.Process("PRODMIXBack")
process.load("SimGeneral.MixingModule.mixLowLumPU_cfi")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        mix = cms.untracked.uint32(12345)
    )
)

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(4),
    fileNames = cms.untracked.vstring('file:Cum_store.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *_*_*_*', 
        'keep *_*_*_PRODMIX'),
    fileName = cms.untracked.string('file:Cum_restored.root')
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('mix'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        MixingModule = cms.untracked.PSet(
            limit = cms.untracked.int32(1000000)
        )
    ),
    categories = cms.untracked.vstring('MixingModule'),
    destinations = cms.untracked.vstring('cout')
)

process.p = cms.Path(process.mix)
process.outpath = cms.EndPath(process.out)
process.mix.playback = True


