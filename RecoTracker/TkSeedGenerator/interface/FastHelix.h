#ifndef TR_FastHelix_H_
#define TR_FastHelix_H_

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "RecoTracker/TkSeedGenerator/interface/FastCircle.h"

/**
   Generation of track parameters at a vertex using two hits and a vertex.
   It is used e.g. by a seed generator.

   24.01.2012: introduced Maxpt cut. changed algo of "FastLine" to use vertex
   21.02.2001: Old FastHelix is now called FastHelixFit. Replace FastLineFit
               by FastLine (z0, dz/drphi calculated without vertex and errors)
   14.02.2001: Replace general Circle by FastCircle.
   13.02.2001: LinearFitErrorsInTwoCoordinates replaced by FastLineFit
   29.11.2000: (Pascal Vanlaer) Modification of calculation of sign of px,py
               and change in calculation of pz, z0.
   29.11.2000: (Matthias Winkler) Split stateAtVertex() in two parts (Circle 
               is valid or not): helixStateAtVertex() and 
	                         straightLineStateAtVertex()
 */

class MagneticField;
class FastHelix {

private:
  
  typedef FreeTrajectoryState FTS;

public:


  //Original constructor (no basis vertex)
  FastHelix(const GlobalPoint& outerHit,
	    const GlobalPoint& middleHit,
	    const GlobalPoint& aVertex,
	    double nomField, MagneticField const * ibField) :
    bField(ibField),
    theOuterHit(outerHit),
    theMiddleHit(middleHit),
    theVertex(aVertex),
    theCircle(outerHit,
	      middleHit,
	      aVertex) {
    tesla0=0.1*nomField;
    maxRho = maxPt/(0.01 * 0.3*tesla0);
    useBasisVertex = false;
  }

  //New constructor (with basis vertex)
  FastHelix(const GlobalPoint& outerHit,
	    const GlobalPoint& middleHit,
	    const GlobalPoint& aVertex,
	    double nomField, MagneticField const * ibField,
	    const GlobalPoint& bVertex) : 
    bField(ibField),
    theOuterHit(outerHit),
    theMiddleHit(middleHit),
    theVertex(aVertex),
    basisVertex(bVertex),
    theCircle(outerHit,
	      middleHit,
	      aVertex) {
    tesla0=0.1*nomField;
    maxRho = maxPt/(0.01 * 0.3*tesla0);
    useBasisVertex = true;
  }

  ~FastHelix() {}
  
  bool isValid() const {return theCircle.isValid();}

  FTS stateAtVertex() const;

  FTS helixStateAtVertex() const;

  FTS straightLineStateAtVertex() const;

  const FastCircle & circle() const { return theCircle; }

private:

  static constexpr double maxPt = 10000; // 10Tev

  MagneticField const * bField; // needed to construct FTS
  GlobalPoint theOuterHit;
  GlobalPoint theMiddleHit;
  GlobalPoint theVertex;
  GlobalPoint basisVertex;
  FastCircle theCircle;
  double tesla0;
  double maxRho;
  bool useBasisVertex;
};

#endif //TR_FastHelix_H_

