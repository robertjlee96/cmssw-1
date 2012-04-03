#!/usr/bin/env python

import copy
import os
import shlex
import socket
import subprocess
import sys
import time

from TauAnalysis.CandidateTools.tools.buildConfigFilesSVfitPerformanceAnalysis import *
from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import buildConfigFile_hadd
from TauAnalysis.Configuration.tools.jobtools import make_bsub_script
from TauAnalysis.Configuration.tools.harvestingLXBatch import make_harvest_scripts
from TauAnalysis.Configuration.tools.harvesting import castor_source
import TauAnalysis.Configuration.tools.castor as castor

version = '2012Mar13'

inputFilePath  = '/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/SVfitStudies/'
harvestingFilePath = '/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/harvesting/SVfitStudies/AHtautau_2012Mar29/'
outputFilePath = '/tmp/veelken/svFitStudies/' 

samplesToAnalyze = [
    'ggHiggs120',
    'vbfHiggs120',
    'ggHiggs130',
    'vbfHiggs130',
    'ggPhi160',
    'bbPhi160',
    'ggPhi200',
    'bbPhi200',
    'ggPhi300',
    'bbPhi300',
    'ggPhi450',
    'bbPhi450',
    'ZplusJets'
]

channelsToAnalyze = [
    'muTau',
    ##'eleTau',
    ##'eleMu',
    ##'diTau'
]

#runSVfitEventHypothesisAnalyzer = True
runSVfitEventHypothesisAnalyzer = False
runLXBatchHarvesting = True
#runLXBatchHarvesting = False

def createFilePath_recursively(filePath):
    filePath_items = filePath.split('/')
    currentFilePath = "/"
    for filePath_item in filePath_items:
        currentFilePath = os.path.join(currentFilePath, filePath_item)
        if len(currentFilePath) <= 1:
            continue
        if not os.path.exists(currentFilePath):
            #sys.stdout.write("creating directory %s\n" % currentFilePath)
            os.mkdir(currentFilePath)

hostname = socket.gethostname()
print "hostname = %s" % hostname
if hostname == 'ucdavis.cern.ch':
    print "Running on %s" % hostname

try:
    castor.rfstat(harvestingFilePath)
except RuntimeError:
    # harvestingFilePath does not yet exist, create it
    print "harvestingFilePath does not yet exist, creating it."
    os.system("rfmkdir %s" % harvestingFilePath)
    os.system("rfchmod 777 %s" % harvestingFilePath)
    
outputFilePath = os.path.join(outputFilePath, version)
print "outputFilePath = %s" % outputFilePath
createFilePath_recursively(outputFilePath)

configFilePath = os.path.join(os.getcwd(), "lxbatch")
print "configFilePath = %s" % configFilePath
createFilePath_recursively(configFilePath)

logFilePath = os.path.join(os.getcwd(), "lxbatch_log")
print "logFilePath = %s" % logFilePath
createFilePath_recursively(logFilePath)

execDir = "%s/bin/%s/" % (os.environ['CMSSW_BASE'], os.environ['SCRAM_ARCH'])

executable_cmsRun = 'cmsRun'
executable_bsub = 'bsub'
executable_waitForLXBatchJobs = 'python %s/src/TauAnalysis/Configuration/python/tools/waitForLXBatchJobs.py' % os.environ['CMSSW_BASE']
executable_rfcp = 'rfcp'
executable_rfrm = 'rfrm'
executable_hadd = 'hadd -f'
executable_shell = '/bin/csh'

bsubQueue = "1nw"

configFileName_SVfitEventHypothesisAnalyzer_template = "runSVfitPerformanceAnalysis_AHtautau_cfg.py"

def runCommand(commandLine):
    sys.stdout.write("%s\n" % commandLine)
    args = shlex.split(commandLine)
    retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
    retVal.wait()
    return retVal

# find and delete "bad" files
##files = [ file_info for file_info in castor.nslsl(harvestingFilePath) ]
##for file in files:
##    if file['size'] < 1000:
##        runCommand("%s %s" % (executable_rfrm, file['path']))

