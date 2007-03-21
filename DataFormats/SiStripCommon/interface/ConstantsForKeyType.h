// Last commit: $Id: $

#ifndef DataFormats_SiStripCommon_ConstantsForKeyType_H
#define DataFormats_SiStripCommon_ConstantsForKeyType_H

#include "DataFormats/SiStripCommon/interface/Constants.h"
#include <string>

/** 
    @file ConstantsForKeyType.h 

    @brief Constants and enumerated type for sistrip::KeyType
*/

namespace sistrip { 
  
  // ---------- Constants ---------- 

  static const std::string unknownKey_   = "UnknownKey";
  static const std::string undefinedKey_ = "UndefinedKey";

  static const std::string fedKey_ = "FedKey";
  static const std::string fecKey_ = "FecKey";
  static const std::string detKey_ = "DetKey";

  // ---------- Enumerated type ---------- 

  enum KeyType { UNKNOWN_KEY   = sistrip::unknown_,  
		 UNDEFINED_KEY = sistrip::invalid_,  
		 FED_KEY       = 1, 
		 FEC_KEY       = 2, 
		 DET_KEY       = 3 
  };
  
}

#endif // DataFormats_SiStripCommon_ConstantsForKeyType_H


