#include "RecoMuon/L3TrackFinder/interface/MuonRoadTrajectoryBuilderESProducer.h"
#include "RecoMuon/L3TrackFinder/interface/MuonRoadTrajectoryBuilder.h"

MuonRoadTrajectoryBuilderESProducer::MuonRoadTrajectoryBuilderESProducer(const edm::ParameterSet& iConfig)
{
  std::string myName = iConfig.getParameter<std::string>("ComponentName");
  measurementTrackerName = iConfig.getParameter<std::string>("measurementTrackerName");
  propagatorName = iConfig.getParameter<std::string>("propagatorName");
  pset_ = iConfig;
  setWhatProduced(this,myName);
}

MuonRoadTrajectoryBuilderESProducer::~MuonRoadTrajectoryBuilderESProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

boost::shared_ptr<TrackerTrajectoryBuilder>
MuonRoadTrajectoryBuilderESProducer::produce(const CkfComponentsRecord& iRecord)
{
   using namespace edm::es;

   edm::ESHandle<MeasurementTracker>             measurementTrackerHandle;
   edm::ESHandle<Propagator> propagatorHandle;
   
   iRecord.get(measurementTrackerName,measurementTrackerHandle);
   iRecord.getRecord<TrackingComponentsRecord>().get(propagatorName,propagatorHandle);
   
   _trajectorybuilder = boost::shared_ptr<TrackerTrajectoryBuilder>(new  MuonRoadTrajectoryBuilder(pset_,
												measurementTrackerHandle.product(),
												propagatorHandle->magneticField(),
												propagatorHandle.product())
								   );
   return _trajectorybuilder;
}
