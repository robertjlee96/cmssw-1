import os
import globalDictionaries
from dataset import Dataset
from TkAlExceptions import AllInOneError


class GenericValidation:
    defaultReferenceName = "DEFAULT"
    def __init__(self, valName, alignment, config):
        import random
        self.name = valName
        self.alignmentToValidate = alignment
        self.general = config.getGeneral()
        self.randomWorkdirPart = "%0i"%random.randint(1,10e9)
        self.configFiles = []
        self.filesToCompare = {}
        self.jobmode = self.general["jobmode"]
        # check, if it has advantages to include the config as validation member
        self.config = config

    def getRepMap(self, alignment = None):
        if alignment == None:
            alignment = self.alignmentToValidate
        result = alignment.getRepMap()
        result.update( self.general )
        result.update({
                "workdir": os.path.join( self.general["workdir"],
                                         self.randomWorkdirPart ),
                "datadir": self.general["datadir"],
                "logdir": self.general["logdir"],
                "dbLoad": alignment.getLoadTemplate(),
                "APE": alignment.getAPETemplate(),
                "CommandLineTemplate": ("#run configfile and post-proccess it\n"
                                        "cmsRun %(cfgFile)s\n"
                                        "%(postProcess)s "),
                "CMSSW_BASE": os.environ['CMSSW_BASE'],
                "SCRAM_ARCH": os.environ['SCRAM_ARCH'],
                "alignmentName": alignment.name,
                "condLoad": alignment.getConditions()
                })
        return result

    def getCompareStrings( self, requestId = None ):
        result = {}
        repMap = self.alignmentToValidate.getRepMap()
        for validationId in self.filesToCompare:
            repMap["file"] = self.filesToCompare[ validationId ]
            if repMap["file"].startswith( "/castor/" ):
                repMap["file"] = "rfio:%(file)s"%repMap
            result[validationId]= "%(file)s=%(name)s|%(color)s|%(style)s"%repMap
        if requestId == None:
            return result
        else:
            if not "." in requestId:
                requestId += ".%s"%GenericValidation.defaultReferenceName
            if not requestId.split(".")[-1] in result:
                msg = ("could not find %s in reference Objects!"
                       %requestId.split(".")[-1])
                raise AllInOneError(msg)
            return result[ requestId.split(".")[-1] ]

    def createFiles( self, fileContents, path ):
        result = []
        for fileName in fileContents:
            filePath = os.path.join( path, fileName)
            theFile = open( filePath, "w" )
            theFile.write( fileContents[ fileName ] )
            theFile.close()
            result.append( filePath )
        return result

    def createConfiguration(self, fileContents, path, schedule= None):
        self.configFiles = GenericValidation.createFiles(self, fileContents,
                                                         path) 
        if not schedule == None:
            schedule = [os.path.join( path, cfgName) for cfgName in schedule]
            for cfgName in schedule:
                if not cfgName in self.configFiles:
                    msg = ("scheduled %s missing in generated configfiles: %s"
                           %(cfgName, self.configFiles))
                    raise AllInOneError(msg)
            for cfgName in self.configFiles:
                if not cfgName in schedule:
                    msg = ("generated configuration %s not scheduled: %s"
                           %(cfgName, schedule))
                    raise AllInOneError(msg)
            self.configFiles = schedule
        return self.configFiles

    def createScript(self, fileContents, path, downloadFiles=[] ):        
        self.scriptFiles = GenericValidation.createFiles(self, fileContents,
                                                         path)
        for script in self.scriptFiles:
            os.chmod(script,0755)
        return self.scriptFiles

    def createCrabCfg(self, fileContents, path ):        
        self.crabConfigFiles = GenericValidation.createFiles(self, fileContents,
                                                             path)
        return self.crabConfigFiles


