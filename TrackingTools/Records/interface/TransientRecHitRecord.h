#ifndef RecoTracker_Record_TransientRecHitRecord_h
#define RecoTracker_Record_TransientRecHitRecord_h

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "boost/mpl/vector.hpp"

class TransientRecHitRecord : public edm::eventsetup::DependentRecordImplementation<TransientRecHitRecord,
  boost::mpl::vector<IdealGeometryRecord,TrackerDigiGeometryRecord> > {};
#endif 

