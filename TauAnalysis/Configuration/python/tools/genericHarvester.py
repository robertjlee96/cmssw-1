#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 3+argument_offset:
    sys.exit("Usage: %s [outputFile] [inputFile1] [inputFile2] ..." % sys.argv[0])

outputFileName = sys.argv[1+argument_offset]
inputFileNames = sys.argv[(2+argument_offset):]

print outputFileName
print inputFileNames

process = cms.Process('genericHarvester')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

process.load = cms.EDAnalyzer("DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(inputFileNames),
        dqmDirectory_store = cms.string('/')
    )
)

process.save = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(outputFileName)
)

process.harvest = cms.Sequence(
    process.load
   + process.save
)

process.p = cms.Path(process.harvest)
