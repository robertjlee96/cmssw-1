#include "RecoEgamma/EgammaPhotonAlgos/interface/OutInConversionSeedFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionBarrelEstimator.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionForwardEstimator.h"
//
#include "FWCore/MessageLogger/interface/MessageLogger.h"
// Field
#include "MagneticField/Engine/interface/MagneticField.h"
//
#include "CLHEP/Matrix/Matrix.h"
// Geometry
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
//
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"
//
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h" 
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"

//
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Geometry/Point3D.h"


OutInConversionSeedFinder::OutInConversionSeedFinder( const MagneticField* field, const MeasurementTracker* theInputMeasurementTracker ) : ConversionSeedFinder( field, theInputMeasurementTracker)  {

  
    LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder CTOR " << "\n";      
    theLayerMeasurements_ =  new LayerMeasurements(theInputMeasurementTracker );


    the2ndHitdphi_ = 0.01; 
    the2ndHitdzConst_ = 5.;
    the2ndHitdznSigma_ = 2.;
    
   
    
  }
 



OutInConversionSeedFinder::~OutInConversionSeedFinder() {
  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder DTOR " << "\n";
  delete theLayerMeasurements_;
 
}


// Return a vector of seeds 
void OutInConversionSeedFinder::makeSeeds( const reco::BasicClusterCollection& allBC )  const  {

  LogDebug("OutInConversionSeedFinder") << "  OutInConversionSeedFinder::makeSeeds() " << "\n";
  theSeeds_.clear();

  theSCPosition_= GlobalPoint ( theSC_->x(), theSC_->y(), theSC_->z() );      
  // debug  

  LogDebug("OutInConversionSeedFinder") << "  OutInConversionSeedFinder::makeSeeds() SC position " << theSCPosition_.x() << " " << theSCPosition_.y() << " " << theSCPosition_.z() << "\n";
  LogDebug("OutInConversionSeedFinder") << " SC eta  " <<  theSCPosition_.eta() <<  " SC phi  " <<  theSCPosition_.phi() << "\n";  
  LogDebug("OutInConversionSeedFinder") << "  OutInConversionSeedFinder::makeSeeds() SC energy " << theSC_->energy()  << "\n";  
  //

  findLayers();

  

  LogDebug("OutInConversionSeedFinder") << " Check Basic cluster collection size " << allBC.size() << "\n";
  
  float  theSCPhi=theSCPosition_.phi();
  float  theSCEta=theSCPosition_.eta();

  

  //  Loop over the Basic Clusters  in the event looking for seeds 
  reco::BasicClusterCollection::const_iterator bcItr;
  for(bcItr = allBC.begin(); bcItr != allBC.end(); bcItr++) {
    theBCEnergy_=bcItr->energy();
    if ( theBCEnergy_ < 1.5 ) continue;

    theBCPosition_ = GlobalPoint(bcItr->position().x(), bcItr->position().y(), bcItr->position().z() ) ;
   
   
    float theBcEta=  theBCPosition_.eta();
    float theBcPhi=  theBCPosition_.phi();
    LogDebug("OutInConversionSeedFinder") << " BC eta  " << theBcEta << " phi " <<  theBcPhi << " BC energy " << theBCEnergy_ << "\n";



    if (  fabs(theBcEta-theSCEta) < 0.015  && fabs(theBcPhi-theSCPhi) < 0.25 ) { 

     fillClusterSeeds( &(*bcItr) );
    }
    

  }


  LogDebug("OutInConversionSeedFinder") << "Built vector of seeds of size  " << theSeeds_.size() <<  "\n" ;
 
  

  
  
}

 


void OutInConversionSeedFinder::fillClusterSeeds(const reco::BasicCluster* bc) const {

  
  theFirstMeasurements_.clear();
  FreeTrajectoryState fts;  

  /// negative charge state
  fts = makeTrackState(-1);
  startSeed(fts);

  /// positive charge state

  fts = makeTrackState(1);
  startSeed(fts);
}