#--------------------------------------------------------------------------------
#
# build config files for running FWLiteTauFakeRateAnalyzer macro on lxbatch
#
fileNames_SVfitEventHypothesisAnalyzer         = {}
bsubJobNames_SVfitEventHypothesisAnalyzer      = {}
bjobListFileNames_SVfitEventHypothesisAnalyzer = {}
for sampleToAnalyze in samplesToAnalyze:
    fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze]         = {}
    bsubJobNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze]      = {}
    bjobListFileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze] = {}
    for channelToAnalyze in channelsToAnalyze:
        retVal_SVfitEventHypothesisAnalyzer = \
          buildConfigFile_SVfitEventHypothesisAnalyzer(sampleToAnalyze, channelToAnalyze,
                                                       configFileName_SVfitEventHypothesisAnalyzer_template,
                                                       os.path.join(inputFilePath, version, channelToAnalyze), 1,
                                                       configFilePath, logFilePath, harvestingFilePath)        
        if retVal_SVfitEventHypothesisAnalyzer is not None:
            fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze] = retVal_SVfitEventHypothesisAnalyzer
        else:
            fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze] = {
                'inputFileNames'  : [],
                'configFileNames' : [],
                'outputFileNames' : [],
                'logFileNames'    : []
            }
        fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]['bsubScriptFileNames'] = []
  
        bsubJobNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze] = []
        bjobListFileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze] = None

        if retVal_SVfitEventHypothesisAnalyzer is None:
            continue

        for i in range(len(retVal_SVfitEventHypothesisAnalyzer['inputFileNames'])):

            # The None in the tuple indicates that batch job has no dependencies on other batch jobs
            input_files_and_jobs = \
              [ (None, os.path.join(inputFilePath, version, channelToAnalyze, inputFileName)) \
                for inputFileName in retVal_SVfitEventHypothesisAnalyzer['inputFileNames'][i] ]

            def log_file_maker(job_hash):
                log_fileName = os.path.join(logFilePath, retVal_SVfitEventHypothesisAnalyzer['logFileNames'][i])
                # CV: delete log-files from previous job submissions
                os.system("rm -f %s" % log_fileName)
                return log_fileName

            # Build script for batch job submission
            jobName, bsubScript = make_bsub_script(
                os.path.join(harvestingFilePath, retVal_SVfitEventHypothesisAnalyzer['outputFileNames'][i]),
                input_files_and_jobs,
                log_file_maker,
                "%s %s" % (executable_cmsRun,
                           os.path.join(configFilePath, retVal_SVfitEventHypothesisAnalyzer['configFileNames'][i])))

            #print "configFilePath = %s" % configFilePath
            #print "retVal_SVfitEventHypothesisAnalyzer['logFileNames'][i] = %s" % retVal_SVfitEventHypothesisAnalyzer['logFileNames'][i]
            
            bsubScriptFileName = \
                os.path.join(configFilePath, retVal_SVfitEventHypothesisAnalyzer['logFileNames'][i].replace(".log", ".sh"))
            bsubScriptFile = open(bsubScriptFileName, "w")
            bsubScriptFile.write(bsubScript)
            bsubScriptFile.close()

            fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]['bsubScriptFileNames'].append(bsubScriptFileName)

            bsubJobName = "svFitPerfAna%s%s_%i" % (sampleToAnalyze, channelToAnalyze, i)
            bsubJobNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze].append(bsubJobName)

        bjobListFileName = \
          os.path.join(configFilePath, "batchJobs_SVfitEventHypothesisAnalyzer_%s_%s.lst" % (sampleToAnalyze, channelToAnalyze))
        bjobListFile = open(bjobListFileName, "w")
        for bsubJobName in bsubJobNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]:
            bjobListFile.write("%s\n" % bsubJobName)
        bjobListFile.close()
        
        bjobListFileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze] = bjobListFileName
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build shell script for running 'hadd' in order to collect histograms
# for single sample and single channel into single .root file
#
bsubFileNames_harvesting    = {}
bsubJobNames_harvesting     = {}
bsubJobNames_harvesting_all = []
for sampleToAnalyze in samplesToAnalyze:
    bsubFileNames_harvesting[sampleToAnalyze] = {}
    bsubJobNames_harvesting[sampleToAnalyze]  = {}
    for channelToAnalyze in channelsToAnalyze:

        plot_regex = r"[a-zA-Z0-9._]+"
        skim_regex = r"dont match anything"
        
        def local_copy_mapper(sample):
            return os.path.join(
                outputFilePath,
                'svFitPerformanceAnalysisPlots_%s_%s_%s_harvested.root' % (channelToAnalyze, sampleToAnalyze, version))

        inputFileInfos = []
        for inputFileName in fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]['outputFileNames']:
            inputFileInfo = {
                'path'        : os.path.join(harvestingFilePath, inputFileName),
                'size'        : 1,           # dummy
                'time'        : time.localtime(),
                'file'        : inputFileName,
                'permissions' : 'mrw-r--r--' # "ordinary" file access permissions
            }
            #print "inputFileInfo = %s" % inputFileInfo
            inputFileInfos.append(inputFileInfo)

        retVal_make_harvest_scripts = make_harvest_scripts(
            plot_regex,
            skim_regex,
            channel = channelToAnalyze,
            sampleToAnalyze = sampleToAnalyze,
            job_id = version,
            input_files_info = inputFileInfos,
            harvester_command = executable_hadd,
            abort_on_rfcp_error = False,
            castor_output_directory = harvestingFilePath,
            script_directory = configFilePath,
            merge_script_name = \
            os.path.join(configFilePath, "_".join(['submit', sampleToAnalyze, channelToAnalyze, 'merge']) + '.sh'),
            local_copy_mapper = local_copy_mapper,
            chunk_size = 2.e+9, # 2 GB
            check_old_files = False,
            max_bsub_concurrent_file_access = 250,
            verbosity = 0
        )

        bsubFileNames_harvesting[sampleToAnalyze][channelToAnalyze] = retVal_make_harvest_scripts

        bsubJobName = "harvest%s%s" % (sampleToAnalyze, channelToAnalyze)
        bsubJobNames_harvesting[sampleToAnalyze][channelToAnalyze] = bsubJobName

        if len(retVal_make_harvest_scripts['final_harvest_files']) > 0:
            bsubJobNames_harvesting_all.append(bsubJobName)

