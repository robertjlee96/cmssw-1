/** \file
 *
 * $Date: 2009/08/20 15:27:38 $
 * $Revision: 1.2 $
 * \author Enzo Busseti - SNS Pisa <enzo.busseti@sns.it>
 */

#include "RecoLocalMuon/DTSegment/src/DTLPPatternReco4D.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "RecoLocalMuon/DTSegment/src/DTSegmentUpdator.h"

// For the 2D reco I use thei reconstructor!
#include "RecoLocalMuon/DTSegment/src/DTLPPatternReco.h"
//#include "RecoLocalMuon/DTSegment/src/DTSegmentCand.h"


#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/DTRecHit/interface/DTRangeMapAccessor.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/DTRecHit/interface/DTSLRecSegment2D.h"
#include "DataFormats/DTRecHit/interface/DTChamberRecSegment2D.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"

using namespace std;
//using namespace edm;

// TODO
// Throw an exception if a theta segment container is requested and in the event
// there isn't it. (Or launch a "lazy" reco on demand)

DTLPPatternReco4D::DTLPPatternReco4D(const edm::ParameterSet& pset):
  DTRecSegment4DBaseAlgo(pset){

    // debug parameter
    debug = pset.getUntrackedParameter<bool>("debug");

    // the updator
    theUpdator = new DTSegmentUpdator(pset);
    
    applyT0corr = pset.getParameter<bool>("performT0SegCorrection");
    computeT0corr = pset.getUntrackedParameter<bool>("computeT0Seg",true);

    // the input type. 
    // If true the instructions in setDTRecSegment2DContainer will be skipped and the 
    // theta segment will be recomputed from the 1D rechits
    // If false the theta segment will be taken from the Event. Caveat: in this case the
    // event must contain the 2D segments!
    allDTRecHits = pset.getParameter<bool>("AllDTRecHits"); // FIXME: is this needed?

    // Get the concrete 2D-segments reconstruction algo from the factory
    // For the 2D reco I use this reconstructor!
    the2DAlgo = new DTLPPatternReco(pset.getParameter<edm::ParameterSet>("Reco2DAlgoConfig"));
    theUpdator = new DTSegmentUpdator(pset);
    if(debug) cout << "DTLPPatternReco4D Constructor Called" << endl;

}

DTLPPatternReco4D::~DTLPPatternReco4D(){
  if(debug) cout << "DTLPPatternReco4D Destructor Called" << endl;
  delete the2DAlgo;
  delete theUpdator;
}

void DTLPPatternReco4D::setES(const edm::EventSetup& setup){
  setup.get<MuonGeometryRecord>().get(theDTGeometry);
  the2DAlgo->setES(setup);
  theUpdator->setES(setup);
}

void DTLPPatternReco4D::setChamber(const DTChamberId &chId){
  // Set the chamber
  theChamber = theDTGeometry->chamber(chId); 
}

void DTLPPatternReco4D::setDTRecHit1DContainer(edm::Handle<DTRecHitCollection> all1DHits){
  theHitsFromTheta.clear();
  theHitsFromPhi.clear();
  // DTRecHitCollection::range is defined as a pair of const_iterators, and the function returns iterators to
  //the beginning and the end of collection
  DTRecHitCollection::range rangeHitsFromPhi1 = 
    all1DHits->get(DTRangeMapAccessor::layersBySuperLayer( DTSuperLayerId(theChamber->id(),1) ) );
  DTRecHitCollection::range rangeHitsFromPhi2 = 
    all1DHits->get(DTRangeMapAccessor::layersBySuperLayer( DTSuperLayerId(theChamber->id(),3) ) );
  // FIXME: optimize this
  for (edm::OwnVector<DTRecHit1DPair>::const_iterator it = rangeHitsFromPhi1.first; it != rangeHitsFromPhi1.second; ++it)  
    theHitsFromPhi.push_back(*it);
  for (edm::OwnVector<DTRecHit1DPair>::const_iterator it = rangeHitsFromPhi2.first; it != rangeHitsFromPhi2.second; ++it)  
    theHitsFromPhi.push_back(*it);
  //  cout<< "Number of DTRecHit1DPair in the SL 1 (Phi 1): " << theHitsFromPhi1.size()<< endl
  // << "Number of DTRecHit1DPair in the SL 3 (Phi 2): " << theHitsFromPhi2.size()<<endl;
  if(allDTRecHits){
    DTRecHitCollection::range rangeHitsFromTheta = 
      all1DHits->get(DTRangeMapAccessor::layersBySuperLayer( DTSuperLayerId(theChamber->id(),2) ) );
    theHitsFromTheta.assign(rangeHitsFromTheta.first,rangeHitsFromTheta.second);
    if(debug)
      cout<< "Number of DTRecHit1DPair in the SL 2 (Theta): " << theHitsFromTheta.size()<<endl;
  }
}

