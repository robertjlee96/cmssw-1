#ifndef FastL1CaloSim_FastL1Region_h
#define FastL1CaloSim_FastL1Region_h
// -*- C++ -*-
//
// Package:    FastL1CaloSim
// Class:      FastL1Region
// 
/**\class FastL1Region

 Description: Container class for L1 regions.

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Mon Feb 19 13:25:24 CST 2007
// $Id: FastL1Region.h,v 1.5 2007/04/25 15:43:40 chinhan Exp $
//

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "Geometry/CaloTopology/interface/CaloTowerConstituentsMap.h"

#include "FastSimulation/L1CaloTriggerProducer/interface/FastL1RegionMap.h"
//#include "FastSimulation/L1CaloTriggerProducer/interface/FastL1BitInfo.h"
//#include "FastSimulation/L1CaloTriggerProducer/interface/FastL1BitInfoFwrd.h"

struct FastL1Config {
  double JetSeedEtThreshold;
  double EMSeedEnThreshold;
 
  double EMActiveLevel; 
  double HadActiveLevel; 
  double EMNoiseLevel; 
  double HadNoiseLevel;

  double noTauVetoLevel; 
  double hOeThreshold; 
  double FGEBThreshold;
  double FGEEThreshold;
  double noFGThreshold;
  double QuietRegionThreshold;  
  double MuonNoiseLevel; 
  double CrystalEBThreshold;
  double CrystalEEThreshold;

  double TowerEMLSB;
  double TowerHadLSB;
  double JetLSB;

  double TowerEBThreshold;
  double TowerEEThreshold;
  double TowerHBThreshold;
  double TowerHEThreshold;

  double TowerEBScale;
  double TowerEEScale;
  double TowerHBScale;
  double TowerHEScale;

  std::vector<edm::InputTag> EmInputs;
  edm::InputTag TowerInput;
};

//
//
// This Class is container for region data like
// towers and algorithms for veto bits.
//
class FastL1Region {

 public:
  FastL1Region();
  ~FastL1Region();

  void SetParameters(FastL1Config);
  void FillTower(const CaloTower& t, int& tid); 
  void FillTowerZero(const CaloTower& t, int& tid); 
  void FillTower_Scaled(const CaloTower& t, int& tid); 
  void FillEMCrystals(const edm::Event&, const edm::EventSetup&,FastL1RegionMap* m_RMap);
  void Dump();
  void SetEtaPhiIndex(int eta,int phi,int ind) 
  { ieta=eta; iphi=phi; id=ind; };

  int GetiEta() { return ieta; };
  int GetiPhi() { return iphi; };
  int GetId()  { return id; };

  int GetNorthId();
  int GetSouthId(); 
  int GetWestId(); 
  int GetEastId(); 
  int GetNWId(); 
  int GetSWId(); 
  int GetNEId(); 
  int GetSEId(); 

  void SetRegionBits(edm::Event const& e);
  void SetTowerBits();
  void SetRegionEnergy();

  bool GetTauBit() { return tauBit; };
  bool GetQuietBit() { return quietBit; };
  bool GetMIPBit() { return mipBit; };
  bool GetFGBit(int i) { if(i>=0 && i<16) {return fgBit[i];} else { return false; } };
  bool GetHOEBit(int i) { if(i>=0 && i<16) { return hOeBit[i]; } else { return false; } };
  bool GetHCFGBit(int i) {  if(i>=0 && i<16) return hcfgBit[i]; else return false; };

  CaloTowerCollection GetCaloTowers() { return Towers; };
  void SetEMCrystalEnergy(int itwr, int icell, double en) { EMCrystalEnergy[itwr][icell] = en; };
  double GetEMCrystalEnergy(int itwr, int icell) { return EMCrystalEnergy[itwr][icell]; };

  double SumE() { return CalcSumE(); };
  double SumEt() { return CalcSumEt(); };
  double CalcSumE();
  double CalcSumEt();
  double GetJetE() { return jetE; };
  double GetJetEt() { return jetEt; };
  void SetJetE(double jE) { jetE = jE; };
  void SetJetEt(double jEt) { jetEt = jEt; };

  double GetJetE3x3() { return jetE3x3; };
  double GetJetEt3x3() { return jetEt3x3; };
  void SetJetE3x3(double jE) { jetE3x3 = jE; };
  void SetJetEt3x3(double jEt) { jetEt3x3 = jEt; };

  std::pair<double, double> getRegionCenterEtaPhi(const edm::EventSetup& c);

  std::pair<int, int> GetTowerNorthEtaPhi(int ieta, int iphi); 

  //FastL1BitInfo getBitInfo() { return BitInfo; }

  // public - has to bechanged!!!
  //FastL1BitInfo BitInfo;

 private:
  void SetTauBit(edm::Event const& e);
  void SetFGBit();
  void SetHOEBit();
  void SetQuietBit();
  void SetMIPBit();
  void SetHCFGBit();

  // Save Tower info
  // 4x4 matrices (rows,columns):
  // 0  1  2  3
  // 4  5  6  7
  // 8  9  10 11
  // 10 13 14 15
  CaloTowerCollection Towers;
  // Save only Crystal Energies
  double EMCrystalEnergy[16][25];

  // if region is central: jet energy is sum of 12x12 tower
  // surrounding the central region
  double jetE;
  double jetEt;
  double jetE3x3;
  double jetEt3x3;

  int id;
  int ieta;
  int iphi;

  bool tauBit;
  bool quietBit;
  bool mipBit;

  bool fgBit[16];
  bool hOeBit[16];
  bool hcfgBit[16];

  double sumEt;
  double sumE;

  FastL1Config Config;
};


double 
TPEnergyRound(double et, double Resol, double thres);

#endif
