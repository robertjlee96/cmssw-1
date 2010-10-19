#include "TauAnalysis/FittingTools/plugins/DQMExportAnalysisResults.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TMath.h>
#include <TH1.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

const std::string systematicsDirKeyword = "#SYSTEMATICSDIR#";
const std::string channelOutputFileNameKeyword = "#CHANNEL_OUTPUTFILENAME#";
const std::string outputFileNameSeparator = "/";

mode_t mkdirAccessPermissions = (S_IRWXU | S_IRWXG | S_IRWXO);

typedef std::vector<std::string> vstring;

DQMExportAnalysisResults::DQMExportAnalysisResults(const edm::ParameterSet& cfg)
{
  std::cout << "<DQMExportAnalysisResults::DQMExportAnalysisResults>:" << std::endl;

  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");
  outputFilePath_ = cfg.getParameter<std::string>("outputFilePath");

  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin();
	processName != processNames.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);
    processEntryType* processEntry = new processEntryType(*processName, cfgProcess);
    processes_.push_back(processEntry);
  }

  edm::ParameterSet cfgChannels = cfg.getParameter<edm::ParameterSet>("channels");
  vstring channelNames = cfgChannels.getParameterNamesForType<edm::ParameterSet>();
  unsigned channelIndex = 0;
  for ( vstring::const_iterator channelName = channelNames.begin();
	channelName != channelNames.end(); ++channelName ) {
    edm::ParameterSet cfgChannel = cfgChannels.getParameter<edm::ParameterSet>(*channelName);
    channelEntryType* channelEntry = new channelEntryType(*channelName, channelIndex, cfgChannel);
    channels_.push_back(channelEntry);
    ++channelIndex;
  }

  edm::ParameterSet cfgSystematics = cfg.getParameter<edm::ParameterSet>("systematics");
  vstring systematicNames = cfgSystematics.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator systematicName = systematicNames.begin();
	systematicName != systematicNames.end(); ++systematicName ) {
    edm::ParameterSet cfgSystematic = cfgSystematics.getParameter<edm::ParameterSet>(*systematicName);
    systematicEntryType* systematicEntry = new systematicEntryType(*systematicName, cfgSystematic);
    systematics_.push_back(systematicEntry);
  }
}

DQMExportAnalysisResults::~DQMExportAnalysisResults()
{
  for ( std::vector<processEntryType*>::iterator it = processes_.begin();
	it != processes_.end(); ++it ) {
    delete (*it);
  }

  for ( std::vector<channelEntryType*>::iterator it = channels_.begin();
	it != channels_.end(); ++it ) {
    delete (*it);
  }

  for ( std::vector<systematicEntryType*>::iterator it = systematics_.begin();
	it != systematics_.end(); ++it ) {
    delete (*it);
  }
}

void createSubDirectories(const std::string& outputFileName, int& errorFlag)
{
  //std::cout << "<createSubDirectories>:" << std::endl;
  //std::cout << " outputFileName = " << outputFileName << std::endl;

  size_t index = outputFileName.find(outputFileNameSeparator, 1);
  while ( index < outputFileName.length() ) {
    std::string outputFilePath = std::string(outputFileName, 0, index);
    //std::cout << " outputFilePath = " << outputFilePath << std::endl;

    struct stat statBuffer;
    int statError = stat(outputFilePath.data(), &statBuffer);
    if ( statError != 0 ) {
      if ( errno == ENOENT ) { // directory does not exist --> create it
	std::cout << "--> directory = " << outputFilePath << " does not yet exist, creating it..." << std::endl;
	int mkdirError = mkdir(outputFilePath.data(), mkdirAccessPermissions);
	if ( mkdirError != 0 ) {
	  edm::LogError ("createSubDirectories") 
	    << " Failed to create directory = " << outputFilePath << " !!";
	  errorFlag = 1;
	}
      } else if ( errno == ENOENT ) { // directory/file marked as "bad" --> print error message
	edm::LogError ("createSubDirectories") 
	  << " Directory = " << outputFilePath << " marked as 'bad' !!";
	errorFlag = 1;
      }
    } else { // directory/file exists, check that it is indeed a directory
      if ( !(statBuffer.st_mode & S_IFDIR) ) {
	edm::LogError ("createSubDirectories") 
	  << " Invalid function argument outputFileName = " << outputFileName << ":" 
	  << " outputFilePath = " << outputFilePath << " is not a directory !!";
	errorFlag = 1;
      }
    }

    if ( errorFlag ) return;

    index = outputFileName.find(outputFileNameSeparator, index + 1);
  }
}

