/** \class StandAloneTrajectoryBuilder
 *  Concrete class for the STA Muon reco 
 *
 *  $Date: 2006/05/23 17:47:24 $
 *  $Revision: 1.5 $
 *  \author R. Bellan - INFN Torino
 *  \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 */

#include "RecoMuon/StandAloneTrackFinder/interface/StandAloneTrajectoryBuilder.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"

#include "RecoMuon/StandAloneTrackFinder/interface/StandAloneMuonRefitter.h"
#include "RecoMuon/StandAloneTrackFinder/interface/StandAloneMuonBackwardFilter.h"
#include "RecoMuon/StandAloneTrackFinder/interface/StandAloneMuonSmoother.h"

#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

#include "Utilities/Timing/interface/TimingReport.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectoryState/interface/PTrajectoryStateOnDet.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"

#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoMuon/Records/interface/MuonRecoGeometryRecord.h"

using namespace edm;
  
StandAloneMuonTrajectoryBuilder::StandAloneMuonTrajectoryBuilder(const ParameterSet& par){

  // The max allowed eta (physical limit). Since it is the same both for the three filter, 
  // it has been placed here
  theMaxEta = par.getParameter<double>("EtaMaxAllowed");

  // The inward-outward fitter (starts from seed state)
  ParameterSet refitterPSet = par.getParameter<ParameterSet>("RefitterParameters");
  theRefitter = new StandAloneMuonRefitter(refitterPSet);
  
  // The outward-inward fitter (starts from theRefitter outermost state)
  ParameterSet bwFilterPSet = par.getParameter<ParameterSet>("BWFilterParameters");
  theBWFilter = new StandAloneMuonBackwardFilter(bwFilterPSet);

  // The outward-inward fitter (starts from theBWFilter innermost state)
  ParameterSet smootherPSet = par.getParameter<ParameterSet>("SmootherParameters");
  theSmoother = new StandAloneMuonSmoother(smootherPSet);
} 

void StandAloneMuonTrajectoryBuilder::setES(const EventSetup& setup){
  // Get the Tracking Geometry
  setup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry); 
  setup.get<IdealMagneticFieldRecord>().get(theMGField);
  setup.get<MuonRecoGeometryRecord>().get(theDetLayerGeometry); 
  
  // FIXME: move the above lines in the fitters!
  
  theRefitter->setES(setup);
  theBWFilter->setES(setup);
  theSmoother->setES(setup);
}

void StandAloneMuonTrajectoryBuilder::setEvent(const edm::Event& event){
  theRefitter->setEvent(event);
  theBWFilter->setEvent(event);
  theSmoother->setEvent(event);
}

StandAloneMuonTrajectoryBuilder::~StandAloneMuonTrajectoryBuilder(){
  delete theRefitter;
  delete theBWFilter;
  delete theSmoother;
}


// FIXME, change trajL in another name

MuonTrajectoryBuilder::TrajectoryContainer 
StandAloneMuonTrajectoryBuilder::trajectories(const TrajectorySeed& seed){ 

  std::string metname = "StandAloneMuonTrajectoryBuilder::trajectories";
  MuonPatternRecoDumper debug;

  // FIXME put a flag
  bool timing = false;
  TimeMe time_STABuilder_tot(metname,timing);

  TrajectoryContainer trajL;

  // Get the Trajectory State on Det (persistent version of a TSOS) from the seed
  PTrajectoryStateOnDet pTSOD = seed.startingState();

  // Transform it in a TrajectoryStateOnSurface
  TrajectoryStateTransform tsTransform;
  DetId seedDetId(pTSOD.detId());
  const GeomDet* gdet = theTrackingGeometry->idToDet( seedDetId );
  TrajectoryStateOnSurface seedTSOS = tsTransform.transientState(pTSOD, &(gdet->surface()), &*theMGField);

  // Get the layer from which start the trajectory building
  const DetLayer *seedDetLayer = theDetLayerGeometry->idToLayer( seedDetId );

  // FreeTrajectoryState ftk = *tsos.freeTrajectoryState();
  // FreeTrajectoryState ftl(ftk);

  LogDebug(metname)<< "---StandAloneMuonTrajectoryBuilder SEED:" << endl ;
  debug.dumpTSOS(seedTSOS,metname);
  
  if (fabs(seedTSOS.globalMomentum().eta())>theMaxEta) {
    LogDebug(metname) << "############################################################" << endl
		      << "StandAloneMuonTrajectoryBuilder: WARNING!! " << endl
		      << "The SeedGenerator delivers this Trajectory:" << endl;
    debug.dumpTSOS(seedTSOS,metname);
    LogDebug(metname) << "Such an high eta is unphysical and may lead to infinite loop" << endl
		      << "rejecting the Track." << endl
		      << "############################################################" << endl;
    return trajL;
  }

  // reset the refitter
  refitter()->reset();
  
  // refine the FTS given by the seed
  static const string t1 = "StandAloneMuonTrajectoryBuilder::refitter";
  TimeMe timer1(t1,timing);
  refitter()->refit(seedTSOS,seedDetLayer);
  
  int totalNofChamberUsed = refitter()->getTotalChamberUsed();

  // Get the last TSOS
  TrajectoryStateOnSurface tsosAfterRefit = refitter()->lastUpdatedTSOS();

  //@@SL 27-Jun-2002: sanity check for trajectory with very high eta, the true
  //problem is why we do reconstruct such problematics trajectories...
  if (fabs(tsosAfterRefit.globalMomentum().eta())>theMaxEta) {
    LogDebug(metname) << "############################################################" << endl
		      << "StandAloneMuonTrajectoryBuilder: WARNING!! " << endl
		      << "At the end of TrajectoryRefitter the Trajectory is:" << endl;
    debug.dumpTSOS(tsosAfterRefit,metname);
    LogDebug(metname) << "Such an high eta is unphysical and may lead to infinite loop" << endl
		      << "rejecting the Track." << endl
		      << "############################################################" << endl;
    return trajL;
  }
  
  LogDebug(metname) << "--- StandAloneMuonTrajectoryBuilder REFITTER OUTPUT " << endl ;
  debug.dumpTSOS(tsosAfterRefit,metname);
  LogDebug(metname) << "No of DT/CSC/RPC chamber used: " 
		    << refitter()->getDTChamberUsed()
		    << refitter()->getCSCChamberUsed() 
		    << refitter()->getRPCChamberUsed();
    
  
  // BackwardFiltering

  Trajectory theTraj(seed);
  //Trajectory theTraj(seed,oppositeToMomentum);
  
  return trajL;
}
