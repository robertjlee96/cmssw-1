#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DQMOffline/JetMET/interface/JetMETAnalyzer.h"
#include "DQMOffline/JetMET/interface/CaloTowerAnalyzer.h"
#include "DQMOffline/JetMET/interface/ECALRecHitAnalyzer.h"
#include "DQMOffline/JetMET/interface/HCALRecHitAnalyzer.h"
#include "DQMOffline/JetMET/interface/BeamHaloAnalyzer.h"
#include "DQMOffline/JetMET/interface/DataCertificationJetMET.h"
#include "DQMOffline/JetMET/interface/JetMETDQMOfflineClient.h"


DEFINE_FWK_MODULE(JetMETAnalyzer);
DEFINE_FWK_MODULE(CaloTowerAnalyzer);
DEFINE_FWK_MODULE(HCALRecHitAnalyzer);
DEFINE_FWK_MODULE(ECALRecHitAnalyzer);
DEFINE_FWK_MODULE(BeamHaloAnalyzer);
DEFINE_FWK_MODULE(DataCertificationJetMET);
DEFINE_FWK_MODULE(JetMETDQMOfflineClient);

