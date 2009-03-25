#! /usr/bin/env python

import os
import sys
import fileinput
import string

##########################################################################
##########################################################################
######### User variables

#Reference release
NewRelease='CMSSW_3_1_0_pre4'

# startup and ideal sample list
#startupsamples= ['RelValTTbar', 'RelValZMM']
#startupsamples= [RelValSingleMuPt10', 'RelValSingleMuPt100', 'RelValSingleMuPt1000', 'RelValTTbar','RelValZMM']
startupsamples= ['']

#idealsamples= ['RelValSingleMuPt10', 'RelValSingleMuPt100', 'RelValSingleMuPt1000', 'RelValTTbar','RelValZMM']
idealsamples= ['RelValSingleMuPt10']



# track algorithm name and quality. Can be a list.
Algos= ['']
Qualities=['']
#Qualities=['', 'highPurity']

#Leave unchanged unless the track collection name changed
Tracksname=''

# Sequence. Possible values:
#   -only_validation
#   -re_tracking
#   -digi2track
#   -only_validation_and_TP
#   -re_tracking_and_TP
#   -digi2track_and_TP
#   -harvesting

#Sequence='only_validation_and_TP'
Sequence='harvesting'

Submit=False
DBS=True
OneAtATime=False

# Ideal and Statup tags
IdealTag='IDEAL'
StartupTag='STARTUP'

IdealTagUse='IDEAL_30X'
StartupTagUse='STARTUP_30X'

# Reference directory name (the macro will search for ReferenceSelection_Quality_Algo)
ReferenceSelection='IDEAL_30X_noPU'
StartupReferenceSelection='STARTUP_30X_noPU'

# Default label is GlobalTag_noPU__Quality_Algo. Change this variable if you want to append an additional string.
NewSelectionLabel='_FSIM'

WorkDirBase = '/tmp/aperrott'
#WorkDirBase = '/afs/cern.ch/user/a/aeverett/scratch0'

#Reference and new repository
RefRepository = '/afs/cern.ch/cms/Physics/muon/CMSSW/Performance/RecoMuon/Validation/val'
NewRepository = '/afs/cern.ch/cms/Physics/muon/CMSSW/Performance/RecoMuon/Validation/val'

#Default Nevents
defaultNevents ='-1'

#Put here the number of event to be processed for specific samples (numbers must be strings) if not specified is -1:
#Events={} #{ 'RelValZMM':'5000', 'RelValTTbar':'5000'}
Events={ 'RelValZMM':'5000', 'RelValTTbar':'5000','RelValSingleMuPt10':'10000',  'RelValSingleMuPt100':'10000'}

# template file names. Usually should not be changed.
cfg='muonReleaseValidationFastSim_cfg.py'
macro='macro/TrackValHistoPublisher.C'


#########################################################################
#########################################################################
############ Functions

def replace(map, filein, fileout):
    replace_items = map.items()
    while 1:
        line = filein.readline()
        if not line: break
        for old, new in replace_items:
            line = string.replace(line, old, new)
        fileout.write(line)
    fileout.close()
    filein.close()
    
############################################

    
def do_validation(samples, GlobalTag, trackquality, trackalgorithm):
    global Sequence, RefSelection, RefRepository, NewSelection, NewRepository, defaultNevents, Events, GlobalTagUse
    global cfg, macro, Tracksname
    print 'Search Tag: ' + GlobalTag
    print 'Tag to use: ' + GlobalTagUse

    #build the New Selection name
    NewSelection=GlobalTag +'_noPU'
    if( trackquality !=''):
        NewSelection+='_'+trackquality
    if(trackalgorithm!=''):
        NewSelection+='_'+trackalgorithm
    if(trackquality =='') and (trackalgorithm==''):
        if(Tracksname==''):
            NewSelection+='_ootb'
            Tracks='generalTracks'
        else:
           NewSelection+= Tracks
    elif(Tracksname==''):
        Tracks='cutsRecoTracks'
    else:
        Tracks=Tracksname
    NewSelection+=NewSelectionLabel

    #loop on all the requested samples
    for sample in samples :
        print 'Get information from DBS for sample', sample

        newdir=NewRepository+'/'+NewRelease+'/'+NewSelection+'/'+sample 

        WorkDir = WorkDirBase+'/'+NewRelease+'/'+NewSelection+'/'+sample

        if(os.path.exists(WorkDir)==False):
            os.makedirs(WorkDir)

        
        #chech if the sample is already done
        if(os.path.isfile(newdir+'/val.'+sample+'.rootytootie' )!=True):    
            
            #search the primary dataset
            cmd='./DDSearchCLI.py  --limit -1 --input="find  dataset.createdate, dataset where dataset like *'
            #search for correct EventContent (and site)
