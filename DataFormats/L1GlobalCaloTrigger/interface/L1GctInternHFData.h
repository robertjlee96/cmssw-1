#ifndef L1GCTINTERNHFDATA_H
#define L1GCTINTERNHFDATA_H

#include <ostream>
#include <string>


/// \class L1GctInternHFData
/// \brief L1 GCT internal ring sums and/or bit counts
/// \author Jim Brooke
/// \date June 2008
///
/// Will store 4 sums/counts of up to 8 bits each
/// 


class L1GctInternHFData {

 public:

  /// et sum type - not clear this is required
  enum L1GctInternHFDataType { null,
			       conc_hf_ring_et_sums,
			       conc_hf_bit_counts
  };
  
  /// default constructor (for vector initialisation etc.)
  L1GctInternHFData();

  /// destructor
  ~L1GctInternHFData();

  static L1GctInternHFData fromConcRingSums(const uint16_t capBlock,
					    const uint16_t capIndex,
					    const uint8_t bx,
					    const uint32_t data);
  
  static L1GctInternHFData fromConcBitCounts(const uint16_t capBlock,
					     const uint16_t capIndex,
					     const uint8_t bx,
					     const uint32_t data);
  
  /// metadata

  /// 'type' of object - not required?
  L1GctInternHFData::L1GctInternHFDataType type() const { return type_; }

  /// get capture block
  uint16_t capBlock() const { return capBlock_; }

  /// get index within capture block
  uint16_t capIndex() const { return capIndex_; }

  /// get BX number
  int16_t bx() const { return bx_; }

  /// is the sum non-zero
  bool empty() const { return (data_ == 0); }


  /// get the actual data

  /// is this ring sums or bit counts?
  bool isRingSums() const { return (type_ == conc_hf_ring_et_sums); }

  /// get the raw data
  uint32_t raw() const { return data_; }
  
  /// get the et sums
  uint16_t et(unsigned const i);

  /// get the counts
  uint16_t count(unsigned const i);
  

  /// operators

  /// equality operator
  bool operator==(const L1GctInternHFData& c) const;
  
  /// inequality operator
  bool operator!=(const L1GctInternHFData& c) const { return !(*this == c); }
  
  // private methods
 private:
  
  /// set cap block
  void setCapBlock(uint16_t capBlock) { capBlock_ = capBlock; }

  /// set cap index
  void setCapIndex(uint16_t capIndex) { capIndex_ = capIndex; }

  /// set bx
  void setBx(uint16_t bx) { bx_ = bx; }

  /// set type
  void setType(L1GctInternHFDataType type) { type_ = type; }

  /// set the sum
  void setEt(unsigned i, uint16_t et);

  /// set the count
  void setCount(unsigned i, uint16_t count);

  void setData(uint32_t data) { data_ = data; }


  // private data
 private:

  // type of data
  L1GctInternHFDataType type_;

  // source of the data
  uint16_t capBlock_;
  uint16_t capIndex_;
  int16_t bx_;

  // the captured data
  uint32_t data_;

 };

std::ostream& operator<<(std::ostream& s, const L1GctInternHFData& cand);

#endif