void DTLPPatternReco4D::setDTRecSegment2DContainer(edm::Handle<DTRecSegment2DCollection> all2DSegments){
  theSegments2DTheta.clear();
  if(!allDTRecHits){
    //Extract the DTRecSegment2DCollection range for the theta SL
    DTRecSegment2DCollection::range rangeTheta = 
      all2DSegments->get(DTSuperLayerId(theChamber->id(),2));
    // Fill the DTRecSegment2D container for the theta SL
    theSegments2DTheta.assign(rangeTheta.first,rangeTheta.second);    // theSegments2DTheta = segments2DTheta;
    if(debug) cout << "Number of 2D-segments in the second SL (Theta): " << theSegments2DTheta.size() << endl;
  }
}

edm::OwnVector<DTRecSegment4D> DTLPPatternReco4D::reconstruct() {
  edm::OwnVector<DTRecSegment4D> result;
  if(debug) cout << "Reconstructing 4D Segments in chamber: " << theChamber->id() << endl;

  if (allDTRecHits){
    // take the theta SL of this chamber
    const DTSuperLayer* sl = theChamber->superLayer(2);
    // sl points to 0 if the theta SL was not found
    if(sl){
      // reconstruct the theta segments
      if(debug) cout <<"Reconstructing the Theta segments"<< endl;
      edm::OwnVector<DTSLRecSegment2D> thetaSegs = the2DAlgo->reconstruct(sl, theHitsFromTheta);
      theSegments2DTheta.assign(thetaSegs.begin(),thetaSegs.end());
      if(debug) cout <<"# of reconstructed theta segs: " <<  theSegments2DTheta.size() << endl;
    }
  }
  // has this chamber the Z-superlayer?
  bool hasZed = theSegments2DTheta.size() > 0;
  if(debug) {
    if (hasZed) cout << "There are " << theSegments2DTheta.size() << " theta cands" << endl;
    else cout << "No Theta cands." << endl;
  }

  // Now I want to build the concrete DTRecSegment4D.
  if (debug) cout << "Reconstructing the Phi segments" << endl;

  edm::OwnVector<DTChamberRecSegment2D> resultPhi = the2DAlgo->reconstructSupersegment(theHitsFromPhi);
  if (debug) cout << "There are " << resultPhi.size() << " Phi cand" << endl;

  if (resultPhi.size()) { //iterating on the result of build_phi_supersegments
    for (edm::OwnVector<DTChamberRecSegment2D>::iterator phi=resultPhi.begin(); phi!=resultPhi.end(); ++phi) {
      if(debug) cout << "superPhi: " << *phi << endl;
      if (hasZed) {
        // Create all the 4D-segment combining the Z view with the Phi one
        // loop over the Z segments
        for(vector<DTSLRecSegment2D>::const_iterator zed = theSegments2DTheta.begin();
	    zed != theSegments2DTheta.end(); ++zed){
          if(debug) cout << "Theta: " << *zed << endl;
          // Important!!
          DTSuperLayerId ZedSegSLId = zed->superLayerId();
          const LocalPoint posZInCh  =
	    theChamber->toLocal( theChamber->superLayer(ZedSegSLId.superlayer())->toGlobal(zed->localPosition() )) ;
          const LocalVector dirZInCh =
	    theChamber->toLocal( theChamber->superLayer(ZedSegSLId.superlayer())->toGlobal(zed->localDirection() )) ;
          DTRecSegment4D* newSeg = new DTRecSegment4D(*phi,*zed,posZInCh,dirZInCh);
          if (debug) cout << "Created a 4D seg " << *newSeg << endl;
          /// 4d segment: I have the pos along the wire => further update!
	  theUpdator->update(newSeg);
	  if(!applyT0corr && computeT0corr) theUpdator->calculateT0corr(newSeg);
          if(applyT0corr) theUpdator->update(newSeg,true);
          result.push_back(*newSeg);
        }
      }
      else {
        // Only phi, we are in chamber lacking z SL
        DTRecSegment4D* newSeg = new DTRecSegment4D(*phi);
        if (debug) cout << "Created a 4D segment using only the 2D Phi segment " << *newSeg << endl;
        //update the segment with the t0 and possibly vdrift correction
        if(!applyT0corr && computeT0corr) theUpdator->calculateT0corr(newSeg);
 	if(applyT0corr) theUpdator->update(newSeg,true);
        result.push_back(newSeg);
      }
    }
  } 
  else {   //if build_phi_supersegments has't returned anything
    // DTRecSegment4D from zed projection only (unlikely, not so useful, but...)
    if (hasZed) {
      for(vector<DTSLRecSegment2D>::const_iterator zed = theSegments2DTheta.begin();
          zed != theSegments2DTheta.end(); ++zed){
        if(debug) cout << "We are building a 4D with only theta: " << *zed << endl;
	// Important!!
        DTSuperLayerId ZedSegSLId = zed->superLayerId();
        const LocalPoint posZInCh  = theChamber->toLocal( theChamber->superLayer(ZedSegSLId.superLayer())->toGlobal(zed->localPosition() )) ;
        const LocalVector dirZInCh = theChamber->toLocal( theChamber->superLayer(ZedSegSLId.superLayer())->toGlobal(zed->localDirection() )) ;
	DTRecSegment4D* newSeg = new DTRecSegment4D( *zed,posZInCh,dirZInCh);
        if (debug) cout << "Created a 4D segment using only the 2D Theta segment " << *newSeg << endl;
	if(!applyT0corr && computeT0corr) theUpdator->calculateT0corr(newSeg);
 	if(applyT0corr) theUpdator->update(newSeg,true);
        result.push_back(newSeg);
      }
    }
  }
  return result;
}

