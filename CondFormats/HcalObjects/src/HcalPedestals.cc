/** 
\class HcalPedestals
\author Fedor Ratnikov (UMd)
POOL object to store Pedestal values 4xCapId
$Author: ratnikov
$Date: 2005/12/15 23:38:04 $
$Revision: 1.6 $
*/

#include <iostream>

#include "CondFormats/HcalObjects/interface/HcalPedestals.h"

namespace {
  class compareItems {
  public:
    bool operator () (const HcalPedestals::Item& first, const HcalPedestals::Item& second) const {
      return first.rawId () < second.rawId ();
    }
  };

  HcalPedestals::Container::const_iterator 
  find (const HcalPedestals::Container& container, unsigned long id) {
    HcalPedestals::Container::const_iterator result = container.begin ();
    for (; result != container.end (); result++) {
      if (result->rawId () == id) break; // found
    }
    return result;
  }
}

HcalPedestals::HcalPedestals() 
  : mSorted (false) {}

HcalPedestals::~HcalPedestals(){}

const HcalPedestal* HcalPedestals::getValues (HcalDetId fId) const {
  Item target (fId.rawId (), 0, 0, 0, 0);
  std::vector<Item>::const_iterator cell;
  if (sorted ()) {
    cell = std::lower_bound (mItems.begin(), mItems.end(), target, compareItems ());
  }
  else {
    std::cerr << "HcalPedestals::getValues-> container is not sorted. Please sort it to search effectively" << std::endl;
    cell = find (mItems, fId.rawId ());
  }
  if (cell == mItems.end() || cell->rawId () != target.rawId ()) return 0;
  return &(*cell);
}

float HcalPedestals::getValue (HcalDetId fId, int fCapId) const {
  const HcalPedestal* values;
  if (fCapId >= 0 && fCapId < 4) {
    values = getValues (fId);
    if (values) return values->getValue (fCapId);
  }
  else {
    std::cerr << "HcalPedestals::getValue-> capId " << fCapId << " is out of range [0..3]" << std::endl;
  }
  return -1;
}

bool HcalPedestals::addValue (HcalDetId fId, const float fValues [4]) {
  return addValue (fId, fValues [0], fValues [1], fValues [2], fValues [3]);
}

bool HcalPedestals::addValue (HcalDetId fId, float fValue0, float fValue1, float fValue2, float fValue3) {
  Item item (fId.rawId (), fValue0, fValue1, fValue2, fValue3);
  mItems.push_back (item);
  mSorted = false;
  return true;
}

std::vector<HcalDetId> HcalPedestals::getAllChannels () const {
  std::vector<HcalDetId> result;
  for (std::vector<Item>::const_iterator item = mItems.begin (); item != mItems.end (); item++) {
    result.push_back (HcalDetId (item->rawId ()));
  }
  return result;
}


void HcalPedestals::sort () {
  if (!mSorted) {
    std::sort (mItems.begin(), mItems.end(), compareItems ());
    mSorted = true;
  }
}
