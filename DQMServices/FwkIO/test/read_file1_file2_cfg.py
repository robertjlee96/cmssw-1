import FWCore.ParameterSet.Config as cms

process = cms.Process("READ")

process.source = cms.Source("DQMRootSource",
                            fileNames = cms.untracked.vstring("file:dqm_file1.root","file:dqm_file2.root"))

seq = cms.untracked.VEventID()
for r in xrange(1,2):
    #begin run
    seq.append(cms.EventID(r,0,0))
    for l in xrange(1,21):
        #begin lumi
        seq.append(cms.EventID(r,l,0))
        #end lumi
        seq.append(cms.EventID(r,l,0))
    #end run
    seq.append(cms.EventID(r,0,0))

process.check = cms.EDAnalyzer("MulticoreRunLumiEventChecker",
                               eventSequence = seq)

process.e = cms.EndPath(process.check)

process.add_(cms.Service("DQMStore"))
process.add_(cms.Service("Tracer"))