FreeTrajectoryState OutInConversionSeedFinder::makeTrackState(int  charge) const {
  LogDebug("OutInConversionSeedFinder") << "  OutInConversionSeedFinder:makeTrackState " << "\n";


  //  Old GlobalPoint gpOrigine(theBCPosition_.x()*0.3, theBCPosition_.y()*0.3, theBCPosition_.z()*0.3) ;

  GlobalPoint gpOrigine(0.,0.,0.);
  GlobalVector gvBcRadius = theBCPosition_ - gpOrigine ;
  HepPoint3D radiusBc(gvBcRadius.x(),gvBcRadius.y(),gvBcRadius.z()) ;
  HepPoint3D momentumWithoutCurvature = radiusBc.unit() * theBCEnergy_ ;

  // compute momentum direction at calo
  double curvature = theMF_->inTesla(theBCPosition_).z() * c_light * 1.e-3 / momentumWithoutCurvature.perp() ;
  curvature /= 100. ; // in cm-1 !!

   LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeTrackState gpOrigine " << gpOrigine.x() << " " <<  gpOrigine.y() << " " <<  gpOrigine.z() << " momentumWithoutCurvature" << momentumWithoutCurvature << " curvature " << curvature << "\n";

  // define rotation angle
  float R = theBCPosition_.perp();
  float r = gpOrigine.perp();
  float rho = 1./curvature;
  // from the formula for the intersection of two circles
  // turns out to be about 2/3 of the deflection of the old formula
  float d = sqrt(r*r+rho*rho);
   float u = rho + rho/d/d*(R*R-rho*rho) - r/d/d*sqrt((R*R-r*r+2*rho*R)*(R*R-r*r+2*rho*R));
  //float u = rho + rho/d/d*(R*R-rho*rho) ;


  double newdphi = charge * asin(0.5*u/R);

  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeTrackState charge " << charge << " u/R " << u/R << " asin(0.5*u/R) " << asin(0.5*u/R) << "\n";

  HepTransform3D rotation =  HepRotate3D(newdphi, HepVector3D(0., 0. ,1.));


  HepPoint3D momentumInTracker = momentumWithoutCurvature.transform(rotation) ;
   LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeTrackState  R " << R << " r " << r << " rho " << rho  << " d " << d  << " u " << u << " newdphi " << newdphi << " momentumInTracker " <<  momentumInTracker << "\n";

  HepPoint3D hepStartingPoint(gpOrigine.x(), gpOrigine.y(), gpOrigine.z()) ;

   LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeTrackState hepStartingPoint " << hepStartingPoint << "\n";

  hepStartingPoint.transform(rotation);

  GlobalPoint startingPoint(hepStartingPoint.x(), hepStartingPoint.y(), hepStartingPoint.z());

   LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeTrackState startingPoint " << startingPoint << " calo position " << theBCPosition_ << endl;
  GlobalVector gvTracker(momentumInTracker.x(), momentumInTracker.y(), momentumInTracker.z());
  GlobalTrajectoryParameters gtp(startingPoint, gvTracker, charge, theMF_);
  
  return FreeTrajectoryState(gtp) ;


}


