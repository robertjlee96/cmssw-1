#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibrator.h"

#include <CLHEP/Random/RandGaussQ.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/Random.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"

/****************************************************************************
 *
 * Propagate SC calibration from Zee fit to the electrons
 *
 ****************************************************************************/

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::cout;
using namespace edm;

void ElectronEnergyCalibrator::init()
{
	if (!isMC_){

	if (verbose_) {std::cout<<"Initialization in DATA mode"<<std::endl;}

	ifstream fin(pathData_.c_str());

	if (!fin){std::cout<<"File reading error!"<<std::endl;} else{

	if (verbose_) {std::cout<<"File "<<pathData_<<" succesfully opened"<<std::endl;}

	string s;
	vector<string> selements;
	string delimiter = ",";
	nCorrValRaw = 0;	
	
	while ( !fin.eof() ) {
	
	    getline(fin, s);
	    if ( !s.empty() ) {

	        splitString(s, selements, delimiter);

		corrValArray[nCorrValRaw].nRunMin = stringToDouble(selements[0]);
		corrValArray[nCorrValRaw].nRunMax = stringToDouble(selements[1]);
		corrValArray[nCorrValRaw].corrCat0 = stringToDouble(selements[2]);
		corrValArray[nCorrValRaw].corrCat1 = stringToDouble(selements[3]);
		corrValArray[nCorrValRaw].corrCat2 = stringToDouble(selements[4]);
		corrValArray[nCorrValRaw].corrCat3 = stringToDouble(selements[5]);
		corrValArray[nCorrValRaw].corrCat4 = stringToDouble(selements[6]);
		corrValArray[nCorrValRaw].corrCat5 = stringToDouble(selements[7]);
		corrValArray[nCorrValRaw].corrCat6 = stringToDouble(selements[8]);
		corrValArray[nCorrValRaw].corrCat7 = stringToDouble(selements[9]);

		nCorrValRaw++;
	
	        selements.clear();
	    }
	}
	
	fin.close();

	if (verbose_) {std::cout<<"File closed"<<std::endl;}

	}
	} else {if (verbose_) {std::cout<<"Initializtion in MC mode"<<std::endl;}}
}

void ElectronEnergyCalibrator::splitString(const string fullstr, vector<string> &elements, const string delimiter)
{

	string::size_type lastpos = fullstr.find_first_not_of(delimiter, 0);
	string::size_type pos     = fullstr.find_first_of(delimiter, lastpos);
	
	while ( (string::npos != pos) || (string::npos != lastpos) ) {
	
	    elements.push_back(fullstr.substr(lastpos, pos-lastpos));

	    lastpos = fullstr.find_first_not_of(delimiter, pos);
	    pos = fullstr.find_first_of(delimiter, lastpos);
	}
}

double ElectronEnergyCalibrator::stringToDouble(const string &str)
{

	istringstream stm;
	double val = 0;
	
	stm.str(str);
	stm >> val;
	
	return val;
}

