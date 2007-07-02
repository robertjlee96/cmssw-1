#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"


SiStripRecHit2D::SiStripRecHit2D( const LocalPoint& pos, const LocalError& err,
				  const DetId& id,
				  edm::Ref<edm::DetSetVector<SiStripCluster>,SiStripCluster, edm::refhelper::FindForDetSetVector<SiStripCluster>  > const & cluster):
  
  BaseSiTrackerRecHit2DLocalPos(pos,err,id), 
  cluster_(cluster),
  clusterRegional_() {}


SiStripRecHit2D::SiStripRecHit2D( const LocalPoint& pos, const LocalError& err,
				  const DetId& id,
				  edm::SiStripRefGetter<SiStripCluster>::value_ref const& cluster): 
  BaseSiTrackerRecHit2DLocalPos(pos,err,id), 
  cluster_(),
  clusterRegional_(cluster) {}


bool 
SiStripRecHit2D::sharesInput( const TrackingRecHit* other, 
			      SharedInputType what) const
{
  if (geographicalId() != other->geographicalId()) return false;

  const SiStripRecHit2D* otherCast = dynamic_cast<const SiStripRecHit2D*>(other);
  if ( otherCast == 0 )  return false;

  return cluster() == otherCast->cluster();
}

