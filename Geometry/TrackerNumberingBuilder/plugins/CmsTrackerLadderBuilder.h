#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerLadderBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerLadderBuilder_H

#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>

/**
 * Class which builds Pixel Ladders
 */
class CmsTrackerLadderBuilder : public CmsTrackerLevelBuilder {
  
 private:
  virtual void sortNS(DDFilteredView& , GeometricDet*);
  virtual void buildComponent(DDFilteredView& , GeometricDet*, std::string);

};

#endif
