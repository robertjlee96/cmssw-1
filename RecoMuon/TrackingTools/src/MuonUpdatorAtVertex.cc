/** \class MuonUpdatorAtVertex
 *  This class do the extrapolation of a TrajectoryStateOnSurface to the PCA and can apply, with a different
 *  method, the vertex constraint. The vertex constraint is applyed using the Kalman Filter tools used for 
 *  the vertex reconstruction.
 *
 *  $Date: 2007/02/02 16:10:29 $
 *  $Revision: 1.17 $
 *  \author R. Bellan - INFN Torino <riccardo.bellan@cern.ch>
 */

#include "RecoMuon/TrackingTools/interface/MuonUpdatorAtVertex.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

#include "RecoVertex/KalmanVertexFit/interface/SingleTrackVertexConstraint.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackFromFTSFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


using namespace std;

/// Constructor
MuonUpdatorAtVertex::MuonUpdatorAtVertex(const string &propagatorName,
					 const MuonServiceProxy *service):theService(service){
  
  // FIXME
  theChi2Cut = 1000000.;

  // FIXME

  // The SteppingHelixPropagator must be used explicitly since the method propagate(TSOS,GlobalPoint)
  // is only in its specific interface. Once the interface of the Propagator base class  will be
  // updated, then thePropagator will become generic. The string and the MuonServiceProxy are used
  // in order to make more simpler and faster the future transition.
  
  thePropagator = 0;
  
  // FIXME
  // remove the flag as the Propagator base class will gains the propagate(TSOS,Position) method
  theFirstTime = true;
}

/// Destructor
MuonUpdatorAtVertex::~MuonUpdatorAtVertex(){
  delete thePropagator;
}

// Operations


/////////
// FIXME!!! remove this method as the Propagator will gains the propagate(TSOS,Position) method
// remove the flag as well
void MuonUpdatorAtVertex::setPropagator(){
  const string metname = "Muon|RecoMuon|MuonUpdatorAtVertex";
  
  if(theFirstTime ||
     theService->isTrackingComponentsRecordChanged()){
    if(thePropagator) delete thePropagator;
    Propagator *propagator = &*theService->propagator("SteppingHelixPropagatorOpposite")->clone();
    thePropagator = dynamic_cast<SteppingHelixPropagator*>(propagator);  
    theFirstTime = false;

    LogDebug(metname) << " MuonUpdatorAtVertex::setPropagator: propagator changed!";
  }
  
}
///////////

/// Propagate the state to the vertex
// FIXME it is const. It will be when setPropagator() will be removed
pair<bool,FreeTrajectoryState>
MuonUpdatorAtVertex::propagate(const TrajectoryStateOnSurface &tsos, 
			       const GlobalPoint &vtxPosition){

  const string metname = "Muon|RecoMuon|MuonUpdatorAtVertex";

  setPropagator();
  //  return thePropagator->propagate(*tsos.freeState(),vtxPosition);
  pair<FreeTrajectoryState,double> 
    result = thePropagator->propagateWithPath(*tsos.freeState(),vtxPosition);

  LogDebug(metname) << "MuonUpdatorAtVertex::propagate, path: "
		    << result.second << " parameters: " << result.first.parameters();

  if( result.first.hasError()) 
    return pair<bool,FreeTrajectoryState>(true,result.first);
  else{
    edm::LogWarning(metname) << "Propagation to the PCA failed!";
    
    // FIXME: returns FreeTrajectoryState() instead of result.first?
    return pair<bool,FreeTrajectoryState>(false,result.first);
  }
}

// FIXME it is const. It will be when setPropagator() will be removed
pair<bool,FreeTrajectoryState>
MuonUpdatorAtVertex::update(const reco::TransientTrack & track){

  // FIXME
  setPropagator();

  pair<bool,FreeTrajectoryState> result(false,FreeTrajectoryState());
  
  GlobalPoint glbPos(0.,0.,0.);
  
  // assume beam spot position with nominal errors
  // sigma(x) = sigma(y) = 15 microns
  // sigma(z) = 5.3 cm

  AlgebraicSymMatrix mat(3,0);
  mat[0][0] = (15.e-04)*(15.e-04);
  mat[1][1] = (15.e-04)*(15.e-04);
  mat[2][2] = (5.3)*(5.3);
  GlobalError glbErrPos(mat);

  reco::TransientTrack newTransientTrack = theConstrictor.constrain(track,glbPos, glbErrPos);
    
  if(newTransientTrack.chi2() <= theChi2Cut) {
    result.first = true;
    result.second = *newTransientTrack.impactPointState().freeState();
  }
  else
    edm::LogWarning("Muon|RecoMuon|MuonUpdatorAtVertex") << "Constraint at vertex failed"; 
    
  return result;
}

pair<bool,FreeTrajectoryState>
MuonUpdatorAtVertex::update(const FreeTrajectoryState& ftsAtVtx){
  
  return update(theTransientTrackFactory.build(ftsAtVtx));
}



pair<bool,FreeTrajectoryState>
MuonUpdatorAtVertex::propagateWithUpdate(const TrajectoryStateOnSurface &tsos, 
					 const GlobalPoint &vtxPosition){
  
  pair<bool,FreeTrajectoryState>
    propagationResult = propagate(tsos,vtxPosition);

  if(propagationResult.first){
    // FIXME!!!
    // This is very very temporary! Waiting for the changes in the KalmanVertexFitter interface
    return update(propagationResult.second);
  }
  else{
    edm::LogWarning("Muon|RecoMuon|MuonUpdatorAtVertex") << "Constraint at vertex failed";
    return pair<bool,FreeTrajectoryState>(false,FreeTrajectoryState());
  }
}


