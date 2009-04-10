#ifndef CommonDet_Trajectory_H
#define CommonDet_Trajectory_H

#include "DataFormats/Common/interface/RefToBase.h"
#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"


#include <vector>
#include <algorithm>

/** A class for detailed particle trajectory representation.
 *  It is used during trajectory building to "grow" a trajectory.
 *  The trajectory is represented as an ordered sequence of 
 *  TrajectoryMeasurement objects with a stack-like interface.
 *  The measurements are added to the Trajectory in the order of
 *  increasing precision: each new TrajectoryMeasurement is assumed to improve
 *  the precision of the last one, normally by adding a constraint from 
 *  a new RecHit.
 *     However the Trajectory class does not have the means to verify
 *  that measurements are added in the correct order, and thus cannot
 *  guarantee the order, which is the responsibility of the 
 *  TrajectoryBuilder. The Trajectory provides some security by
 *  allowing to add or remove measurements only on one of it's ends,
 *  with push(TM) and pop() methods. The last measurement in a Trajectory
 *  can thus be either the innermost (closest to the interaction point)
 *  or the outermost, depending on the way the Trajectory was built.
 *  The direction of building is represented as a PropagationDirection,
 *  which has two possible values: alongMomentum (outwards) and
 *  oppositeToMomentum (inwards), and is accessed with the direction()
 *  method.
 */


class Trajectory
{
public:

  typedef std::vector<TrajectoryMeasurement>                   DataContainer;
  typedef TransientTrackingRecHit::ConstRecHitContainer        ConstRecHitContainer;
  typedef ConstRecHitContainer                                 RecHitContainer;


  /** Default constructor of an empty trajectory with undefined seed and 
   * undefined direction. This constructor is necessary in order to transiently
   * copy vector<Trajectory> in the edm::Event
   */
  
  Trajectory() :  theChiSquared(0), theValid(true),
    theNumberOfFoundHits(0), theNumberOfLostHits(0),
    theDirection(alongMomentum), theDirectionValidity(false),
    theSeed(TrajectorySeed()), seedRef_()  
    {}


  /** Constructor of an empty trajectory with undefined direction.
   *  The direction will be defined at the moment of the push of a second
   *  measurement, from the relative radii of the first and second 
   *  measurements.
   */
    
  Trajectory( const TrajectorySeed& seed) : 
    theChiSquared(0), theValid(true),
    theNumberOfFoundHits(0), theNumberOfLostHits(0),
    theDirection(alongMomentum), theDirectionValidity(false),
    theSeed(seed), seedRef_()
  {}

  /** Constructor of an empty trajectory with defined direction.
   *  No check is made in the push method that measurements are
   *  added in the correct direction.
   */
  Trajectory( const TrajectorySeed& seed, PropagationDirection dir) : 
    theChiSquared(0), theValid(true),
    theNumberOfFoundHits(0), theNumberOfLostHits(0),
    theDirection(dir), theDirectionValidity(true),
    theSeed(seed), seedRef_()
  {}

  /** Reserves space in the vector to avoid lots of allocations when 
      push_back-ing measurements */
  void reserve (unsigned int n) { theData.reserve(n); }
  
  /** Add a new measurement to a Trajectory.
   *  The Chi2 of the trajectory is incremented by the value
   *  of tm.estimate() . 
   */
  void push( const TrajectoryMeasurement& tm);

  /** same as the one-argument push, but the trajectory Chi2 is incremented 
   *  by chi2Increment. Useful e.g. in trajectory smoothing.
   */
  void push( const TrajectoryMeasurement& tm, double chi2Increment);

  /** Remove the last measurement from the trajectory.
   */
  void pop();

  /** Access to the last measurement.
   *  It's the most precise one in a trajectory before smoothing.
   *  It's the outermost measurement if direction() == alongMomentum,
   *  the innermost one if direction() == oppositeToMomentum.
   */
  TrajectoryMeasurement const & lastMeasurement() const {
    check(); 
    if (theData.back().recHit()->hit()!=0) return theData.back();
    else if (theData.size()>2) return *(theData.end()-2);
    else throw cms::Exception("TrajectoryMeasurement::lastMeasurement - Too few measurements in trajectory");
  }

  /** Access to the first measurement.
   *  It is the least precise one in a trajectory before smoothing.
   *  It is precise in a smoothed trajectory. 
   *  It's the innermost measurement if direction() == alongMomentum,
   *  the outermost one if direction() == oppositeToMomentum.
   */
  TrajectoryMeasurement const & firstMeasurement() const {
    check(); 
    if (theData.front().recHit()->hit()!=0) return theData.front();
    else if (theData.size()>2) return *(theData.begin()+1);
    else throw cms::Exception("TrajectoryMeasurement::firstMeasurement - Too few measurements in trajectory");
  }
  
