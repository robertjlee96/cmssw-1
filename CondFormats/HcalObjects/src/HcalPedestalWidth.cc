/** 
\class HcalPedestalWidth
\author Fedor Ratnikov (UMd)
correlation matrix for pedestals
$Author: ratnikov
$Date: 2006/10/24 23:30:01 $
$Revision: 1.4 $
*/

#include <math.h>
#include <iostream>

#include "CondFormats/HcalObjects/interface/HcalPedestalWidth.h"

namespace {
  int offset (int fCapId1, int fCapId2) {
    static int offsets [4] = {0, 1, 3, 6};
    if (fCapId1 < fCapId2) { // swap
      int tmp = fCapId1; fCapId1 = fCapId2; fCapId2 = tmp;
    }
    return offsets [fCapId1] + fCapId2;
  }
}

HcalPedestalWidth::HcalPedestalWidth (int fId) : mId (fId) {
  for (int i = 10; --i >= 0; *(&mSigma00 + i) = 0) {}
}

float HcalPedestalWidth::getWidth (int fCapId) const {
  return sqrt (*(getValues () + offset (fCapId, fCapId)));
}

float HcalPedestalWidth::getSigma (int fCapId1, int fCapId2) const {
  return *(getValues () + offset (fCapId1, fCapId2));
}

void HcalPedestalWidth::setSigma (int fCapId1, int fCapId2, float fSigma) {
  *(&mSigma00 + offset (fCapId1, fCapId2)) = fSigma;
}

// produces pedestal noise in assumption of near correlations and small variations
void HcalPedestalWidth::makeNoise (unsigned fFrames, const double* fGauss, double* fNoise) const {
  double s_xx_mean = (getSigma (0,0) + getSigma (1,1) + getSigma (2,2) + getSigma (3,3)) / 4;
  double s_xy_mean = (getSigma (1,0) + getSigma (2,1) + getSigma (3,2) + getSigma (3,0)) / 4;
  double sigma = sqrt (0.5 * (s_xx_mean + sqrt (s_xx_mean*s_xx_mean - 2*s_xy_mean*s_xy_mean)));
  double corr = sigma == 0 ? 0 : 0.5*s_xy_mean / sigma;
  for (unsigned i = 0; i < fFrames; i++) {
    fNoise [i] = fGauss[i]*sigma;
    if (i > 0) fNoise [i] += fGauss[i-1]*corr;
    if (i < fFrames-1) fNoise [i] += fGauss[i+1]*corr;
  }
}
