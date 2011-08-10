import FWCore.ParameterSet.Config as cms
import sys

process = cms.Process("READMERGE")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = cms.untracked.string('ERROR')

process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")

process.AdaptorConfig = cms.Service("AdaptorConfig",
    stats = cms.untracked.bool(False)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(
      'file:reduced_test.root'
    )
)

process.testmerge = cms.EDAnalyzer("TestMergeResults",
                            
    #   These values below are just arbitrary and meaningless
    #   We are checking to see that the value we get out matches what
    #   was put in.

    #   expected values listed below come in sets of three
    #      value expected in Thing
    #      value expected in ThingWithMerge
    #      value expected in ThingWithIsEqual
    #   This set of 3 is repeated below at each point it might change
    #   The Prod suffix refers to objects from the process named PROD
    #   The New suffix refers to objects created in the most recent process
    #   When the sequence of parameter values is exhausted it stops checking
    #   0's are just placeholders, if the value is a "0" the check is not made
    #   and it indicates the product does not exist at that point.
    #   *'s indicate lines where the checks are actually run by the test module.

    expectedBeginRunNew = cms.untracked.vint32(
        0,           0,      0,   # start
        0,           0,      0,   # begin file 1
        10001,   10002,  10003,   # * begin run 100
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 100
        10001,   10002,  10003,   # * begin run 1
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 1
        10001,   10002,  10003,   # * begin run 1
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 1
        10001,   10002,  10003,   # * begin run 2
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 2
        10001,   10002,  10003,   # * begin run 11
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 11
        10001,   10002,  10003,   # * begin run 12
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 12
        10001,   10002,  10003,   # * begin run 13
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 13
        10001,   10002,  10003,   # * begin run 1000
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 1000
        10001,   10002,  10003,   # * begin run 1001
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 1001
        10001,   10002,  10003,   # * begin run 1002
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 1002
        10001,   10002,  10003,   # * begin run 2000
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 2000
        10001,   10002,  10003,   # * begin run 2001
        10001,   20004,  10003,   # * events
        10001,   20004,  10003,   # end run 2001
        10001,   10002,  10003,   # * begin run 2002
        10001,   20004,  10003,   # * events
        10001,   20004,  10003    # end run 2002
    ),

    expectedEndRunNew = cms.untracked.vint32(
        0,           0,      0,   # start
        0,           0,      0,   # begin file 1
        100001,   100002,  100003,   # begin run 100
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 100
        100001,   100002,  100003,   # begin run 1
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 1
        100001,   100002,  100003,   # begin run 1
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 1
        100001,   100002,  100003,   # begin run 2
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 2
        100001,   100002,  100003,   # begin run 11
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 11
        100001,   100002,  100003,   # begin run 12
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 12
        100001,   100002,  100003,   # begin run 13
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 13
        100001,   100002,  100003,   # begin run 1000
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 1000
        100001,   100002,  100003,   # begin run 1001
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 1001
        100001,   100002,  100003,   # begin run 1002
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 1002
        100001,   100002,  100003,   # begin run 2000
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # * end run 2000
        100001,   100002,  100003,   # begin run 2001
        100001,   200004,  100003,   # * events
        100001,   200004,  100003,   # *end run 2001
        100001,   100002,  100003,   # begin run 2002
        100001,   200004,  100003,   # * events
        100001,   200004,  100003    # * end run 2002
    ),

    expectedBeginLumiNew = cms.untracked.vint32(
        0,           0,      0,   # start
        0,           0,      0,   # begin file 1
        101,       102,    103,   # * begin run 100 lumi 100
        101,       204,    103,   # * events
        101,       204,    103    # end run 100 lumi 100
# There are more, but all with the same pattern as the first        
    ),

    expectedEndLumiNew = cms.untracked.vint32(
        0,           0,      0,   # start
        0,           0,      0,   # begin file 1
        1001,     1002,   1003,   # begin run 100 lumi 100
        1001,     2004,   1003,   # * events
        1001,     2004,   1003,   # * end run 100 lumi 100
    ),

    expectedProcessHistoryInRuns = cms.untracked.vstring(
        'PROD',            # Run 100
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 1 in files 1, 8, 9, 10
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 1 in files 2, 3, 11
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 2 in file 2
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 11 in file 5
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 12 in file 5
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 13 in file 5
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 1000 in file 6
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 1001 in file 6
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 1002 in file 6
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 2000 in file 7
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 2001 in file 7
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE',
        'PROD',            # Run 2002 in file 7
        'EXTRA',
        'MERGE',
        'MERGETWOFILES',
        'READMERGE'
    ),
    verbose = cms.untracked.bool(True)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:"+sys.argv[2]),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
)

