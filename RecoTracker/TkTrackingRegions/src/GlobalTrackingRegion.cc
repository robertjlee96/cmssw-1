#include "RecoTracker/TkTrackingRegions/interface/GlobalTrackingRegion.h"
#include <cmath>
#include <sstream>
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "RecoTracker/TkTrackingRegions/interface/HitEtaCheck.h"
#include "RecoTracker/TkTrackingRegions/interface/HitRCheck.h"
#include "RecoTracker/TkTrackingRegions/interface/HitZCheck.h"

#include "RecoTracker/TkMSParametrization/interface/PixelRecoUtilities.h"
#include "RecoTracker/TkMSParametrization/interface/MultipleScatteringParametrisation.h"
#include "RecoTracker/TkMSParametrization/interface/PixelRecoPointRZ.h"
#include "RecoTracker/TkMSParametrization/interface/PixelRecoLineRZ.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"


template <class T> T sqr( T t) {return t*t;}

using namespace GeomDetEnumerators;

std::string GlobalTrackingRegion::print() const {
  std::ostringstream str;
  str << TrackingRegionBase::print() << "precise: "<<thePrecise;
  return str.str(); 
}

TrackingRegion::Hits GlobalTrackingRegion::hits(
      const edm::Event& ev,
      const edm::EventSetup& es,
      const ctfseeding::SeedingLayer* layer) const
{
 return layer->hits(ev,es);
}



HitRZCompatibility* 
GlobalTrackingRegion::checkRZ(const DetLayer* layer, 
			      const Hit& outerHit, const edm::EventSetup& iSetup, 
			      const DetLayer* outerlayer, float u, float v, float dv) const
{

  bool isBarrel = layer->isBarrel();
  bool isPixel = (layer->subDetector() == PixelBarrel || layer->subDetector() == PixelEndcap);
  
  if unlikely(!outerlayer) {
      GlobalPoint ohit =  outerHit->globalPosition();
      if (isBarrel) {
	u = sqrt( sqr(ohit.x()-origin().x())+sqr(ohit.y()-origin().y()) );
	v = ohit.z();
	dv = outerHit->errorGlobalZ();
      } else {
	v = sqrt( sqr(ohit.x()-origin().x())+sqr(ohit.y()-origin().y()) );
	u = ohit.z();
	dv = outerHit->errorGlobalR();
      }
    }
  


  auto lr = isBarrel ? u : v;
  auto gz =  isBarrel ? v : u;
  PixelRecoPointRZ outerred(lr, gz);


  PixelRecoPointRZ vtxR = (gz > origin().z()+originZBound()) ?
      PixelRecoPointRZ(-originRBound(), origin().z()+originZBound())
    : PixelRecoPointRZ( originRBound(), origin().z()+originZBound());
  PixelRecoPointRZ vtxL = (gz< origin().z()-originZBound()) ?
      PixelRecoPointRZ(-originRBound(), origin().z()-originZBound())
    : PixelRecoPointRZ( originRBound(), origin().z()-originZBound()); 

  if unlikely((!thePrecise) &&(isPixel )) {
    double VcotMin = PixelRecoLineRZ( vtxR, outerred).cotLine();
    double VcotMax = PixelRecoLineRZ( vtxL, outerred).cotLine();
    return new HitEtaCheck(isBarrel, outerred, VcotMax, VcotMin);
  }
  
  constexpr float nSigmaPhi = 3.;
 
  dv *= nSigmaPhi;
  PixelRecoPointRZ  outerL, outerR;

  if (layer->isBarrel()) {
    outerL = PixelRecoPointRZ(u, v-dv);
    outerR = PixelRecoPointRZ(u, v+dv);
  } 
  else if (gz > 0) {
    outerL = PixelRecoPointRZ(v+dv, u);
    outerR = PixelRecoPointRZ(v-dv, u);
  } 
  else {
    outerL = PixelRecoPointRZ(v-dv, u);
    outerR = PixelRecoPointRZ(v+dv, u);
  }
  
  MultipleScatteringParametrisation iSigma(layer,iSetup);
  PixelRecoPointRZ vtxMean(0.,origin().z());

  /*
  float innerScatt=0;
  if (outerlayer) {
    innerScatt = 3.f * iSigma( ptMin(), vtxMean, outerred);
    float anew = 3.f * iSigma( ptMin(), vtxMean, outerred, outerlayer->seqNum());
    if (std::abs( (innerScatt-anew)/innerScatt) > .05)  
    std::cout << "MS old/new in " << outerlayer->seqNum() << " " << layer->seqNum()
              << ": " << innerScatt <<  " / " <<   anew
             << std::endl;
  } else
  innerScatt = 3.f * iSigma( ptMin(), vtxMean, outerred);
  */


  float innerScatt = 3.f * ( outerlayer ?
     iSigma( ptMin(), vtxMean, outerred, outerlayer->seqNum())
    :  iSigma( ptMin(), vtxMean, outerred) ) ;


  //
  //
  //
  SimpleLineRZ leftLine( vtxL, outerL);
  SimpleLineRZ rightLine( vtxR, outerR);
  HitRZConstraint rzConstraint(leftLine, rightLine);
  float cotTheta = SimpleLineRZ(vtxMean,outerred).cotLine();

  if (isBarrel) {
    float sinTheta = 1/std::sqrt(1+sqr(cotTheta));
    float corrZ = innerScatt/sinTheta + dv;
    return new HitZCheck(rzConstraint, HitZCheck::Margin(corrZ,corrZ));
  } else {
    float cosTheta = 1/std::sqrt(1+sqr(1/cotTheta));
    float corrR = innerScatt/cosTheta + dv;
    return new HitRCheck( rzConstraint, HitRCheck::Margin(corrR,corrR));
  }
}
