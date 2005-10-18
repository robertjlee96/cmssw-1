#ifndef FEDRawData_FEDRawDataCollection_h
#define FEDRawData_FEDRawDataCollection_h

/** \class FEDRawDataCollection
 *  An EDCollection storing the raw data for all  FEDs in a Event.
 *  
 *  Reference: DaqPrototype/DaqPersistentData/interface/DaqFedOpaqueData.h
 *
 *  $Date: 2005/10/06 18:25:22 $
 *  $Revision: 1.4 $
 *  \author N. Amapane - S. Argiro'
 */

#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <vector>


class FEDRawDataCollection  {
 public:
  FEDRawDataCollection();

  virtual ~FEDRawDataCollection();
    
  /// retrieve data for fed @param fedid
  const FEDRawData&  FEDData(int fedid) const;

  /// retrieve data for fed @param fedid
  FEDRawData&        FEDData(int fedid);

  FEDRawDataCollection(const FEDRawDataCollection &);

 private:

  std::vector<FEDRawData> data_; ///< the raw data 

};


#endif

