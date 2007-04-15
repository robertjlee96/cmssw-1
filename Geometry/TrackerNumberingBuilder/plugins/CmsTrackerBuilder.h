#ifndef Geometry_TrackerNumberingBuilder_CmsTrackerBuilder_H
#define Geometry_TrackerNumberingBuilder_CmsTrackerBuilder_H

#include "Geometry/TrackerNumberingBuilder/interface/CmsTrackerLevelBuilder.h"
#include "FWCore/ParameterSet/interface/types.h"
#include <string>
/**
 * Abstract Class to construct a Level in the hierarchy
 */
class CmsTrackerBuilder : public CmsTrackerLevelBuilder {
  
 private:
  virtual void sortNS(DDFilteredView& , GeometricDet*);
  virtual void buildComponent(DDFilteredView& , GeometricDet*, std::string);
};

#endif
