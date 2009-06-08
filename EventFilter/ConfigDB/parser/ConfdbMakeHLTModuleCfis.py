#!/usr/bin/env python

# ConfdbMakeHLTModulesCfis.py
#
# Get list of CVS tags for a release in ConfDB
#
# Jonathan Hollar LLNL March 13, 2009

import os, string, sys, posix, tokenize, array, getopt, operator

sys.path.append(os.environ.get("CMS_PATH") + "/sw/slc4_ia32_gcc345/external/py2-cx-oracle/4.2/lib/python2.4/site-packages/") 
 
import cx_Oracle 

def main(argv):

    input_verbose = 0
    #    input_dbuser = "CMS_HLT_TEST"
    #    input_dbpwd = ""
    #    input_host = "CMS_ORCOFF_INT2R"
    
    input_dbuser = "CMS_HLTDEV_WRITER"
    input_dbpwd = ""
    input_host = "CMS_ORCOFF_PROD" 
    
    input_addtorelease = "none"
    input_release = "CMSSW_3_1_0_pre6"

    opts, args = getopt.getopt(sys.argv[1:], "r:v:d:u:s:oh", ["release=","verbose=","dbname=","user=","password=","dbtype=","hostname=",])

    for o, a in opts:
        if o in ("-r","release="):
            input_release = str(a)
            print "Using release name " + input_release
        if o in ("-d","dbname="):
            input_dbname = str(a)
            print "Using DB named " + input_dbname
        if o in ("-u","user="):
            input_dbuser = str(a)
            print "Connecting as user " + input_dbuser
        if o in ("-s","password="):
            input_dbpwd = str(a)
            print "Use DB password " + input_dbpwd
        if o in ("-o","hostname="):
            input_host = str(a)
            print "Use hostname " + input_host
        if o in ("-v","verbose="):
            input_verbose = int(a)
            print "Verbosity = " + str(input_verbose)

    confdbjob = ConfdbMakeHLTModuleCfis(input_release,input_verbose,input_dbuser,input_dbpwd,input_host)
    confdbjob.BeginJob()

class ConfdbMakeHLTModuleCfis:
    def __init__(self,clirel,cliverbose,clidbuser,clidbpwd,clihost):
        
        self.dbname = ''
        self.dbuser = clidbuser
        self.verbose = int(cliverbose)
        self.dbpwd = clidbpwd
        self.dbhost = clihost
        self.verbose = cliverbose
        self.release = clirel
        
        # Track CVS tags
        self.tagtuple = []
        self.alreadyadded = []
        
        # Get a Conf DB connection here. Only need to do this once at the
        # beginning of a job.
        print "Connecting as " + self.dbuser+"@"+self.dbhost+"/"+self.dbpwd
        self.connection = cx_Oracle.connect(self.dbuser+"/"+self.dbpwd+"@"+self.dbhost) 
        self.dbcursor = self.connection.cursor()  

    def BeginJob(self):
        self.dbcursor.execute("SELECT SoftwareReleases.releaseId FROM SoftwareReleases WHERE (releaseTag = '" + self.release + "')")
        tmprelid = self.dbcursor.fetchone()
        if(tmprelid):
            tmprelid = tmprelid[0]
        else:
            print 'Could not find the release ' + str(self.release) + ' - exiting'
            return
            

        thetemplates = ["ModuleTemplates",
                        "ServiceTemplates",
                        "ESModuleTemplates",
                        "ESSourceTemplates",
                        "EDSourceTemplates"]

        theinstances = ["Modules",
                        "Services",
                        "ESModules",
                        "ESSources",
                        "EDSources"]

        j = 0
        
        for templatetype in thetemplates:
            tagselstr = "SELECT " + str(templatetype) + ".name, " + str(templatetype) + ".cvsTag, " + str(templatetype) + ".packageId, " + str(templatetype) + ".superId FROM " + str(templatetype) + " JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = " + str(templatetype) + ".superId) WHERE SuperIdReleaseAssoc.releaseId = " + str(tmprelid)

            self.dbcursor.execute(tagselstr)
            modstags = self.dbcursor.fetchall()
            packname = ''
            subsysname = ''
            templateid = -1
            instancetype = theinstances[j]
            j = j + 1
            
            for mod, tag, packageid, templateid in modstags:
                packselstr = "SELECT SoftwarePackages.name, SoftwarePackages.subsysId FROM SoftwarePackages WHERE SoftwarePackages.packageId = " + str(packageid)
                self.dbcursor.execute(packselstr)
                packfetch = self.dbcursor.fetchone()
                if(packfetch):
                    packname = packfetch[0]
                    subsysid = packfetch[1]
                    subsysselstr = "SELECT SoftwareSubsystems.name FROM SoftwareSubsystems WHERE SoftwareSubsystems.subsysId = " + str(subsysid)
                    self.dbcursor.execute(subsysselstr)
                    subsysfetch = self.dbcursor.fetchone()
                    if(subsysfetch):
                        subsysname = subsysfetch[0]


                theinstance = ''
                if(instancetype != "Services" and instancetype != "EDSources"):
                    instanceselstr = "SELECT " + str(instancetype) + ".name " + " FROM " + str(instancetype) + " WHERE " + str(instancetype) + ".templateId = " + str(templateid)
                    self.dbcursor.execute(instanceselstr)                
                    thetmpinstance = self.dbcursor.fetchone()
                    if(thetmpinstance):
                        theinstance = thetmpinstance[0]

                dirname = str(subsysname) + "/" + str(packname)
                if(subsysname == 'HLTrigger' or
                   packname == 'EgammaHLTProducers' or
                   packname == 'HLTProducers' or
                   packname == 'L2MuonIsolationProducer' or
                   packname == 'L2MuonProducer' or
                   packname == 'L2MuonSeedGenerator' or
                   packname == 'L3MuonIsolationProducer' or
                   packname == 'L3MuonProducer' or
                   packname == 'L3TrackFinder'):
                    self.tagtuple.append((str(tag),str(dirname),str(mod),str(theinstance)))
                    self.alreadyadded.append(str(dirname))

        sortedtagtuple = sorted(self.tagtuple, key=operator.itemgetter(1))
                                
        for thecvstag, thepackagesubsysname, theplugin, theinst in sortedtagtuple:                        
            print str(thecvstag).ljust(12) + "\t" + str(thepackagesubsysname).ljust(50) + "\t" + str(theplugin).ljust(50) + "\t" + str(theinst).ljust(50)

        foundtemplates = 0
        usedtemplates = 0

        print '\n\n\n'
        theconfig = '/dev/CMSSW_3_1_0/pre6/HLT/V51'
        for thecvstag, thepackagesubsysname, theplugin, theinst in sortedtagtuple:
            foundtemplates = foundtemplates + 1
            if(theinst != ''):
                conffromdbstr = 'edmConfigFromDB --hltdev --configName ' + str(theconfig) + ' --nopaths --nosequences --noservices --noes --nopsets --cff --modules ' + str(theinst) + ' > ' + str(theplugin) + '_cfi.py'
                print conffromdbstr
                usedtemplates = usedtemplates + 1

        print str(usedtemplates) + ' out of ' + str(foundtemplates) + ' were used in the configuration'
                
        self.connection.commit() 
        self.connection.close() 
            
if __name__ == "__main__": 
    main(sys.argv[1:]) 
