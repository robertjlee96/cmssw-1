#ifndef ThirdHitPredictionFromInvParabola_H
#define ThirdHitPredictionFromInvParabola_H

/** Check phi compatibility of a hit with a track
    constrained by a hit pair and TrackingRegion kinematical constraints.
    The "inverse parabola method" is used. 
    M.Hansroul, H.Jeremie, D.Savard NIM A270 (1998) 490. 
 */
    
  

#include "Geometry/Vector/interface/Basic2DVector.h"
#include "Geometry/Vector/interface/Basic3DVector.h"
#include "Geometry/Surface/interface/TkRotation.h"


class TrackingRegion;
class OrderedHitPair;
#include "Geometry/Vector/interface/GlobalPoint.h"
//#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"


class ThirdHitPredictionFromInvParabola {

public:

  typedef TkRotation<double> Rotation;

//  ThirdHitPredictionFromInvParabola(const OrderedHitPair & hitPair);
//  double isCompatible(const TrackingRecHit & hit, const TrackingRegion & region) const;
  ThirdHitPredictionFromInvParabola(const GlobalPoint & P1, const GlobalPoint & P2);
  double isCompatible(const GlobalPoint& hit, double ip, double curv) const;

  void init( const GlobalPoint & P1, const GlobalPoint & P2);
private:

  double test( const GlobalPoint & P3, const double & ip) const;

  double coeffA(const double & impactParameter) const;
  double coeffB(const double & impactParameter) const;
  double predV(const double & u, const double & ip) const;
  double ipFromCurvature(const double & curvature) const;

private:

  template <class T> class MappedPoint {
  public:
    MappedPoint() : theU(0), theV(0), pRot(0) { }
    MappedPoint(const T & aU, const T & aV, const TkRotation<T> * aRot) 
        : theU(aU), theV(aV), pRot(aRot) { }
    MappedPoint(const Basic2DVector<T> & point, const TkRotation<T> * aRot)
        : pRot(aRot) {
      T radius2 = point.mag2();
      Basic3DVector<T> rotated = (*pRot) * point;
      theU = rotated.x() / radius2;
      theV = rotated.y() / radius2;
    }
    T u() const {return theU; } 
    T v() const {return theV; }
    Basic2DVector<T> unmap () const {
       T invRadius2 = theU*theU+theV*theV; 
       Basic3DVector<T> tmp
           = (*pRot).multiplyInverse(Basic2DVector<T>(theU,theV));
       return Basic2DVector<T>( tmp.x()/invRadius2, tmp.y()/invRadius2);
    }
  private:
    T theU, theV;
    const TkRotation<T> * pRot;
  };

private:

  Rotation theRotation;
  typedef MappedPoint<double> PointUV;
  PointUV p1, p2;
};

#endif
