#ifndef L1CALOREGION_H
#define L1CALOREGION_H

#include <boost/cstdint.hpp>
#include <ostream>

#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"

/*!
 * \author Jim Brooke
 * \date May 2006
 */

/*!
 * \class L1CaloRegion
 * \brief A calorimeter trigger region (sum of 4x4 trigger towers)
 *
 * 
 *
 */


class L1CaloRegion
{
public:

  /// default constructor
  L1CaloRegion();

  /// constructor for emulation : HB/HE regions (for RCT emulator)
  L1CaloRegion(unsigned et, bool overFlow, bool tauVeto, bool mip, bool quiet, unsigned crate, unsigned card, unsigned rgn);

  /// constructor for emulation : HF regions (for RCT emulator)
  L1CaloRegion(unsigned et, bool overFlow, bool fineGrain, unsigned crate, unsigned rgn);

  /// construct with global eta,phi indices (for testing GCT emulator)
  L1CaloRegion(unsigned et, bool overFlow, bool tauVeto, bool mip, bool quiet, int eta, int phi);

  /// constructor from raw data & position (for unpacking)
  L1CaloRegion(uint16_t data, int eta, int phi);

  /// destructor
  ~L1CaloRegion();
  

  // get/set methods for the data

  /// get Et
  unsigned et() const { return (m_data & 0x3ff); }

  /// get overflow
  bool overFlow() const { return ((m_data>>10) & 0x1)!=0; }

  /// get tau veto bit
  bool tauVeto() const { return ((m_data>>11) & 0x1)!=0; }

  /// get MIP bit
  bool mip() const { return ((m_data>>12) & 0x1)!=0; }

  /// get quiet bit
  bool quiet() const { return ((m_data>>13) & 0x1)!=0; }

  /// set MIP bit (required for GCT emulator standalone operation)
  void setMip(bool mip);

  /// set quiet bit (required for GCT emulator standalone operation)
  void setQuiet(bool quiet);

  
  // get methods for the geographical information

  /// get global region ID
  L1CaloRegionDetId id() const { return m_id; }

  /// get RCT crate ID
  unsigned rctCrate() const { return m_id.rctCrate(); }

  /// get RCT reciever card ID (valid output for HB/HE)
  unsigned rctCard() const { return m_id.rctCard(); }

  /// get RCT region index
  unsigned rctRegionIndex() const { return m_id.rctRegion(); }

  /// get local eta index (within RCT crate)
  unsigned rctEtaIndex() const { return m_id.rctEta(); }

  /// get local phi index (within RCT crate)
  unsigned rctPhiIndex() const { return m_id.rctPhi(); } 

  /// get GCT source card ID
  unsigned gctCard() const { return 0; }

  /// get GCT eta index (global)
  unsigned gctEtaIndex() const { return m_id.ieta(); }

  /// get GCT phi index (global)
  unsigned gctPhiIndex() const { return m_id.iphi(); }

  /// get pseudorapidity
  float pseudorapidity() const { return 0.; }

  /// get phi in radians
  float phi() const { return 0.; }


  /// print to stream
  friend std::ostream& operator << (std::ostream& os, const L1CaloRegion& reg);

 private:

  /// region id
  L1CaloRegionDetId m_id;

  /// region data : et, overflow, tau veto, mip and quiet bits
  uint16_t m_data;

};


#endif /*L1GCTREGION_H_*/
