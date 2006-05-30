#include "Geometry/EcalTestBeam/interface/EcalTBHodoscopeGeometry.h"

#include <vector>
#include <iostream>

using namespace std;

int main() {

  EcalTBHodoscopeGeometry theTestGeom;
  
  for ( int j = 0 ; j < theTestGeom.getNPlanes() ; ++j ) 
    {
      for ( int i = 0 ; i < 1000 ; ++i ) 
	{
	  std::cout << "Position " << -17.+ 34./1000.*i << " Plane " << j << std::endl;
	  std::vector<int> firedFibres=theTestGeom.getFiredFibresInPlane(-17.+ 34./1000.*i,j);
	  for (unsigned int k=0; k < firedFibres.size() ; k++)
	    std::cout << firedFibres[k] << std::endl;
	}
    }


  return 0;

}
