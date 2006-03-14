#ifndef DetLayers_DetLayer_h
#define DetLayers_DetLayer_h

#include "TrackingTools/DetLayers/interface/Enumerators.h"
#include "TrackingTools/DetLayers/interface/GeometricSearchDet.h"

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/DetLayers/interface/NavigableLayer.h"
#include <vector>



/** The DetLayer is the detector abstraction used for track reconstruction.
 *  It inherits from GeometricSearchDet the interface for accessing 
 *  components and compatible components. 
 *  It extends the interface by providing navigation capability 
 *  from one layer to another. 
 *  The Navigation links must be created in a 
 *  NavigationSchool and activated with a NavigationSetter before they 
 *  can be used.
 * 
 */

class DetLayer : public GeometricSearchDet {
  
 public:
  typedef GeometricSearchDet          Det;

  DetLayer() : theNavigableLayer(0){};

  virtual ~DetLayer();

  // Extension of the interface 
  virtual Module module() const = 0;
  virtual Part   part()   const = 0;

  NavigableLayer* navigableLayer() const { return theNavigableLayer;}
  virtual void setNavigableLayer( NavigableLayer* nlp);
  
  virtual vector<const DetLayer*> 
  nextLayers( const FreeTrajectoryState& fts, 
	      PropagationDirection timeDirection) const;

  virtual vector<const DetLayer*> 
  nextLayers( PropagationDirection timeDirection) const;


  vector<const DetLayer*> 
  compatibleLayers( PropagationDirection timeDirection ) const;


  vector<const DetLayer*> 
  compatibleLayers(const FreeTrajectoryState& fts, 
		   PropagationDirection timeDirection) const;

  
 private:
  NavigableLayer* theNavigableLayer;

};

#endif 
