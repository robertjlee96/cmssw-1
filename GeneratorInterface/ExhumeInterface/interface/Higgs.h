//-*-C++-*-
//-*-Higgs.h-*-
//   Written by James Monk and Andrew Pilkington
/////////////////////////////////////////////////////////////////////////////

#ifndef HIGGS_HH
#define HIGGS_HH

#include "GeneratorInterface/ExhumeInterface/interface/CrossSection.h"

namespace Exhume{

  class Higgs : public CrossSection{

  public:
    
    Higgs(const edm::ParameterSet&);

    double SubProcess();
    void SetPartons();
    void SetSubParameters();
    double SubParameterWeight();
    void MaximiseSubParameters();
    double SubParameterRange();
    void SetHiggsMass(const double&);
    inline double GetC(){
      return(C);
    };
    inline complex<double> GetPropagator(){
      return(Propagator());
    };

    void SetHiggsDecay(const int&);

  private:

    double HiggsWidth_();

    void SetC();

    inline complex<double> Propagator(){
      //See hep-ph 9505211

      return(I * (1.0 + I * HiggsWidth/HiggsMass)/
	     (sHat - HiggsMass2 + I * HiggsWidth * sHat/HiggsMass));
    };

    inline complex<double> GluGlu2HiggsAmp(){
      return(GGHConst * sHat * AlphaS_ * 
	     (Fsf(sHat * FsfTop) + Fsf(sHat * FsfBottom)));
    };
    
    complex<double> GGHConst;
    double AlphaS_, FsfTop, FsfBottom,  NLOConst;
    double HiggsMass2, HiggsWidth, TotWidth;
    double C, One;
    double *BR;


  };

}

#endif
