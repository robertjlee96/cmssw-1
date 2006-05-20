/** \class MuonDetLayerMeasurements
 *  The class to access recHits and TrajectoryMeasurements from DetLayer.
 *
 *  $Date: 2006/05/17 20:49:08 $
 *  $Revision: 1.2 $
 *  \author C. Liu - Purdue University
 *
 */

#include "RecoMuon/MeasurementDet/interface/MuonDetLayerMeasurements.h"

#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/TransientTrackingRecHit/interface/GenericTransientTrackingRecHit.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h" 

MuonDetLayerMeasurements::MuonDetLayerMeasurements() {

}

MuonDetLayerMeasurements::~MuonDetLayerMeasurements() {

}

RecHitContainer MuonDetLayerMeasurements::recHits(const DetLayer* layer, const edm::Event& iEvent) const
{
  RecHitContainer rhs;
  
  Module mtype = layer->module();
  if (mtype == dt ) {
     edm::Handle<DTRecSegment4DCollection> dtRecHits;
     iEvent.getByLabel("recseg4dbuilder", dtRecHits);  //FIXME

     std::vector <const GeomDet*> gds = layer->basicComponents();
     for (std::vector<const GeomDet*>::const_iterator igd = gds.begin(); igd != gds.end(); igd++) {
               DTChamberId chamberId((*igd)->geographicalId().rawId());
               DTRecSegment4DCollection::range  range = dtRecHits->get(chamberId);
               for (DTRecSegment4DCollection::const_iterator rechit = range.first; rechit!=range.second;++rechit){
               TransientTrackingRecHit* gttrh = new GenericTransientTrackingRecHit((*igd), (&(*rechit)));
               rhs.push_back(gttrh);
                }//for DTRecSegment4DCollection
       }// for GeomDet
  }else if (mtype == csc ) {
     edm::Handle<CSCSegmentCollection> cscSegments;
     iEvent.getByLabel("segmentbuilder", cscSegments); 

     std::vector <const GeomDet*> gds = layer->basicComponents();
     for (std::vector<const GeomDet*>::const_iterator igd = gds.begin(); igd != gds.end(); igd++) {
               CSCDetId id((*igd)->geographicalId().rawId());
               CSCSegmentCollection::range  range = cscSegments->get(id);
               for (CSCSegmentCollection::const_iterator rechit = range.first; rechit!=range.second;++rechit){
               TransientTrackingRecHit* gttrh = new GenericTransientTrackingRecHit((*igd), (&(*rechit)));
               rhs.push_back(gttrh);
                }//for CSCSegmentCollection
       }// for GeomDet
  }else if (mtype == rpc ) {

  }else {
      //wrong type
  }
  return rhs;
}

MeasurementContainer
MuonDetLayerMeasurements::measurements( const DetLayer* layer,
              const TrajectoryStateOnSurface& startingState,
              const Propagator& prop,
              const MeasurementEstimator& est,
              const edm::Event& iEvent) const {
   MeasurementContainer result;
   std::vector<DetWithState> dss = layer->compatibleDets(startingState, prop, est);
   RecHitContainer rhs = recHits(layer, iEvent);
   for (std::vector<DetWithState>::const_iterator ids = dss.begin(); ids !=dss.end(); ids++){
     for (RecHitContainer::const_iterator irh = rhs.begin(); irh!=rhs.end(); irh++) {
      if (est.estimate((*ids).second, (**irh)).first)
      result.push_back(TrajectoryMeasurement((*ids).second,(*irh),0,layer));  
     }
   }
   return result;
}

MeasurementContainer
MuonDetLayerMeasurements::fastMeasurements( const DetLayer* layer,
              const TrajectoryStateOnSurface& theStateOnDet,
              const TrajectoryStateOnSurface& startingState,
              const Propagator& prop,
              const MeasurementEstimator& est,
              const edm::Event& iEvent) const {
   MeasurementContainer result;
   RecHitContainer rhs = recHits(layer, iEvent);
   for (RecHitContainer::const_iterator irh = rhs.begin(); irh!=rhs.end(); irh++) {
      if (est.estimate(theStateOnDet, (**irh)).first)
      result.push_back(TrajectoryMeasurement(theStateOnDet,(*irh),0,layer));
   }

   return result;
}