int getFirstBin(TH1* histogram, const std::string& axis)
{
  return 0; // return 0/1 to include/exclude underflow bin
}

int getLastBin(TH1* histogram, const std::string& axis)
{
  int numBins = 0;
  if      ( axis == "x" || axis == "X" ) numBins = histogram->GetNbinsX();
  else if ( axis == "y" || axis == "Y" ) numBins = histogram->GetNbinsY();
  else if ( axis == "z" || axis == "Z" ) numBins = histogram->GetNbinsZ();
  else {
    edm::LogError ("getLastBin")
      << " Invalid function argument axis = " << axis << " !!";
    return -1;
  }
  
  return numBins + 1; // return numBins/(numBins + 1) to include/exclude overflow bin
}

int getNumBins(TH1* histogram, const std::string& axis)
{
  return (getLastBin(histogram, axis) - getFirstBin(histogram, axis)) + 1;
}

double getSumBinContents(TH1* histogram, int firstBinX, int lastBinX, int firstBinY, int lastBinY, int firstBinZ, int lastBinZ)
{
  double sumBinContents = 0.;
  for ( int iBinX = firstBinX; iBinX <= lastBinX; ++iBinX ) {
    for ( int iBinY = firstBinY; iBinY <= lastBinY; ++iBinY ) {
      for ( int iBinZ = firstBinZ; iBinZ <= lastBinZ; ++iBinZ ) {
	double binContent = 0.;
	if      ( histogram->GetDimension() == 1 ) binContent = histogram->GetBinContent(iBinX);
	else if ( histogram->GetDimension() == 2 ) binContent = histogram->GetBinContent(iBinX, iBinY);
	else if ( histogram->GetDimension() == 3 ) binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	else assert(0);
	
	sumBinContents += binContent;
      }
    }
  }

  return sumBinContents;
}

double getSumBinErrors2(TH1* histogram, int firstBinX, int lastBinX, int firstBinY, int lastBinY, int firstBinZ, int lastBinZ)
{
  double sumBinErrors2 = 0.;
  for ( int iBinX = firstBinX; iBinX <= lastBinX; ++iBinX ) {
    for ( int iBinY = firstBinY; iBinY <= lastBinY; ++iBinY ) {
      for ( int iBinZ = firstBinZ; iBinZ <= lastBinZ; ++iBinZ ) {
	double binError = 0.;
	if      ( histogram->GetDimension() == 1 ) binError = histogram->GetBinError(iBinX);
	else if ( histogram->GetDimension() == 2 ) binError = histogram->GetBinError(iBinX, iBinY);
	else if ( histogram->GetDimension() == 3 ) binError = histogram->GetBinError(iBinX, iBinY, iBinZ);
	else assert(0);
	
	sumBinErrors2 += (binError*binError);
      }
    }
  }

  return sumBinErrors2;
}

