// -*- C++ -*-
//
// Package:     Electrons
// Class  :     makeSuperCluster
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Fri Dec  5 15:32:33 EST 2008
// $Id: makeSuperCluster.cc,v 1.6 2010/06/18 12:42:18 yana Exp $
//

// system include files
#include "TEveGeoNode.h"
#include "TGeoBBox.h"
#include "TGeoTube.h"

// user include files
#include "Fireworks/Electrons/interface/makeSuperCluster.h"

#include "Fireworks/Core/interface/BuilderUtils.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Core/interface/FWProxyBuilderBase.h" 

namespace fireworks {
bool makeRhoPhiSuperCluster(FWProxyBuilderBase* pb,
                            const reco::SuperClusterRef& iCluster,
                            float iPhi,
                            TEveElement& oItemHolder)
{
   if ( !iCluster.isAvailable() ) return false;
   TEveGeoManagerHolder gmgr(TEveGeoShape::GetGeoMangeur());

   std::vector< std::pair<DetId, float> > detids = iCluster->hitsAndFractions();
   std::vector<double> phis;
   for (std::vector<std::pair<DetId, float> >::const_iterator id = detids.begin(); id != detids.end(); ++id )
   {
     std::vector<TEveVector> corners = pb->context().getGeom()->getPoints( id->first.rawId());
     if( ! corners.empty() )
     {
       TEveVector centre = corners[0] + corners[1] + corners[2] + corners[3] + corners[4] + corners[5] + corners[6] + corners[7];
       phis.push_back( centre.Phi());
     }
   }
   std::pair<double,double> phiRange = fw::getPhiRange( phis, iPhi);
   const double r = 122;
   TGeoBBox *sc_box = new TGeoTubeSeg(r - 1, r + 1, 1,
                                      phiRange.first * 180 / M_PI - 0.5,
                                      phiRange.second * 180 / M_PI + 0.5 ); // 0.5 is roughly half size of a crystal
   TEveGeoShape *sc = fw::getShape( "supercluster", sc_box, pb->item()->defaultDisplayProperties().color() );
   sc->SetPickable(kTRUE);
   pb->setupAddElement(sc, &oItemHolder);
   return true;
}

bool makeRhoZSuperCluster(FWProxyBuilderBase* pb,
                          const reco::SuperClusterRef& iCluster,
                          float iPhi,
                          TEveElement& oItemHolder)
{

   if ( !iCluster.isAvailable() ) return false;
   TEveGeoManagerHolder gmgr(TEveGeoShape::GetGeoMangeur());
   double theta_max = 0;
   double theta_min = 10;
   std::vector<std::pair<DetId, float> > detids = iCluster->hitsAndFractions();
   for (std::vector<std::pair<DetId, float> >::const_iterator id = detids.begin(); id != detids.end(); ++id)
   {
     std::vector<TEveVector> corners = pb->context().getGeom()->getPoints( id->first.rawId() );
     if( ! corners.empty() )
     {
       TEveVector centre = corners[0] + corners[1] + corners[2] + corners[3] + corners[4] + corners[5] + corners[6] + corners[7];
       double theta = centre.Theta();
       if ( theta > theta_max ) theta_max = theta;
       if ( theta < theta_min ) theta_min = theta;
     }
   }
   // expand theta range by the size of a crystal to avoid segments of zero length
   double z_ecal = 302; // ECAL endcap inner surface
   double r_ecal = 122;
   fw::addRhoZEnergyProjection( pb, &oItemHolder, r_ecal, z_ecal, theta_min-0.003, theta_max+0.003,
                                iPhi);

   return true;
}

}