bjobListFileName_harvesting = os.path.join(configFilePath, "batchJobs_harvesting_all.lst")
bjobListFile_harvesting = open(bjobListFileName_harvesting, "w")
for sampleToAnalyze in samplesToAnalyze:
    for channelToAnalyze in channelsToAnalyze:
        for bsubJobName in bsubFileNames_harvesting[sampleToAnalyze][channelToAnalyze]['bsub_job_names']:        
            bjobListFile_harvesting.write("%s\n" % bsubJobName)
bjobListFile_harvesting.close()
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build shell script for running 'hadd' in order to collect histograms
# for all samples and event selections into single .root file
#
final_haddInputFileNames = []
for sampleToAnalyze in samplesToAnalyze:
    for channelToAnalyze in channelsToAnalyze:
        for final_harvest_file in bsubFileNames_harvesting[sampleToAnalyze][channelToAnalyze]['final_harvest_files']:
            # CV:
            #    (1) file name of final harvesting output file is stored at index[1] in final_harvest_file-tuple
            #       (cf. TauAnalysis/Configuration/python/tools/harvestingLXBatch.py)
            #    (2) assume that .root files containing histograms for single sample and single channel
            #        are copied to local disk via rfcp prior to running 'hadd'
            final_haddInputFileNames.append(os.path.join(outputFilePath, os.path.basename(final_harvest_file[1])))
final_haddShellFileName = os.path.join(configFilePath, 'harvestSVfitPerformanceHistograms_%s.csh' % version)
final_haddOutputFileName = os.path.join(outputFilePath, 'svFitPerformanceAnalysisPlots_all_%s.root' % version)
retVal_final_hadd = \
  buildConfigFile_hadd(executable_hadd, final_haddShellFileName, final_haddInputFileNames, final_haddOutputFileName)
final_haddJobName = 'final_hadd'
final_haddLogFileName = retVal_final_hadd['logFileName']
#--------------------------------------------------------------------------------

def make_MakeFile_vstring(list_of_strings):
    retVal = ""
    for i, string_i in enumerate(list_of_strings):
        if i > 0:
            retVal += " "
        retVal += string_i
    return retVal

# done building config files, now build Makefile...
makeFileName = "Makefile_SVfitPerformanceAnalysis_AHtautau"
makeFile = open(makeFileName, "w")
makeFile.write("\n")
outputFileNames_make = []
if runSVfitEventHypothesisAnalyzer:
    for sampleToAnalyze in samplesToAnalyze:
        for channelToAnalyze in channelsToAnalyze:
            fileNameEntry = fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]
            if fileNameEntry is None or len(fileNameEntry['inputFileNames']) == 0:
                continue
            for i in range(len(fileNameEntry['inputFileNames'])):
                outputFileNames_make.append(fileNameEntry['outputFileNames'][i])
