#include "SimCalorimetry/HcalTestBeam/interface/HcalTBSimParameterMap.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
  

HcalTBSimParameterMap::HcalTBSimParameterMap() :
  theHBParameters(2000., std::vector<double>(16, 0.3305), 117, 5, 10, 5, 
		  true, false, 1, std::vector<double>(16, 117.)),
  theHEParameters(2000., std::vector<double>(16, 0.3305), 178, 5, 10, 5, 
		  true, false, 16, std::vector<double>(16, 178.)),
  theHOParameters(4000., std::vector<double>(16, 0.3065), 217, 5, 10, 5, 
		  true, false, 1, std::vector<double>(16, 217.)) {}

/*
  CaloSimParameters(double photomultiplierGain, double amplifierGain,
                 double samplingFactor, double timePhase,
                 int readoutFrameSize, int binOfMaximum,
                 bool doPhotostatistics)
*/

HcalTBSimParameterMap::HcalTBSimParameterMap(const edm::ParameterSet & p) :
  theHBParameters(p.getUntrackedParameter<double>("photomultiplierGainTBHB",2000.),
		  p.getUntrackedParameter<std::vector<double> >("photoelectronsToAnalogTBHB",std::vector<double>(16, 0.3305)),
		  p.getUntrackedParameter<double>("samplingFactorTBHB",117),
		  p.getUntrackedParameter<double>("timePhaseTBHB",5),
		  p.getUntrackedParameter<int>("readoutFrameSizeTB",10),
		  p.getUntrackedParameter<int>("binOfMaximumTBHB",5),
		  p.getUntrackedParameter<bool>("doPhotostatisticsTB",true),
		  p.getUntrackedParameter<bool>("syncPhaseTB",true),
		  p.getUntrackedParameter<int>("firstRingTBHB",1), 
                  p.getUntrackedParameter<std::vector<double> >("samplingFactorsTBHB", std::vector<double>(16, 117.)) ),
  theHEParameters(p.getUntrackedParameter<double>("photomultiplierGainTBHE",2000.),
		  p.getUntrackedParameter<std::vector<double> >("photoelectronsToAnalogTBHE",std::vector<double>(16, 0.3305)),
		  p.getUntrackedParameter<double>("samplingFactorTBHE",178),
		  p.getUntrackedParameter<double>("timePhaseTBHE",5),
		  p.getUntrackedParameter<int>("readoutFrameSizeTB",10),
		  p.getUntrackedParameter<int>("binOfMaximumTBHE",5),
		  p.getUntrackedParameter<bool>("doPhotostatisticsTB",true),
		  p.getUntrackedParameter<bool>("syncPhaseTB",true),
		  p.getUntrackedParameter<int>("firstRingTBHE",16), 
                  p.getUntrackedParameter<std::vector<double> >("samplingFactorsTBHE", std::vector<double>(16, 178.)) ),
  theHOParameters(p.getUntrackedParameter<double>("photomultiplierGainTBHE",4000.),
		  p.getUntrackedParameter<std::vector<double> >("photoelectronsToAnalogTBHO",std::vector<double>(16, 3065)),
		  p.getUntrackedParameter<double>("samplingFactorTBHO",217),
		  p.getUntrackedParameter<double>("timePhaseTBHO",5),
		  p.getUntrackedParameter<int>("readoutFrameSizeTB",10),
		  p.getUntrackedParameter<int>("binOfMaximumTBHO",5),
		  p.getUntrackedParameter<bool>("doPhotostatisticsTB",true),
		  p.getUntrackedParameter<bool>("syncPhaseTB",true),
		  p.getUntrackedParameter<int>("firstRingTBHO",1), 
                  p.getUntrackedParameter<std::vector<double> >("samplingFactorsTBHO", std::vector<double>(16, 217.)) ) {}


const CaloSimParameters & HcalTBSimParameterMap::simParameters(const DetId & detId) const {
  HcalDetId hcalDetId(detId);
  if(hcalDetId.subdet() == HcalBarrel) {
    return theHBParameters;
  } else if(hcalDetId.subdet() == HcalEndcap) {
    return theHEParameters;
  } else {
    return theHOParameters;
  }
}