void OutInConversionSeedFinder::startSeed(const FreeTrajectoryState & fts) const {


  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::startSeed layer list " << this->layerList().size() <<  "\n";
  LogDebug("OutInConversionSeedFinder") << " fts " << fts <<  "\n";  

  vector<const DetLayer*> myLayers=layerList();
  if ( myLayers.size() > 3 ) {
    
    for(unsigned int ilayer = myLayers.size()-1; ilayer >= myLayers.size()-2; --ilayer) {
      const DetLayer * layer = myLayers[ilayer];
      
      
      // allow the z of the hit to be within a straight line from a vertex
      // of +-15 cm to the cluster
      float dphi = 0.015;
      MeasurementEstimator * newEstimator = makeEstimator(layer, dphi);


      thePropagatorWithMaterial_.setPropagationDirection(alongMomentum);
     
      LogDebug("OutInConversionSeedFinder") << "OutInSeedFinder::startSeed propagationDirection  " << int(thePropagatorWithMaterial_.propagationDirection() ) << "\n";       
 
      TSOS tsos(fts, layer->surface() );

      LogDebug("OutInConversionSeedFinder") << "OutInSeedFinder::startSeed  after  TSOS tsos(fts, layer->surface() ) " << "\n";

      theFirstMeasurements_ = theLayerMeasurements_->measurements( *layer, tsos, thePropagatorWithMaterial_, *newEstimator);

      LogDebug("OutInConversionSeedFinder") << "OutInSeedFinder::startSeed  after  theFirstMeasurements_   " << "\n";

      if(theFirstMeasurements_.size() > 1) // always a dummy returned, too
	LogDebug("OutInConversionSeedFinder") <<  " Found " << theFirstMeasurements_.size()-1 << " 1st hits in seed" << "\n";
      
      delete newEstimator;
      
      LogDebug("OutInConversionSeedFinder") << "OutInSeedFinder::startSeed Layer " << ilayer << " theFirstMeasurements_.size " << theFirstMeasurements_.size() << endl;
      
      for(unsigned int i = 0; i < theFirstMeasurements_.size(); ++i) {
	TrajectoryMeasurement m1 = theFirstMeasurements_[i];
	if(m1.recHit()->isValid()) {
	  
	  // update the fts to start from this point.  much better than starting from
	  // extrapolated point along the line
	  GlobalPoint hitPoint = m1.recHit()->globalPosition();
	  //GlobalPoint hitPoint = innerState.globalPosition();
	  
	  FreeTrajectoryState newfts = trackStateFromClusters(fts.charge(), hitPoint, alongMomentum, 0.8);
	  
	  thePropagatorWithMaterial_.setPropagationDirection(oppositeToMomentum);  
	  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::startSeed propagationDirection  after switching " << int(thePropagatorWithMaterial_.propagationDirection() ) << "\n";               
	  completeSeed(m1, newfts, &thePropagatorWithMaterial_, ilayer-1);
	  // skip a layer, if you haven't already skipped the first layer
	  if(ilayer == myLayers.size()-1) {
	    completeSeed(m1, newfts, &thePropagatorWithMaterial_, ilayer-2);
	  }
	}
      }
      
    } // loop over layers
  }



  
}



MeasurementEstimator * OutInConversionSeedFinder::makeEstimator(const DetLayer * layer, float dphi) const {
 
  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeEstimator  " << "\n";

  MeasurementEstimator * newEstimator=0;

  if (layer->location() == GeomDetEnumerators::barrel ) {
    
    const BarrelDetLayer * barrelLayer = dynamic_cast<const BarrelDetLayer*>(layer);
    LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeEstimator Barrel  r = " << barrelLayer->specificSurface().radius() << " " << "\n";        
    float r = barrelLayer->specificSurface().radius();
    float zrange = 15.* (1.-r/theBCPosition_.perp());
    newEstimator = new ConversionBarrelEstimator(-dphi, dphi, -zrange, zrange);
  }



  if (layer->location() == GeomDetEnumerators::endcap ) {   
   
    const ForwardDetLayer * forwardLayer = dynamic_cast<const ForwardDetLayer*>(layer);
    LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::makeEstimator Endcap r = " << forwardLayer->specificSurface().innerRadius() << " R " << forwardLayer->specificSurface().outerRadius()  <<  " Z " << forwardLayer->specificSurface().position().z() << "\n";  
    //  LogDebug("OutInConversionSeedFinder") << "  InwardConversionSeedFinder::makeEstimator Endcap  2 " << endl;
   float zc = fabs(theBCPosition_.z());
   float z =  fabs(forwardLayer->surface().position().z());
   // LogDebug("OutInConversionSeedFinder") << "  InwardConversionSeedFinder::makeEstimator Endcap  3 " << endl;
   float rrange = 15. * theBCPosition_.perp() * (zc - z) / (zc*zc - 15.*zc);
   newEstimator = new ConversionForwardEstimator(-dphi, dphi, rrange);
  }




  return newEstimator;
}




