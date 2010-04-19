#include "FWCore/Framework/interface/MakerMacros.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTProducer.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTInputProducer.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTTestAnalyzer.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTPatternTestAnalyzer.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTSaveInput.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTRelValAnalyzer.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTLutWriter.h"
//#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTGenCalibrator.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTFilter.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTPatternProducer.h"

//DEFINE_FWK_MODULE(L1RCTGenCalibrator);
DEFINE_FWK_MODULE(L1RCTProducer);
DEFINE_FWK_MODULE(L1RCTInputProducer);
DEFINE_FWK_MODULE(L1RCTTestAnalyzer);
DEFINE_FWK_MODULE(L1RCTPatternTestAnalyzer);
DEFINE_FWK_MODULE(L1RCTSaveInput);
DEFINE_FWK_MODULE(L1RCTRelValAnalyzer);
DEFINE_FWK_MODULE(L1RCTLutWriter);
DEFINE_FWK_MODULE(L1RCTFilter);
DEFINE_FWK_MODULE(L1RCTPatternProducer);

