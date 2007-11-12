#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DumpDBToFile.h"
#include "DumpFileToDB.h"
#include "DTT0Analyzer.h"
#include "DTTTrigAnalyzer.h"
#include "DTVDriftAnalyzer.h"
#include "ProduceFakeDB.h"
#include "ShiftTTrigDB.h"
#include "ToChamberRefT0DB.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(DumpDBToFile);
DEFINE_ANOTHER_FWK_MODULE(DumpFileToDB);
DEFINE_ANOTHER_FWK_MODULE(DTT0Analyzer);
DEFINE_ANOTHER_FWK_MODULE(DTTTrigAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(DTVDriftAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(ProduceFakeDB);
DEFINE_ANOTHER_FWK_MODULE(ShiftTTrigDB);
DEFINE_ANOTHER_FWK_MODULE(ToChamberRefT0DB);


