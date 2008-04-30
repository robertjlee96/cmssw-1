#ifndef CALIBELECTRON_H
#define CALIBELECTRON_H

#include <TROOT.h>
#include <TLorentzVector.h>

#include <vector>
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"



namespace calib
{
  class CalibElectron {
    
  public:
    
    CalibElectron();
    CalibElectron(const reco::PixelMatchGsfElectron* ele ,const EcalRecHitCollection* theHits, const EcalRecHitCollection* theEEHits) : 
      theElectron_(ele),
      theHits_(theHits), 
      theEEHits_(theEEHits) 
      {
      };

    ~CalibElectron() {};


    std::vector< std::pair<int,float> > getCalibModulesWeights(TString calibtype);
    const reco::PixelMatchGsfElectron* getRecoElectron() { return theElectron_; }
    const EcalRecHitCollection* getRecHits() { return theHits_; }
    const EcalRecHitCollection* getEERecHits() { return theEEHits_; }

  private:
  
    const reco::PixelMatchGsfElectron* theElectron_;
    
    const EcalRecHitCollection* theHits_;
    const EcalRecHitCollection* theEEHits_;

  };
}
#endif