void OutInConversionSeedFinder::completeSeed(const TrajectoryMeasurement & m1, 
					     FreeTrajectoryState & fts, 
					     const Propagator* propagator, int ilayer) const {

  LogDebug("OutInConversionSeedFinder") <<  "OutInConversionSeedFinder::completeSeed ilayer " << ilayer << "\n";

  MeasurementEstimator * newEstimator=0;
  const DetLayer * layer = theLayerList_[ilayer];
  // LogDebug("OutInConversionSeedFinder") << "no. hits on layer: " << layer->recHits().size() << endl;

  if ( layer->location() == GeomDetEnumerators::barrel ) {
    // z error for 2nd hit is  2 sigma quadded with 5 cm
    LogDebug("OutInConversionSeedFinder") << " Barrel OutInConversionSeedFinder::completeSeed " << the2ndHitdznSigma_ << " " << the2ndHitdzConst_ << " " << the2ndHitdphi_ << "\n";
    float dz = sqrt(the2ndHitdznSigma_*the2ndHitdznSigma_*m1.recHit()->globalPositionError().czz() 
		    + the2ndHitdzConst_*the2ndHitdzConst_);
    newEstimator =
      new ConversionBarrelEstimator(-the2ndHitdphi_, the2ndHitdphi_, -dz, dz);
  }
  else {
    LogDebug("OutInConversionSeedFinder") << " EndCap OutInConversionSeedFinder::completeSeed " << the2ndHitdznSigma_ << " " << the2ndHitdzConst_ << " " << the2ndHitdphi_ << "\n";
    // z error for 2nd hit is 2sigma quadded with 5 cm
    //float m1dr = m1.recHit().globalPositionError().rerr(m1.recHit().globalPosition());
    float m1dr = sqrt(m1.recHit()->localPositionError().yy());
    float dr = sqrt(the2ndHitdznSigma_*the2ndHitdznSigma_*m1dr*m1dr 
                  + the2ndHitdzConst_*the2ndHitdznSigma_);
    // LogDebug("OutInConversionSeedFinder") << "second hit forward dr " << dr << " this hit " << m1dr << endl;
    newEstimator =
      new ConversionForwardEstimator(-the2ndHitdphi_, the2ndHitdphi_, dr);
  }

  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::completeSeed  ilayer " << ilayer <<  "\n"; 
  
  // Get the measurements consistent with the FTS and the Estimator
  TSOS tsos(fts, layer->surface() );
  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::completeSeed propagationDirection  " << int(propagator->propagationDirection() ) << "\n";               
  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::completeSeed pointer to estimator " << newEstimator << "\n";
  vector<TrajectoryMeasurement> measurements = theLayerMeasurements_->measurements( *layer, tsos, *propagator, *newEstimator);
  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::completeSeed Found " << measurements.size() << " second hits " << endl;
  delete newEstimator;

  for(unsigned int i = 0; i < measurements.size(); ++i) {
    if( measurements[i].recHit()->isValid()  ) {
      createSeed(m1, measurements[i]);
    }
  }



  //LogDebug("OutInConversionSeedFinder") << "COMPLETED " << theSeeds_.size() << " SEEDS " << "\n";
}