#            cmd+=sample+'/'+NewRelease+'_'+GlobalTag+'*GEN-SIM-DIGI-RAW-HLTDEBUG-RECO* AND site like *cern* "'
#            cmd+=sample+'/'+NewRelease+'_'+GlobalTag+'*GEN-SIM-RECO* AND site like *cern* "'
#            cmd+='|grep '+sample+'|grep -v FastSim |sort| tail -1 | cut -d "," -f2 '
            cmd+=sample+'/'+NewRelease+'_'+GlobalTag+'*GEN-SIM-DIGI-RECO* AND site like *cern* "'
            cmd+='|grep '+sample+'|grep FastSim |sort| tail -1 | cut -d "," -f2 '
            print cmd
            dataset= os.popen(cmd).readline()
            print 'DataSet:  ', dataset, '\n'
            
            #Check if a dataset is found
            if(dataset!="" or DBS==False):
                print 'dataset found'
                #Find and format the list of files
                cmd2='./DDSearchCLI.py  --limit -1 --cff --input="find file where dataset like '+ dataset +'"|grep ' + sample 

                thisFile=0
                for thisFilename in os.popen(cmd2).readlines():
                    templatecfgFile = open(cfg, 'r')
                    thisFile=thisFile+1
                    filenames='import FWCore.ParameterSet.Config as cms\n\n'
                    filenames+='readFiles = cms.untracked.vstring()\n'
                    filenames+='secFiles = cms.untracked.vstring()\n'
                    filenames+='source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)\n'
                    filenames+='readFiles.extend( [\n'
                    if dataset!="":
                        if (OneAtATime==False):
                            for filename in os.popen(cmd2).readlines():
                                filenames+=filename
                        else:
                            filenames+=thisFilename
                    filenames+=']);\n'

                
                # if not harvesting find secondary file names
                    if(dataset!="" and Sequence!="harvesting"):
                        print 'Getting secondary files'
 #                       cmd3='./DDSearchCLI.py  --limit -1 --input="find dataset.parent where dataset like '+ dataset +'"|grep ' + sample
                        cmd3=cmd
                        parentdataset=os.popen(cmd3).readline()
                        print 'Parent DataSet:  ', parentdataset, '\n'
                    
                        #Check if a dataset is found
                        if parentdataset!="":
                            cmd4='./DDSearchCLI.py  --limit -1 --cff --input="find file where dataset like '+ parentdataset +'"|grep ' + sample 
                            filenames+='secFiles.extend( [\n'
                            first=True                        
                            for line in os.popen(cmd4).readlines():
                                filenames+=line
                            filenames+=']);\n'
                        else :
                            print "No primary dataset found skipping sample: ", sample
                            continue
                    else :
                        filenames+='secFiles.extend( (               ) )\n'

                    cfgFileName=('%s_%d') % (sample,thisFile)
                    print 'cfgFileName ' + cfgFileName
                    cfgFile = open(cfgFileName+'.py' , 'w' )
                    cfgFile.write(filenames)

                    if (Events.has_key(sample)!=True):
                        Nevents=defaultNevents
                    else:
                        Nevents=Events[sample]
                    print 'line 199'
                    symbol_map = { 'NEVENT':Nevents, 'GLOBALTAG':GlobalTagUse, 'SEQUENCE':Sequence, 'SAMPLE': sample, 'ALGORITHM':trackalgorithm, 'QUALITY':trackquality, 'TRACKS':Tracks}


                    cfgFile = open(cfgFileName+'.py' , 'a' )
                    replace(symbol_map, templatecfgFile, cfgFile)

                    cmdrun='cmsRun ' + WorkDir +'/'+cfgFileName+ '.py >&  ' + cfgFileName + '.log < /dev/zero '

                    print cmdrun

                    lancialines='#!/usr/local/bin/bash \n'
                    lancialines+='cd '+ProjectBase+'/src \n'
                    lancialines+='eval `scramv1 run -sh` \n\n'
                    lancialines+='cd '+WorkDir+'\n'
                    lancialines+='cmsRun '+cfgFileName+'.py  >&  ' + cfgFileName + '.log < /dev/zero \n'
                    lancialines+='mv  DQM_V0001_R000000001__' + GlobalTagUse+ '__' + sample + '__Validation.root' + ' ' + 'val.' +sample+'.root \n'
                    
                    lanciaName=('lancia_%s_%s_%d') % (GlobalTag,sample,thisFile)
                    lanciaFile = open(lanciaName,'w')
                    lanciaFile.write(lancialines)
                    lanciaFile.close()
                    
                    print ("copying py file for sample: %s %s") % (sample,lanciaName)
                    iii = os.system('mv '+lanciaName+' '+WorkDir+'/.')
                    print iii
                    iii = os.system('mv '+cfgFileName+'.py '+WorkDir)
                    print iii
                    if(OneAtATime==False):
                        break

                retcode=0

                if(Submit):
                    retcode=os.system(cmdrun)
                    if os.path.isfile(sample+'.log'):
                        os.system('mv '+sample+'.log ' + WorkDir)
                else:
                    continue

                if (retcode!=0):
                    print 'Job for sample '+ sample + ' failed. \n'
                else:
                    if Sequence=="harvesting":
                        os.system('mv  DQM_V0001_R000000001__' + GlobalTag+ '__' + sample + '__Validation.root' + ' ' + WorkDir+'/val.' +sample+'.root')
                    if os.path.isfile('fullOutput.'+sample+'.root'):
                        os.system('mv fullOutput.'+sample+'.root ' + WorkDir)
                    if os.path.isfile('output.'+sample+'.root'):
                        os.system('mv output.'+sample+'.root ' + WorkDir)
                        
            else:
                print 'No dataset found skipping sample: '+ sample, '\n'
        else:
            print 'Validation for sample ' + sample + ' already done. Skipping this sample. \n'