void ElectronEnergyCalibrator::calibrate(SimpleElectron &electron)
{
  double scale = 1.0;
  double dsigMC=0., corrMC=0.;

  double run_ = electron.getRunNumber();
  bool isEB = electron.isEB();
  double eta = electron.getEta();
  double r9 = electron.getR9();

  switch (correctionsType_){

	  case 1: if (verbose_) {std::cout<<"Using regression energy for calibration"<<std::endl;}
  		  newEnergy_ = electron.getRegEnergy();
  		  newEnergyError_ = electron.getRegEnergyError();
		  break;
	  case 2: std::cout<<"Regression type 2 corrections are not yet implemented"<<std::endl; break;
	  case 3: if (verbose_) {std::cout<<"Using standard ecal energy for calibration"<<std::endl;}
  		  newEnergy_ = electron.getSCEnergy();
  		  newEnergyError_ = electron.getSCEnergyError();
		  break;
  }

   edm::Service<edm::RandomNumberGenerator> rng;
   if ( ! rng.isAvailable()) {
     throw cms::Exception("Configuration")
       << "XXXXXXX requires the RandomNumberGeneratorService\n"
          "which is not present in the configuration file.  You must add the service\n"
          "in the configuration file or remove the modules that require it.";
   }
  
   if (!isMC_ ){
  	 for (int i=0; i < nCorrValRaw; i++)
  	 {
  	         if ((run_ > corrValArray[i].nRunMin)&&(run_ < corrValArray[i].nRunMax)){
  	      	   if (isEB) {if (fabs(eta) < 1) {if (r9<0.94) {scale = corrValArray[i].corrCat0;} else {scale = corrValArray[i].corrCat1;}} else {if (r9<0.94) {scale = corrValArray[i].corrCat2;} else {scale = corrValArray[i].corrCat3;}}} else {if (fabs(eta) < 2) {if (r9<0.94) {scale = corrValArray[i].corrCat4;} else {scale = corrValArray[i].corrCat5;}} else {if (r9<0.94) {scale = corrValArray[i].corrCat6;} else {scale = corrValArray[i].corrCat7;}}}
  	         }
  	 }
	newEnergy_ = newEnergy_*scale;
   }
  
   switch (correctionsType_){
   case 1:
   // Implementation of the MC smearing for regression energy type 1
   if (dataset_=="Summer12_DR53X_HCP2012"||dataset_=="Moriond2013") { 
    if (!isMC_){
      if (run_ <=203002) {
        if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0103;
        if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0090;
        if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0190;
        if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0156;
        if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0269;
        if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0287;
        if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0364;
        if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0321;   
      } else {
        if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0109;
        if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0099;
        if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0182;
        if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0200;
        if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0282;
        if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0309;
        if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0386;
        if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0359;   
      }
    } else {
        CLHEP::RandFlat flatRandom(rng->getEngine());	
	double rn = flatRandom.fire();
	if (rn>lumiRatio_) {
          if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0109;
          if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0099;
          if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0182;
          if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0200;
          if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0282;
          if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0309;
          if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0386;
          if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0359;  
	} else {
          if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0103;
          if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0090;
          if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0190;
          if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0156;
          if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0269;
          if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0287;
          if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0364;
          if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0321;  
	}
	if (lumiRatio_ == 0.0){
          if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0103;
          if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0090;
          if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0190;
          if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0156;
          if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0269;
          if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0287;
          if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0364;
          if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0321; 
	}
	if (lumiRatio_ == 1.0){
          if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0109;
          if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0099;
          if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0182;
          if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0200;
          if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0282;
          if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0309;
          if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0386;
          if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0359; 
	}
    }
    }
   break;

   case 2: std::cout<<"Regression type 2 corrections are not yet implemented"<<std::endl; break;
   case 3: // standard SC energy scale corrections implementation
      if (dataset_=="Summer11"||dataset_=="ReReco") { // values from https://indico.cern.ch/conferenceDisplay.py?confId=146386
      if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.01;
      if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0099;
      if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0217;
      if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0157;
      if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0326;
      if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0330;
      if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0331;
      if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0378;
    } else if (dataset_=="Fall11"||dataset_=="Jan16ReReco") { // values from https://hypernews.cern.ch/HyperNews/CMS/get/higgs2g/634.html, consistant with Jan16ReReco corrections
      if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0096;
      if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0074;
      if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0196;
      if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0141;
      if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0279;
      if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0268;
      if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0301;
      if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0293;   
    } else if (dataset_=="Summer12"||dataset_=="ICHEP2012") { 
      // new values from https://twiki.cern.ch/twiki/pub/CMS/EcalEnergyResolutionWithZee/oriented-ICHEP-scales_resolution.pdf
      if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0119;
      if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0107;
      if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0240;
      if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0149;
      if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0330;
      if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0375;
      if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0602;
      if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0607;   
    }  else if (dataset_=="Summer12_DR53X_HCP2012"||dataset_=="Moriond2013") { 
      if (isEB && fabs(eta)<1 && r9<0.94) dsigMC = 0.0099;
      if (isEB && fabs(eta)<1 && r9>=0.94) dsigMC = 0.0103;
      if (isEB && fabs(eta)>=1 && r9<0.94) dsigMC = 0.0219;
      if (isEB && fabs(eta)>=1 && r9>=0.94) dsigMC = 0.0158;
      if (!isEB && fabs(eta)<2 && r9<0.94) dsigMC = 0.0222;
      if (!isEB && fabs(eta)<2 && r9>=0.94) dsigMC = 0.0298;
      if (!isEB && fabs(eta)>=2 && r9<0.94) dsigMC = 0.0318;
      if (!isEB && fabs(eta)>=2 && r9>=0.94) dsigMC = 0.0302;   
    }	   
    break;
   }


  if (isMC_) {
    CLHEP::RandGaussQ gaussDistribution(rng->getEngine(), 1.,dsigMC);
    corrMC = gaussDistribution.fire();
    if (verbose_) std::cout << "[ElectronEnergyCalibrator] unsmeared energy " << newEnergy_ << std::endl;
    if (synchronization_) {
	    std::cout << "[ElectronEnergyCalibrator] ======================= SYNCRONIZATION MODE! ======================="<< std::endl;
	    newEnergy_ = newEnergy_*(1+dsigMC);
    } else {newEnergy_ = newEnergy_*corrMC; }
    if (verbose_) std::cout << "[ElectronEnergyCalibrator] smeared energy " << newEnergy_ << std::endl;
  }  

  // correct energy error for MC and for data as error is obtained from (ideal) MC parametrisation
  if (updateEnergyErrors_) {newEnergyError_ = sqrt(newEnergyError_*newEnergyError_ + dsigMC*dsigMC*newEnergy_*newEnergy_);}
  if (verbose_) std::cout << "[ElectronEnergyCalibrator] initial energy " << electron.getRegEnergy() << " recalibrated  energy " << newEnergy_ << std::endl;
  if (verbose_) std::cout << "[ElectronEnergyCalibrator] initial energy error " << electron.getRegEnergyError() << " recalibrated energy error " << newEnergyError_ << std::endl;

  electron.setNewEnergy(newEnergy_);
  electron.setNewEnergyError(newEnergyError_);
}
