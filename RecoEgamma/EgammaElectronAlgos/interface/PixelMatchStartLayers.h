#ifndef PIXELMATCHSTARTLAYERS_H
#define PIXELMATCHSTARTLAYERS_H
// -*- C++ -*-
//
// Package:    EgammaElectronAlgos
// Class:      PixelMatchStartLayers
// 
/**\class PixelMatchStartLayers EgammaElectronAlgos/PixelMatchStartLayers

 Description: class to find the innermost pixel forward layers

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ursula Berthon, Claude Charlot
//         Created:  Mon Mar 27 13:22:06 CEST 2006
// $Id$
//
//
#include "TrackingTools/DetLayers/interface/ForwardDetLayer.h" 
#include "TrackingTools/DetLayers/interface/BarrelDetLayer.h" 

class GeometricSearchTracker;

/** Class to find the innermost pixel layers 
 */

class PixelMatchStartLayers {

public:
  typedef vector<ForwardDetLayer*> ForwardLayerContainer;
  typedef vector<ForwardDetLayer*>::const_iterator ForwardLayerIterator;

  typedef vector<BarrelDetLayer*> BarrelLayerContainer;
  typedef vector<BarrelDetLayer*>::const_iterator BarrelLayerIterator;

  PixelMatchStartLayers();
  void setup(const GeometricSearchTracker *);

  ForwardLayerIterator pos1stFLayer()
  {return posPixel.begin();}
  ForwardLayerIterator neg1stFLayer()
  {return negPixel.begin();}
  BarrelLayerIterator firstBLayer()
  {return barrelPixel.begin();}

 
private:
 
  ForwardLayerContainer posPixel;
  ForwardLayerContainer negPixel;
  BarrelLayerContainer barrelPixel;
};

#endif