void OutInConversionSeedFinder::createSeed(const TrajectoryMeasurement & m1, 
                                         const TrajectoryMeasurement & m2) const {

  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::createSeed " << "\n";

  FreeTrajectoryState fts = createSeedFTS(m1, m2);


  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::createSeed First point errors " <<m1.recHit()->parametersError() << "\n";
  // LogDebug("OutInConversionSeedFinder") << "original cluster FTS " << fts << endl;

  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::createSeed propagation dir " << int( thePropagatorWithMaterial_.propagationDirection() ) << "\n"; 
  TrajectoryStateOnSurface state1 = thePropagatorWithMaterial_.propagate(fts,  m1.recHit()->det()->surface());

  // LogDebug("OutInConversionSeedFinder") << "hit surface " << h1.det().surface().position() << endl;
  // LogDebug("OutInConversionSeedFinder") << "prop to " << typeid(h1.det().surface()).name() << endl;
  // LogDebug("OutInConversionSeedFinder") << "prop to first hit " << state1 << endl; 
  // LogDebug("OutInConversionSeedFinder") << "update to " << h1.globalPosition() << endl;

  if ( state1.isValid() ) {
    TrajectoryStateOnSurface updatedState1 = theUpdator_.update(state1,  *m1.recHit() );

    if ( updatedState1.isValid() ) {
      TrajectoryStateOnSurface state2 = thePropagatorWithMaterial_.propagate(*updatedState1.freeTrajectoryState(),  m2.recHit()->det()->surface());

      if ( state2.isValid() ) {

	TrajectoryStateOnSurface updatedState2 = theUpdator_.update(state2, *m2.recHit() );
	TrajectoryMeasurement meas1(state1, updatedState1,  m1.recHit()  , m1.estimate(), m1.layer());
	TrajectoryMeasurement meas2(state2, updatedState2,  m2.recHit()  , m2.estimate(), m2.layer());
	
        edm::OwnVector<TrackingRecHit> myHits;
	myHits.push_back(meas1.recHit()->hit()->clone());
	myHits.push_back(meas2.recHit()->hit()->clone());
   
	LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::createSeed new seed " << "\n";
		
	//	InwardSeed * seed = new InwardSeed(measurements, theBasicCluster, oppositeToMomentum);
	//   LogDebug("OutInConversionSeedFinder") << " InwardConversionSeedFinder seed direction " << seed->direction() << endl;
	TrajectoryStateTransform tsTransform;
	PTrajectoryStateOnDet* ptsod= tsTransform.persistentState(state2, meas2.recHit()->hit()->geographicalId().rawId()  );
	theSeeds_.push_back(TrajectorySeed( *ptsod, myHits, oppositeToMomentum )); 
      }
    }
  }





}





FreeTrajectoryState OutInConversionSeedFinder::createSeedFTS(const TrajectoryMeasurement & m1,
                                  const TrajectoryMeasurement & m2) const {

  LogDebug("OutInConversionSeedFinder") << "OutInConversionSeedFinder::createSeedFTS " << "\n";

  GlobalPoint xmeas = fixPointRadius(m1);
  GlobalPoint xvert = fixPointRadius(m2);

  float pt = theSC_->energy() * sin(theSCPosition_.theta());
  float pz = theSC_->energy() * cos(theSCPosition_.theta());

  // doesn't work at all for endcap, where r is badly measured
  //float dphidr = (p1.phi()-p2.phi())/(p1.perp()-p2.perp());
  //int charge = (dphidr > 0.) ? -1 : 1;
  int charge = m2.predictedState().charge();

  double BInTesla = theMF_->inTesla(xmeas).z();
  GlobalVector xdiff = xmeas -xvert;
 
  double phi= xdiff.phi();
  double pxOld = pt*cos(phi);
  double pyOld = pt*sin(phi);
  double RadCurv = 100*pt/(BInTesla*0.29979);
  double alpha = asin(0.5*xdiff.perp()/RadCurv);
  float ca = cos(charge*alpha);
  float sa = sin(charge*alpha);
  double pxNew =   ca*pxOld + sa*pyOld;
  double pyNew =  -sa*pxOld + ca*pyOld;
  GlobalVector pNew(pxNew, pyNew, pz);

  GlobalTrajectoryParameters gp(xmeas, pNew, charge, theMF_);

  AlgebraicSymMatrix m(5,1) ;
  m[0][0] = 0.05; m[1][1] = 0.02 ; m[2][2] = 0.007 ;
  m[3][3] = 10. ; m[4][4] = 10. ;
  return FreeTrajectoryState(gp, CurvilinearTrajectoryError(m));


}




GlobalPoint OutInConversionSeedFinder::fixPointRadius(const  TrajectoryMeasurement& m1) const {
  GlobalPoint p1 = m1.recHit()->globalPosition();
  GlobalPoint p2;
  if(m1.layer()->location() == GeomDetEnumerators::barrel) {
    p2 = p1;
  } else {
    float z = p1.z();
    float phi = p1.phi();
    float theta = theSCPosition_.theta();
    float r = p1.z() * tan(theta);
    p2 = GlobalPoint(r*cos(phi), r*sin(phi), z);
    // LogDebug("OutInConversionSeedFinder") << "fixing point radius " << p2 << " from " << p1 << endl;
  }
  return p2;
}