void exportAnalysisResults(
       DQMStore& dqmStore, const std::string& meName, double numEventsTotal, unsigned numChannels, unsigned channelIndex,
       const std::string& outputFileName, bool failSilent = false)
{
  std::cout << "<exportAnalysisResults>:" << std::endl;
  std::cout << " meName = " << meName << std::endl;
  std::cout << " outputFileName = " << outputFileName << std::endl;
 
  MonitorElement* me = dqmStore.get(meName);
  TH1* histogram = ( me ) ? me->getTH1() : NULL;
  if ( histogram == NULL ) {
    if ( !failSilent ) 
      edm::LogError ("exportAnalysisResults") 
	<< " Failed to access dqmMonitorElement = " << meName << " --> analysis results will NOT be exported !!";
    return;
  }

  int numBins = getNumBins(histogram, "X");
  int firstBinX = getFirstBin(histogram, "X");
  int lastBinX = getLastBin(histogram, "X");

  int firstBinY, lastBinY;
  if ( histogram->GetDimension() >= 2 ) {
    numBins *= getNumBins(histogram, "Y");
    firstBinX = getFirstBin(histogram, "Y");
    lastBinY = getLastBin(histogram, "Y");
  } else {
    firstBinY = 1;
    lastBinY = 1;
  }

  int firstBinZ, lastBinZ;
  if ( histogram->GetDimension() >= 3 ) {
    numBins *= getNumBins(histogram, "Z");
    firstBinZ = getFirstBin(histogram, "Z");
    lastBinZ = getLastBin(histogram, "Z");
  } else {
    firstBinZ = 1;
    lastBinZ = 1;
  }

  std::vector<int> binContents(numChannels*numBins);

  int binIndex = 0;
  int binOffset = channelIndex*numBins;

  double sumBinContents = getSumBinContents(histogram, firstBinX, lastBinX, firstBinY, lastBinY, firstBinZ, lastBinZ);
  std::cout << " sumBinContents = " << sumBinContents << std::endl;

  double sumBinErrors2 = getSumBinErrors2(histogram, firstBinX, lastBinX, firstBinY, lastBinY, firstBinZ, lastBinZ);
  std::cout << " sumBinErrors2 = " << sumBinErrors2 << std::endl;

//--- scale (weighted) number of events expected for luminosity of analyzed dataset
//    to "effective" number of events Neff for which 
//      sumBinContents/sqrt(sumBinErrors2) = sqrt(Neff)
//    corresponding to number of events needed to reach same level of statistical precision
//    in case all events would have unit weight
  double scaleFactor = sumBinContents/sumBinErrors2;

  for ( int iBinX = firstBinX; iBinX <= lastBinX; ++iBinX ) {
    for ( int iBinY = firstBinY; iBinY <= lastBinY; ++iBinY ) {
      for ( int iBinZ = firstBinZ; iBinZ <= lastBinZ; ++iBinZ ) {
	double binContent = 0.;
	if      ( histogram->GetDimension() == 1 ) binContent = histogram->GetBinContent(iBinX);
	else if ( histogram->GetDimension() == 2 ) binContent = histogram->GetBinContent(iBinX, iBinY);
	else if ( histogram->GetDimension() == 3 ) binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	else assert(0);

	binContent *= scaleFactor;
	
	binContents[binIndex + binOffset] = TMath::Nint(binContent);
	++binIndex;
      }
    }
  }

  numEventsTotal *= scaleFactor;

  int errorFlag = 0;
  createSubDirectories(outputFileName, errorFlag);
  if ( errorFlag ) {
    if ( !failSilent )
      edm::LogError ("exportAnalysisResults") 
	<< " Failed to create directory structure --> analysis results will NOT be exported !!";
    return;
  }

  ostream* outputFile = new std::ofstream(outputFileName.data(), std::ios::out);

  unsigned width = 8;           // 8 characters per number, right justified
  unsigned numbersPerLine = 10; // max. 10 numbers per line

  (*outputFile) << std::setw(width) << std::setfill(' ') << numChannels;
  (*outputFile) << std::setw(width) << std::setfill(' ') << numBins;
  (*outputFile) << std::setw(width) << std::setfill(' ') << TMath::Nint(numEventsTotal);
  (*outputFile) << std::setw(width) << std::setfill(' ') << TMath::Nint(sumBinContents*scaleFactor);
  (*outputFile) << std::endl;

  for ( unsigned iBin = 0; iBin < (numChannels*numBins); ++iBin ) {
    (*outputFile) << std::setw(width) << std::setfill(' ') << binContents[iBin];
    if ( ((iBin + 1) % numbersPerLine) == 0 ) (*outputFile) << std::endl;
  }

  delete outputFile;
}

