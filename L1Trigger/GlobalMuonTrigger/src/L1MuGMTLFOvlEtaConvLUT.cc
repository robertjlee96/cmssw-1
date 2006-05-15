//-------------------------------------------------
//
//   Class: L1MuGMTLFOvlEtaConvLUT
//
// 
//   $Date: 2004/11/24 14:05:29 $
//   $Revision: 1.1 $
//
//   Author :
//   H. Sakulin            HEPHY Vienna
//
//   Migrated to CMSSW:
//   I. Mikulec
//
//--------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTLFOvlEtaConvLUT.h"

//---------------
// C++ Headers --
//---------------

//#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTScales.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuTriggerScales.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuPacking.h"
#include "SimG4Core/Notification/interface/Singleton.h"

//-------------------
// InitParameters  --
//-------------------

void L1MuGMTLFOvlEtaConvLUT::InitParameters() {
  m_theGMTScales = Singleton<L1MuGMTScales>::instance();
  m_theTriggerScales = Singleton<L1MuTriggerScales>::instance();
};


//--------------------------------------------------------------------------------
// Overlap eta conversion LUT
// 
// convert global eta to a 4-bit pseudo-signed eta in the overlap region to be used in
// the COU matching units
//
// instances:
// ----------
//   barrel chip: DT, bRPC
//   barrel chip : ovlCSC 
//
//   forward chip: CSC bRPC
//   forward chip: ovlDT
//
//--------------------------------------------------------------------------------

unsigned L1MuGMTLFOvlEtaConvLUT::TheLookupFunction (int idx, unsigned eta6) const {
  // idx is DT, CSC, bRPC, fRPC, ovlCSC, ovlDT
  // INPUTS:  eta6(6)
  // OUTPUTS: eta_ovl(4) 

  int idx_drcr = 0;

  switch (idx) {
  case DT     : idx_drcr = 0; break;
  case CSC    : idx_drcr = 2; break;
  case bRPC   : idx_drcr = 1; break;
  case fRPC   : idx_drcr = 3; break;
  case ovlCSC : idx_drcr = 2; break;
  case ovlDT  : idx_drcr = 0; break;
  }

  float etaValue = m_theTriggerScales->getRegionalEtaScale(idx_drcr)->getCenter( eta6 );

  unsigned eta4bit = 0;
  if (fabs(etaValue) <  m_theGMTScales->getOvlEtaScale(idx_drcr)->getScaleMin() || 
      fabs(etaValue) >  m_theGMTScales->getOvlEtaScale(idx_drcr)->getScaleMax() ) {
    eta4bit = 7; // out of range code is max pos value
  }

  else {
    eta4bit  = m_theGMTScales->getOvlEtaScale(idx_drcr)->getPacked( etaValue );
    //    cout << "etaValue  = " << etaValue << "   eta OVERLAP= " << eta4bit << endl;
  }

  return eta4bit;
}; 












