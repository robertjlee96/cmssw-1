#ifndef INCLUSIVE_JET_TREE_PRODUCER_H
#define INCLUSIVE_JET_TREE_PRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TH1.h" 
#include "TNamed.h"
#include <vector>
#include <string>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"

//Hcal Noise Objects
#include "RecoMET/METAlgorithms/interface/HcalNoiseRBXArray.h"
#include "DataFormats/METReco/interface/HcalNoiseHPD.h"

//TFile Service 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

class InclusiveJetTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit InclusiveJetTreeProducer(edm::ParameterSet const& cfg);
    virtual void beginJob(edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~InclusiveJetTreeProducer();

  private:
    
    void buildTree();
    void clearTreeVectors();
    
    bool mIsMCarlo;
    std::string mJetsName;
    std::string mJetsIDName;
    std::string mJetExtender;
    std::string mMetName;
    std::string mMetNoHFName;
    std::string mTriggerProcessName;
    std::vector<std::string> mTriggerNames;
    std::vector<std::string> mL1TriggerNames;
    std::vector<unsigned int> mTriggerIndex;
    edm::InputTag mHcalNoiseTag;
    edm::InputTag mTriggerResultsTag, mL1GTReadoutRcdSource, mL1GTObjectMapRcdSource;    
    HLTConfigProvider mHltConfig;     

    edm::Service<TFileService> fs;                                                                                                                        
    TTree *mTree;

    std::vector<int>    *mNtrkVtx,*mNtrkCalo,*mN90;
    std::vector<float> *mE,*mPt,*mEta,*mEtaD,*mPhi,*mY,*mEmf;
    std::vector<float> *mTrkCaloPt,*mTrkCaloEta,*mTrkCaloPhi;
    std::vector<float> *mTrkVtxPt,*mTrkVtxEta,*mTrkVtxPhi;
    std::vector<float> *mfHPD,*mfRBX,*mEtaMoment,*mPhiMoment;
    std::vector<float> *mPVx,*mPVy,*mPVz;
    std::vector<float> *mfHcalNoise;
    std::vector<std::string> *mHLTNames;
    std::vector<std::string> *mL1Names;
    
    float mMET, mMETnoHF, mSumET, mSumETnoHF, mPtHat, mWeight;
    int mRunNo, mEvtNo, mLumi;
};
#endif