/*
vector<DTSegmentCand*> DTLPPatternReco4D::buildPhiSuperSegmentsCandidates(vector<DTHitPairForFit*> &pairPhiOwned){
  DTSuperLayerId slId;
  if(theHitsFromPhi1.size()) slId = theHitsFromPhi1.front().wireId().superlayerId();
  else if (theHitsFromPhi2.size()) slId = theHitsFromPhi2.front().wireId().superlayerId();
  else {//no hits in the two Phi SL
    if(debug) cout<<"DTCombinatorialPatternReco4D::buildPhiSuperSegmentsCandidates: "
    <<"No Hits in the two Phi SL"<<endl;
    return vector<DTSegmentCand*>(); }
  const DTSuperLayer *sl = theDTGeometry->superLayer(slId);
  vector<DTHitPairForFit*> pairPhi1 = the2DAlgo->initHits(sl,theHitsFromPhi1);
  // same sl!! Since the fit will be in the sl phi 1!
  vector<DTHitPairForFit*> pairPhi2 = the2DAlgo->initHits(sl,theHitsFromPhi2);
  // copy the pairPhi2 in the pairPhi1 vector 
  copy(pairPhi2.begin(),pairPhi2.end(),back_inserter(pairPhi1));
  pairPhiOwned.swap(pairPhi1);
  // Build the segment candidate
  return the2DAlgo->buildSegments(sl,pairPhiOwned);
}
*/


  /* Build a 4d segment with a zed component made by just one hits, apparently this function isn't used anywhere

DTRecSegment4D* DTCombinatorialPatternReco4D::segmentSpecialZed(const DTRecSegment4D* seg){
  // Get the zed projection
  //if (!seg->hasZed()) return seg;
  const DTSLRecSegment2D* zedSeg=seg->zSegment();
  vector<DTRecHit1D> hits = zedSeg->specificRecHits();

  // pick up a hit "in the middle", where the single hit will be put.
  int nHits=hits.size();
  DTRecHit1D middle=hits[static_cast<int>(nHits/2.)];

  //Need to extrapolate pos to the middle layer z
  LocalPoint posInSL = zedSeg->localPosition();
  LocalVector dirInSL = zedSeg->localDirection();
  LocalPoint posInMiddleLayer = posInSL+dirInSL*(-posInSL.z())/cos(dirInSL.theta());

  // create a hit with position and error as the Zed projection one's
  auto_ptr<DTRecHit1D> hit(new DTRecHit1D( middle.wireId(),
                                           middle.lrSide(),
                                           middle.digiTime(),
                                           posInMiddleLayer,
                                           zedSeg->localPositionError()));

  vector<DTRecHit1D> newHits(1,*hit);

  // create a new zed segment with that single hits, but same dir and pos
  LocalPoint pos(zedSeg->localPosition());
  LocalVector dir(zedSeg->localDirection());
  AlgebraicSymMatrix cov(zedSeg->covMatrix());
  double chi2(zedSeg->chi2());
  //cout << "zed " << *zedSeg << endl;
  auto_ptr<DTSLRecSegment2D> newZed(new DTSLRecSegment2D(zedSeg->superLayerId(),
                                                         pos,
                                                         dir,
                                                         cov,
                                                         chi2,
                                                         newHits));
  //cout << "newZed " << *newZed << endl;

  // create a 4d segment with the special zed
  DTRecSegment4D* result= new DTRecSegment4D(*seg->phiSegment(),
                                             *newZed,
                                             seg->localPosition(),
                                             seg->localDirection());
  // delete the input segment
  delete seg;

  // return it
  return result;
}
*/
