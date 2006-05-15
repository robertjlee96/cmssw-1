//-------------------------------------------------
//
/** \class L1MuGMTMIAUPhiPro1LUT
 *
 *   MIAUPhiPro1 look-up table
 *          
 *   this class was automatically generated by 
 *     L1MuGMTLUT::MakeSubClass()  
*/ 
//   $Date: 2004/02/03 16:33:44 $
//   $Revision: 1.2 $
//
//   Author :
//   H. Sakulin            HEPHY Vienna
//
//   Migrated to CMSSW:
//   I. Mikulec
//
//--------------------------------------------------
#ifndef L1TriggerGlobalMuonTrigger_L1MuGMTMIAUPhiPro1LUT_h
#define L1TriggerGlobalMuonTrigger_L1MuGMTMIAUPhiPro1LUT_h

//---------------
// C++ Headers --
//---------------


//----------------------
// Base Class Headers --
//----------------------
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTLUT.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class L1MuTriggerScales;
//class L1MuGMTScales;


//              ---------------------
//              -- Class Interface --
//              ---------------------

using namespace std;

class L1MuGMTMIAUPhiPro1LUT : public L1MuGMTLUT {
  
 public:
  enum {MIP_DT, MIP_BRPC, ISO_DT, ISO_BRPC, MIP_CSC, MIP_FRPC, ISO_CSC, ISO_FRPC};

  /// constuctor using function-lookup
  L1MuGMTMIAUPhiPro1LUT() : L1MuGMTLUT("MIAUPhiPro1", 
				       "MIP_DT MIP_BRPC ISO_DT ISO_BRPC MIP_CSC MIP_FRPC ISO_CSC ISO_FRPC",
				       "phi_fine(3) eta(4) pt(5) charge(1)",
				       "cphi_fine(1) cphi_ofs(3)", 11, false) {
    InitParameters();
  } ;

  /// destructor
  virtual ~L1MuGMTMIAUPhiPro1LUT() {};

  /// specific lookup function for cphi_fine
  unsigned SpecificLookup_cphi_fine (int idx, unsigned phi_fine, unsigned eta, unsigned pt, unsigned charge) const {
    vector<unsigned> addr(4);
    addr[0] = phi_fine;
    addr[1] = eta;
    addr[2] = pt;
    addr[3] = charge;
    return Lookup(idx, addr) [0];
  };

  /// specific lookup function for cphi_ofs
  unsigned SpecificLookup_cphi_ofs (int idx, unsigned phi_fine, unsigned eta, unsigned pt, unsigned charge) const {
    vector<unsigned> addr(4);
    addr[0] = phi_fine;
    addr[1] = eta;
    addr[2] = pt;
    addr[3] = charge;
    return Lookup(idx, addr) [1];
  };

  /// specific lookup function for entire output field
  unsigned SpecificLookup (int idx, unsigned phi_fine, unsigned eta, unsigned pt, unsigned charge) const {
    vector<unsigned> addr(4);
    addr[0] = phi_fine;
    addr[1] = eta;
    addr[2] = pt;
    addr[3] = charge;
    return LookupPacked(idx, addr);
  };



  /// access to lookup function with packed input and output

  virtual unsigned LookupFunctionPacked (int idx, unsigned address) const {
    vector<unsigned> addr = u2vec(address, m_Inputs);
    return TheLookupFunction(idx ,addr[0] ,addr[1] ,addr[2] ,addr[3]);

  };

 private:
  /// Initialize scales, configuration parameters, alignment constants, ...
  void InitParameters();

  /// The lookup function - here the functionality of the LUT is implemented
  unsigned TheLookupFunction (int idx, unsigned phi_fine, unsigned eta, unsigned pt, unsigned charge) const;

  /// Private data members (LUT parameters);
  L1MuTriggerScales *m_theTriggerScales; // pointer to L1MuTriggerScales Singleton
  //  L1MuGMTScales *m_theGMTScales; // pointer to L1MuGMTScales Singleton
  float m_calo_align; // angle between nominal phi=0 and start of calo region 0
};
#endif



