##########################################################################
#########################################################################
######## This is the main program
            
try:
     #Get some environment variables to use
     if Submit:
         print 'Will Submit job'
         #NewRelease     = os.environ["CMSSW_VERSION"]
     ProjectBase    = os.environ["CMSSW_BASE"]
     #else:
         #NewRelease='CMSSW_2_2_3'


      
except KeyError:
     print >>sys.stderr, 'Error: The environment variable CMSSW_VERSION is not available.'
     print >>sys.stderr, '       Please run eval `scramv1 runtime -csh` to set your environment variables'
     sys.exit()


#print 'Running validation on the following samples: ', samples

if os.path.isfile( 'DDSearchCLI.py')!=True:
    e =os.system("wget --no-check-certificate https://cmsweb.cern.ch/dbs_discovery/aSearchCLI -O DDSearchCLI.py")
    if  e != 0:
        print >>sys.stderr, "Failed to dowload dbs aSearch file (https://cmsweb.cern.ch/dbs_discovery/aSearchCLI)"
        print >>sys.stderr, "Child was terminated by signal", e
        os.remove('DDSearchCLI.py')
        sys.exit()
    else:
        os.system('chmod +x DDSearchCLI.py')

NewSelection=''

for algo in Algos:
    print 'algo ' + algo
    for quality in Qualities:
        print 'quality ' + quality
        RefSelection=ReferenceSelection
        print 'Before RefSelection: ' + RefSelection
        if( quality !=''):
            RefSelection+='_'+quality
        if(algo!=''):
            RefSelection+='_'+algo
        if(quality =='') and (algo==''):
            RefSelection+='_ootb'
        print 'After RefSelection: ' + RefSelection
        GlobalTagUse=IdealTagUse
        do_validation(idealsamples, IdealTag, quality , algo)
        RefSelection=StartupReferenceSelection
        print 'Before StartupRefSelection: ' + RefSelection
        if( quality !=''):
            RefSelection+='_'+quality
        if(algo!=''):
            RefSelection+='_'+algo
        if(quality =='') and (algo==''):
            RefSelection+='_ootb'
        print 'After StartupRefSelection: ' + RefSelection
        GlobalTagUse=StartupTagUse
        do_validation(startupsamples, StartupTag, quality , algo)

