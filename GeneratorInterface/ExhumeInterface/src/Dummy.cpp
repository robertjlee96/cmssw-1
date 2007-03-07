//-*-C++-*-
//-*-Dummy.cpp-*-
//   Written by James Monk and Andrew Pilkington
/////////////////////////////////////////////////////////////////////////////
#include "GeneratorInterface/ExhumeInterface/interface/Dummy.h"

/////////////////////////////////////////////////////////////////////////////
Exhume::Dummy::Dummy(const edm::ParameterSet& pset) : CrossSection(pset){

  std::cout<<std::endl<<"   = Dummy Subprocess Selected ="<<std::endl;

  Inv32 = 1.0/32.0;
  Partons.resize(1);

}
/////////////////////////////////////////////////////////////////////////////
double Exhume::Dummy::SubProcess(){

  return(Inv32 * SqrtsHat);
}
/////////////////////////////////////////////////////////////////////////////
void Exhume::Dummy::SetPartons(){
  
  Partons[0].p = CentralVector;
  Partons[0].id = -1;

  return;
}
/////////////////////////////////////////////////////////////////////////////
void Exhume::Dummy::SetSubParameters(){

  return;
}
/////////////////////////////////////////////////////////////////////////////
double Exhume::Dummy::SubParameterWeight(){

  return(1.0);
}
/////////////////////////////////////////////////////////////////////////////
double Exhume::Dummy::SubParameterRange(){

  return(1.0);
}
/////////////////////////////////////////////////////////////////////////////
void Exhume::Dummy::MaximiseSubParameters(){

  return;
}
/////////////////////////////////////////////////////////////////////////////