void DQMExportAnalysisResults::endJob()
{
  std::cout << "<DQMExportAnalysisResults::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be plotted !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());  

  for ( std::vector<processEntryType*>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    int errorFlag = 0;
    std::string dqmDirectory_process = replace_string(dqmDirectory_, processDirKeyword, (*process)->name_, 0, 1, errorFlag);
    std::cout << " dqmDirectory_process = " << dqmDirectory_process << std::endl;

    for ( std::vector<channelEntryType*>::iterator channel = channels_.begin();
	  channel != channels_.end(); ++channel ) {
      std::string meName_channel = dqmDirectoryName(dqmDirectory_process).append((*channel)->meName_);
      meName_channel = replace_string(meName_channel, systematicsDirKeyword, "", 0, 1, errorFlag);
      meName_channel = replace_string(meName_channel, "//", "/", 0, 1000, errorFlag);
      std::cout << " meName_channel = " << meName_channel << std::endl;
      
      std::string outputFileName_channel = std::string(outputFilePath_).append("/");
      outputFileName_channel.append((*process)->outputFilePath_).append("/");
      outputFileName_channel.append((*process)->outputFileName_);
      outputFileName_channel = 
	replace_string(outputFileName_channel, channelOutputFileNameKeyword, (*channel)->outputFileName_, 0, 1, errorFlag);
      outputFileName_channel = replace_string(outputFileName_channel, "//", "/", 0, 1000, errorFlag);
      std::cout << " outputFileName_channel = " << outputFileName_channel << std::endl;
      
//--- export "central values" 
//   (analysis results with no systematic shifts applied)
      exportAnalysisResults(dqmStore, meName_channel, (*process)->numEvents_, channels_.size(), (*channel)->index_, 
			    outputFileName_channel, false);

//--- export systematic uncertainties
      if ( (*process)->hasSysUncertainties_ ) {
	for ( std::vector<systematicEntryType*>::iterator systematic = systematics_.begin();
	      systematic != systematics_.end(); ++systematic ) {
	  std::string meName_systematic = dqmDirectoryName(dqmDirectory_process).append((*channel)->meName_);
	  meName_systematic = replace_string(meName_systematic, systematicsDirKeyword, (*systematic)->dqmDirectory_, 0, 1, errorFlag);
	  meName_systematic = replace_string(meName_systematic, "//", "/", 0, 1000, errorFlag);
	  std::cout << " meName_systematic = " << meName_systematic << std::endl;
	  
	  std::string outputFileName_systematic = std::string(outputFilePath_).append("/");
	  outputFileName_systematic.append((*process)->outputFilePath_).append("/");
	  outputFileName_systematic.append((*systematic)->outputFilePath_).append("/");
	  outputFileName_systematic.append((*process)->outputFileName_);
	  outputFileName_systematic = 
	    replace_string(outputFileName_systematic, channelOutputFileNameKeyword, (*channel)->outputFileName_, 0, 1, errorFlag);
	  outputFileName_systematic = replace_string(outputFileName_systematic, "//", "/", 0, 1000, errorFlag);
	  std::cout << " outputFileName_systematic = " << outputFileName_systematic << std::endl;
	  
	  exportAnalysisResults(dqmStore, meName_systematic, (*process)->numEvents_, channels_.size(), (*channel)->index_, 
				outputFileName_systematic, false); // true
	}
      }
    }
  }

  std::cout << "done." << std::endl;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMExportAnalysisResults);
