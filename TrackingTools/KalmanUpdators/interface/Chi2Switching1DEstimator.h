#ifndef Chi2Switching1DEstimator_H_
#define Chi2Switching1DEstimator_H_

#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2Strip1DEstimator.h"
#include "Geometry/CommonDetAlgo/interface/DeepCopyPointerByClone.h"

/** A measurement estimator that uses Chi2MeasurementEstimator for
 *  pixel and matched strip hits, and Chi2Strip1DEstimator for
 *  simple strip hits.
 */

class Chi2Switching1DEstimator : public Chi2MeasurementEstimatorBase {

public:

  explicit Chi2Switching1DEstimator(double aMaxChi2, double nSigma = 3.) : 
    Chi2MeasurementEstimatorBase(aMaxChi2,nSigma),
    theLocalEstimator(new Chi2MeasurementEstimator(aMaxChi2,nSigma)),
    theStripEstimator(new Chi2Strip1DEstimator(aMaxChi2,nSigma)) {};

  /// implementation of MeasurementEstimator::estimate
  virtual std::pair<bool, double> estimate(const TrajectoryStateOnSurface& aTsos,
				      const TransientTrackingRecHit& aHit) const;

#ifndef CMS_NO_RELAXED_RETURN_TYPE
  virtual Chi2Switching1DEstimator* clone() const 
#else
  virtual MeasurementEstimator* clone() const 
#endif
  {
    return new Chi2Switching1DEstimator(*this);
  }

private:
  /// estimator for 2D hits (matched or pixel)
  const Chi2MeasurementEstimator& localEstimator() const {
    return *theLocalEstimator;
  }
  /// estimator for 1D hits (non-matched strips)
  const Chi2Strip1DEstimator& stripEstimator() const {
    return *theStripEstimator;
  }

private:
  DeepCopyPointerByClone<const Chi2MeasurementEstimator> theLocalEstimator;
  DeepCopyPointerByClone<const Chi2Strip1DEstimator>     theStripEstimator;

};
#endif //Chi2Switching1DEstimator_H_