if runLXBatchHarvesting:
    for sampleToAnalyze in samplesToAnalyze:
        for channelToAnalyze in channelsToAnalyze:
            outputFileNames_make.append(bsubJobNames_harvesting[sampleToAnalyze][channelToAnalyze])
makeFile.write("all: %s\n" %
  (make_MakeFile_vstring(outputFileNames_make)))
makeFile.write("\techo 'Finished running SVfitPerformanceAnalysis.'\n")
makeFile.write("\n")
if runSVfitEventHypothesisAnalyzer:
    for sampleToAnalyze in samplesToAnalyze:
        for channelToAnalyze in channelsToAnalyze:
            fileNameEntry = fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]
            if fileNameEntry is None or len(fileNameEntry['inputFileNames']) == 0:
                continue
            bsubJobEntry = bsubJobNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]
            for i in range(len(fileNameEntry['inputFileNames'])):
                makeFile.write("%s:\n" %
                  (fileNameEntry['outputFileNames'][i]))
                makeFile.write("\t%s -q %s -J %s < %s\n" %
                  (executable_bsub,
                   bsubQueue,
                   bsubJobEntry[i],
                   fileNameEntry['bsubScriptFileNames'][i]))
makeFile.write("\n")
if runLXBatchHarvesting:     
    for sampleToAnalyze in samplesToAnalyze:
        for channelToAnalyze in channelsToAnalyze:
            if runSVfitEventHypothesisAnalyzer:
                makeFile.write("%s: %s\n" %
                  (bsubJobNames_harvesting[sampleToAnalyze][channelToAnalyze],
                   make_MakeFile_vstring(fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]['outputFileNames'])))
                makeFile.write("\t%s %s\n" %
                  (executable_waitForLXBatchJobs,
                   bjobListFileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]))
            else:
                makeFile.write("%s:\n" %
                  (bsubJobNames_harvesting[sampleToAnalyze][channelToAnalyze]))
            makeFile.write("\t%s %s\n" %
              (executable_shell,
               bsubFileNames_harvesting[sampleToAnalyze][channelToAnalyze]['harvest_script_name']))
    makeFile.write("%s: %s\n" %
      (final_haddJobName,
       final_haddOutputFileName))
    makeFile.write("\t%s %s\n" %
      (executable_waitForLXBatchJobs,
       bjobListFileName_harvesting))
if not (runSVfitEventHypothesisAnalyzer or runLXBatchHarvesting):
    for final_haddInputFileName in final_haddInputFileNames:
        makeFile.write("\t%s %s %s\n" %
          (executable_rfcp,
           os.path.join(harvestingFilePath, os.path.basename(final_haddInputFileName)),
           final_haddInputFileName))
    makeFile.write("\t%s %s\n" %
      (executable_shell,
       final_haddShellFileName))
makeFile.write("\n")
makeFile.write(".PHONY: clean\n")
makeFile.write("clean:\n")
for sampleToAnalyze in samplesToAnalyze:
    for channelToAnalyze in channelsToAnalyze:
        if runSVfitEventHypothesisAnalyzer:
            fileNameEntry = fileNames_SVfitEventHypothesisAnalyzer[sampleToAnalyze][channelToAnalyze]
            if fileNameEntry is None:
                continue
            for outputFileName in fileNameEntry['outputFileNames']:
                makeFile.write("\t- %s %s\n" %
                  (executable_rfrm,
                   os.path.join(harvestingFilePath, outputFileName)))
        if runLXBatchHarvesting: 
            for final_harvest_file in bsubFileNames_harvesting[sampleToAnalyze][channelToAnalyze]['final_harvest_files']:
                # CV: file name of final harvesting output file is stored at index[1] in final_harvest_file-tuple
                #    (cf. TauAnalysis/Configuration/python/tools/harvestingLXBatch.py)    
                makeFile.write("\t- %s %s\n" %
                  (executable_rfrm,
                   os.path.join(harvestingFilePath, final_harvest_file[1])))
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(final_haddInputFileNames))            
makeFile.write("\trm -f %s\n" % final_haddShellFileName)
makeFile.write("\trm -f %s\n" % final_haddOutputFileName)
makeFile.write("\techo 'Finished deleting old files.'\n")
makeFile.write("\n")
makeFile.close()

print("Finished building Makefile. Now execute 'make -j 8 -f %s'." % makeFileName)
