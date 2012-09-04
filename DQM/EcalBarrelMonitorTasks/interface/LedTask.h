#ifndef LedTask_H
#define LedTask_H

#include "DQWorkerTask.h"

#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

namespace ecaldqm {

  class LedTask : public DQWorkerTask {
  public:
    LedTask(edm::ParameterSet const&, edm::ParameterSet const&);
    ~LedTask() {}

    void setDependencies(DependencySet&);

    bool filterRunType(const std::vector<short>&);

    void beginEvent(const edm::Event &, const edm::EventSetup &);

    void analyze(const void*, Collections);

    void runOnRawData(EcalRawDataCollection const&);
    void runOnDigis(const EcalDigiCollection&);
    void runOnPnDigis(const EcalPnDiodeDigiCollection&);
    void runOnUncalibRecHits(const EcalUncalibratedRecHitCollection&);

    enum MESets {
      kAmplitudeSummary,
      kAmplitude,
      kOccupancy,
      kShape,
      kTiming,
      kAOverP,
      kPNAmplitude,
      nMESets
    };

    static void setMEOrdering(std::map<std::string, unsigned>&);

  private:
    std::map<int, unsigned> wlToME_;

    bool enable_[BinService::nEEDCC];
    unsigned wavelength_[BinService::nEEDCC];
    unsigned rtHalf_[BinService::nEEDCC];
    std::map<unsigned, float> pnAmp_;
  };

  inline void LedTask::analyze(const void* _p, Collections _collection){
    switch(_collection){
    case kEcalRawData:
      runOnRawData(*static_cast<EcalRawDataCollection const*>(_p));
      break;
    case kEEDigi:
      runOnDigis(*static_cast<const EcalDigiCollection*>(_p));
      break;
    case kPnDiodeDigi:
      runOnPnDigis(*static_cast<const EcalPnDiodeDigiCollection*>(_p));
      break;
    case kEELaserLedUncalibRecHit:
      runOnUncalibRecHits(*static_cast<const EcalUncalibratedRecHitCollection*>(_p));
      break;
    default:
      break;
    }
  }

}

#endif