class GenericValidationData(GenericValidation):
    """Subclass of `GenericValidation` which is the base for validations using
    datasets.
    """
    
    def __init__(self, valName, alignment, config, valType):
        """This method adds additional items to the `self.general` dictionary
        which are only needed for validations using datasets.
        
        Arguments:
        - `valName`: String which identifies individual validation instances
        - `alignment`: `Alignment` instance to validate
        - `config`: `BetterConfigParser` instance which includes the
                    configuration of the validations
        - `valType`: String which specifies the type of validation
        """

        GenericValidation.__init__(self, valName, alignment, config)
        defaults = {"jobmode": self.jobmode,
                    "runRange": "",
                    "firstRun": "",
                    "lastRun": "",
                    "begin": "",
                    "end": "",
                    "JSON": ""
                    }
        theUpdate = config.getResultingSection(valType+":"+self.name,
                                               defaultDict = defaults)
        self.general.update(theUpdate)
        self.jobmode = self.general["jobmode"]
        if self.general["dataset"] not in globalDictionaries.usedDatasets:
            globalDictionaries.usedDatasets[self.general["dataset"]] = Dataset(
                self.general["dataset"] )
        self.dataset = globalDictionaries.usedDatasets[self.general["dataset"]]
        
        if not self.jobmode.split( ',' )[0] == "crab":
            try:
                self.general["datasetDefinition"] = self.dataset.datasetSnippet(
                    jsonPath = self.general["JSON"],
                    nEvents = self.general["maxevents"],
                    firstRun = self.general["firstRun"],
                    lastRun = self.general["lastRun"],
                    begin = self.general["begin"],
                    end = self.general["end"] )
            except AllInOneError, e:
                msg = "In section [%s:%s]: "%(valType, self.name)
                msg += str(e)
                raise AllInOneError(msg)
        else:
            if self.dataset.predefined():
                msg = ("For jobmode 'crab' you cannot use predefined datasets "
                       "(in your case: '%s')."%( self.dataset.name() ))
                raise AllInOneError( msg )
            if self.general["begin"] or self.general["end"]:
                ( self.general["begin"],
                  self.general["end"],
                  self.general["firstRun"],
                  self.general["lastRun"] ) = self.dataset.convertTimeToRun(
                    firstRun = self.general["firstRun"],
                    lastRun = self.general["lastRun"],
                    begin = self.general["begin"],
                    end = self.general["end"],
                    shortTuple = False)
                if self.general["begin"] == None:
                    self.general["begin"] = ""
                if self.general["end"] == None:
                    self.general["end"] = ""
                self.general["firstRun"] = str( self.general["firstRun"] )
                self.general["lastRun"] = str( self.general["lastRun"] )
            if ( not self.general["firstRun"] ) and \
                   ( self.general["end"] or self.general["lastRun"] ):
                self.general["firstRun"] = str(
                    self.dataset.runList()[0]["run_number"])
            if ( not self.general["lastRun"] ) and \
                   ( self.general["begin"] or self.general["firstRun"] ):
                self.general["lastRun"] = str(
                    self.dataset.runList()[-1]["run_number"])
            if self.general["firstRun"] and self.general["lastRun"]:
                if int(self.general["firstRun"]) > int(self.general["lastRun"]):
                    msg = ( "The lower time/runrange limit ('begin'/'firstRun') "
                            "chosen is greater than the upper time/runrange limit "
                            "('end'/'lastRun').")
                    raise AllInOneError( msg )
                self.general["runRange"] = (self.general["firstRun"]
                                            + '-' + self.general["lastRun"])
            try:
                self.general["datasetDefinition"] = self.dataset.datasetSnippet(
                    jsonPath = self.general["JSON"],
                    nEvents = self.general["maxevents"],
                    firstRun = self.general["firstRun"],
                    lastRun = self.general["lastRun"],
                    begin = self.general["begin"],
                    end = self.general["end"],
                    crab = True )
            except AllInOneError, e:
                msg = "In section [%s:%s]: "%(valType, self.name)
                msg += str( e )
                raise AllInOneError( msg )
