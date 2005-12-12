//   COCOA class header file
//Id:  OptOXLaser.h
//CAT: Model
//
//   Base class to describe Optical Objects of type sensor 2D
// 
//   History: v1.0 
//   Pedro Arce

#ifndef _OptOXLaser_HH
#define _OptOXLaser_HH

#include "OpticalAlignment/CocoaUtilities/interface/CocoaGlobals.h"
#include "OpticalAlignment/CocoaModel/interface/OpticalObject.h"
class Measurement;
class LightRay;

class OptOXLaser: public OpticalObject
{

public:
  //---------- Constructors / Destructor
  OptOXLaser(){ };
  OptOXLaser(OpticalObject* parent, const ALIstring& type, const ALIstring& name, const ALIbool copy_data) : 
  OpticalObject( parent, type, name, copy_data){ };
  ~OptOXLaser(){ };

  //---------- Default behaviour: create a LightRay object
  virtual void defaultBehaviour( LightRay& lightray, Measurement& meas );

#ifdef COCOA_VIS
  virtual void fillIguana();
#endif

};

#endif
