#ifndef _TRACKER_GLOBALTRAJECTORYPARAMETERS_H_
#define _TRACKER_GLOBALTRAJECTORYPARAMETERS_H_

#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/Vector/interface/GlobalVector.h"
#include "Geometry/CommonDetAlgo/interface/AlgebraicObjects.h"
#include "DataFormats/TrajectoryState/interface/TrackCharge.h"

class MagneticField;

/** Class providing access to a set of relevant parameters of a trajectory
 *  in the global, Cartesian frame. The basic data members used to calculate
 *  these parameters are the charge and global position and momentum.
 */

class GlobalTrajectoryParameters {
public:
// construct
  GlobalTrajectoryParameters() {}

  /** Constructing class from global position, global momentum and charge.
   */

  GlobalTrajectoryParameters(const GlobalPoint& aX,
                             const GlobalVector& aP,
                             TrackCharge aCharge, 
			     const MagneticField* fieldProvider) :
    theX(aX), theP(aP), theCharge(aCharge), theField(fieldProvider) {}

  /** Constructing class from global position, direction (unit length) 
   *  and transverse curvature. The fourth int argument is dummy, 
   *  it serves only to distinguish
   *  this constructor from the one above.
   */

  GlobalTrajectoryParameters(const GlobalPoint& aX,
                             const GlobalVector& direction,
                             double transverseCurvature, int, 
			     const MagneticField* fieldProvider);

  /** Global position.
   */
  
  GlobalPoint position() const {
    return theX;
  }

  /** Global momentum.
   */
  
  GlobalVector momentum() const {
    return theP;
  }

  /** Charge q of particle, either +1 or -1.
   */

  TrackCharge charge() const {
    return theCharge;
  }

  /** Charge divided by (magnitude of) momentum, i.e. q/p.
   */  

  double signedInverseMomentum() const {
    return theCharge/theP.mag();
  }

  /** Charge divided by transverse momentum, i.e. q/p_T.
   */ 

  double signedInverseTransverseMomentum() const {
    return theCharge/theP.perp();
  }

  /** Transverse curvature kappa (which is the inverse radius of curvature in the transverse plane) 
   *  in cm^{-1}. Sign convention is such that positive kappa means
   *  counterclockwise rotation of the track with respect to the global z-axis.
   */

  double transverseCurvature() const;

  /** Vector whose first three elements are the global position coordinates and
   *  whose last three elements are the global momentum coordinates.
   */

  AlgebraicVector vector() const {
    AlgebraicVector v(6);
    v[0] = theX.x();
    v[1] = theX.y();
    v[2] = theX.z();
    v[3] = theP.x();
    v[4] = theP.y();
    v[5] = theP.z();
    return v;
  }

  GlobalVector megneticFieldInInverseGeV( const GlobalPoint& x) const; 
  const MagneticField& magneticField() const {return *theField;}

private:
  GlobalPoint theX;
  GlobalVector theP;
  TrackCharge theCharge;
  const MagneticField* theField;
};

#endif
