#ifndef HcalPedestal_h
#define HcalPedestal_h

/** 
\class HcalPedestal
\author Fedor Ratnikov (UMd)
POOL object to store Pedestal values 4xCapId
$Author: ratnikov
$Date: 2005/12/15 23:38:04 $
$Revision: 1.1 $
*/

class HcalPedestal {
 public:
  /// get value for all capId = 0..3
  const float* getValues () const {return &mValue0;}
  /// get value for capId = 0..3
  float getValue (int fCapId) const {return *(getValues () + fCapId);}

  // functions below are not supposed to be used by consumer applications

  HcalPedestal () : mId (0), mValue0 (0), mValue1 (0), mValue2 (0), mValue3 (0) {}
  
  HcalPedestal (unsigned long fId, float fCap0, float fCap1, float fCap2, float fCap3) :
    mId (fId),
    mValue0 (fCap0),
    mValue1 (fCap1),
    mValue2 (fCap2),
    mValue3 (fCap3) {}

  unsigned long rawId () const {return mId;}

 private:
  unsigned long mId;
  float mValue0;
  float mValue1;
  float mValue2;
  float mValue3;
};

#endif