  /** Return all measurements in a container.
   */
  DataContainer const & measurements() const { return theData;}
  /// obsolete name, use measurements() instead.
  DataContainer const & data() const { return measurements();}

  /** Return all RecHits in a container.
   */
  ConstRecHitContainer recHits(bool splitting=false) const;

  void recHitsV(ConstRecHitContainer & cont,bool splitting = false) const;

  /** Just valid hits..
   *
   */
  void validRecHits(ConstRecHitContainer & cont) const;

  /** Number of valid RecHits used to determine the trajectory.
   *  Can be less than the number of measurements in data() since
   *  detector layers crossed without using RecHits from them are also 
   *  stored as measurements.
   */

  int foundHits() const { return theNumberOfFoundHits;}

  /** Number of detector layers crossed without valid RecHits.
   *  Used mainly as a criteria for abandoning a trajectory candidate
   *  during trajectory building.
   */

  int lostHits() const { return theNumberOfLostHits;}
  
  /// True if trajectory has no measurements.
  bool empty() const { return theData.empty();}

  /// Value of the raw Chi2 of the trajectory, not normalised to the N.D.F.
  double chiSquared() const { return theChiSquared;}

  /// Number of dof of the trajectory. The method accepts a bool in order to properly 
  /// take into account the presence of magnetic field in the dof computation.
  /// By default the MF is considered ON.
  int ndof(bool bon = true) const;


  /** Direction of "growing" of the trajectory. 
   *  Possible values are alongMomentum (outwards) and 
   *  oppositeToMomentum (inwards).
   */
  PropagationDirection const & direction() const;

  /** Returns true if the Trajectory is valid.
   *  Trajectories are invalidated e.g. during ambiguity resolution.
   */
  bool isValid() const { return theValid;}

  /// Method to invalidate a trajectory. Useful during ambiguity resolution.
  void invalidate() { theValid = false;}

  /// Access to the seed used to reconstruct the Trajectory
  TrajectorySeed const & seed() const { return theSeed;}


  /** Definition of inactive Det from the Trajectory point of view.
   */
  static bool inactive(//const Det& det
		       ){return false;}//FIXME

  /** Definition of what it means for a hit to be "lost".
   *  This definition is also used by the TrajectoryBuilder.
   */
  static bool lost( const TransientTrackingRecHit& hit);

  /// Redundant method, returns the layer of lastMeasurement() .
  const DetLayer* lastLayer() const {
    check();
    if (theData.back().recHit()->hit()!=0) return theData.back().layer();
    else if (theData.size()>2) return (theData.end()-2)->layer();
    else throw cms::Exception("TrajectoryMeasurement::lastMeasurement - Too few measurements in trajectory");
  }

  /**  return the Reference to the trajectory seed in the original
   *   seeds collection. If the collection has been dropped from the
   *   Event, the reference may be invalid. Its validity should be tested,
   *   before the reference is actually used. 
   */
  edm::RefToBase<TrajectorySeed> seedRef(void) const { return seedRef_; }
  
  void setSeedRef(const edm::RefToBase<TrajectorySeed> & seedRef) { seedRef_ = seedRef ; } 

  TrajectoryStateOnSurface geometricalInnermostState() const;

  TrajectoryMeasurement const & closestMeasurement(GlobalPoint) const; 

  /// Reverse the propagation direction and the order of the trajectory measurements.
  /// It doesn't reverse the forward and backward predicted states within each trajectory measurement
  void reverse() ;
private:

  /// used to determine closest measurement to given point
  struct LessMag {
    LessMag(GlobalPoint point) : thePoint(point) {}
    bool operator()(const TrajectoryMeasurement& lhs,
                    const TrajectoryMeasurement& rhs) const{ 
            return (lhs.updatedState().globalPosition() - thePoint).mag() < (rhs.updatedState().globalPosition() -thePoint).mag();
      }
    GlobalPoint thePoint;
  };

  DataContainer theData;
  double theChiSquared;
  bool theValid;

  int theNumberOfFoundHits;
  int theNumberOfLostHits;

  PropagationDirection theDirection;
  bool                 theDirectionValidity;

  TrajectorySeed       theSeed;
  edm::RefToBase<TrajectorySeed> seedRef_;

  void check() const;
};

#endif