process.test = cms.EDAnalyzer('RunLumiEventAnalyzer',
    verbose = cms.untracked.bool(True),
    expectedRunLumiEvents = cms.untracked.vuint32(
100,   0,   0,
100, 100,   0,
100, 100, 100,
100, 100, 100,
100, 100,   0,
100,   0,   0,
  1,   0,   0,  
  1,   1,   0,
  1,   1,  11,
  1,   1,  12,
  1,   1,  13,
  1,   1,  14,
  1,   1,  15,
  1,   1,  16,
  1,   1,  17,
  1,   1,  18,
  1,   1,  19,
  1,   1,  20,
  1,   1, 102,
  1,   1,  11,
  1,   1,  12,
  1,   1,  13,
  1,   1,  14,
  1,   1,  15,
  1,   1,  16,
  1,   1,  17,
  1,   1,  18,
  1,   1,  19,
  1,   1,  20,
  1,   1, 102,
  1,   1,   0,
  1,   0,   0,
  1,   0,   0,
  1,   1,   0,
  1,   1,  21,
  1,   1,  22,
  1,   1,  23,
  1,   1,  24,
  1,   1,  25,
  1,   1,   1,
  1,   1,   2,
  1,   1,   3,
  1,   1,   4,
  1,   1,   5,
  1,   1,   6,
  1,   1,   7,
  1,   1,   8,
  1,   1,   9,
  1,   1,  10,
  1,   1,  21,
  1,   1,  22,
  1,   1,  23,
  1,   1,  24,
  1,   1,  25,
  1,   1,   1,
  1,   1,   2,
  1,   1,   3,
  1,   1,   4,
  1,   1,   5,
  1,   1,   6,
  1,   1,   7,
  1,   1,   8,
  1,   1,   9,
  1,   1,  10,
  1,   1,   0,
  1,   2,   0,
  1,   2,   0,
  1,   0,   0,
  2,   0,   0,
  2,   1,   0,
  2,   1,   1,
  2,   1,   2,
  2,   1,   3,
  2,   1,   4,
  2,   1,   5,
  2,   1,   1,
  2,   1,   2,
  2,   1,   3,
  2,   1,   4,
  2,   1,   5,
  2,   1,   0,
  2,   0,   0
)
)

process.test.expectedRunLumiEvents.extend([
 11,   0,   0,
 11,   1,   0,
 11,   1,   1,
 11,   1,   1,
 11,   1,   0,
 11,   2,   0,
 11,   2,   1,
 11,   2,   2,
 11,   2,   3,
 11,   2,   1,
 11,   2,   2,
 11,   2,   3,
 11,   2,   0,
 11,   3,   0,
 11,   3,   4,
 11,   3,   5,
 11,   3,   6,
 11,   3,   4,
 11,   3,   5,
 11,   3,   6,
 11,   3,   0,
 11,   4,   0,
 11,   4,   7,
 11,   4,   8,
 11,   4,   9,
 11,   4,   7,
 11,   4,   8,
 11,   4,   9,
 11,   4,   0,
 11,   0,   0,
 12,   0,   0,
 12,   2,   0,
 12,   2,   1,
 12,   2,   2,
 12,   2,   3,
 12,   2,   1,
 12,   2,   2,
 12,   2,   3,
 12,   2,   0,
 12,   3,   0,
 12,   3,   4,
 12,   3,   5,
 12,   3,   6,
 12,   3,   4,
 12,   3,   5,
 12,   3,   6,
 12,   3,   0,
 12,   4,   0,
 12,   4,   7,
 12,   4,   8,
 12,   4,   9,
 12,   4,   7,
 12,   4,   8,
 12,   4,   9,
 12,   4,   0,
 12,   0,   0,
 13,   0,   0,
 13,   2,   0,
 13,   2,   1,
 13,   2,   2,
 13,   2,   1,
 13,   2,   2,
 13,   2,   0,
 13,   0,   0,
1000,  0,   0,
1000,  1,   0,
1000,  1,   0,
1000,  0,   0,
1001,  0,   0,
1001,  1,   0,
1001,  1,   0,
1001,  0,   0,
1002,  0,   0,
1002,  1,   0,
1002,  1,   0,
1002,  0,   0,
# Between ~3_1_0  and 3_7_X these following are not in the input file
# because runs with no lumis in the input were always dropped.
# The test passes, it just never gets past this point.
2000,  0,   0,
2000,  0,   0,
2001,  0,   0,
2001,  0,   0,
2002,  0,   0,
2002,  0,   0
])

process.path1 = cms.Path(process.test*process.testmerge)

process.ep = cms.EndPath(process.output)
