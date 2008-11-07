#ifndef GUARD_DDCoreToDDXMLOutput_H
#define GUARD_DDCoreToDDXMLOutput_H

#include "DetectorDescription/Core/interface/DDTransform.h"
#include "DetectorDescription/Core/interface/DDLogicalPart.h"
#include "DetectorDescription/Core/interface/DDPosData.h"

#include <iostream>

/** @class DDCoreToDDXMLOutput DDCoreToDDXMLOutput.h
 *
 *  @author:  Michael Case               Initial Version
 *  @version: 0.0
 *  @date:    17.10.08
 * 
 *  Description:
 *       The idea is to store one large XML file containing the XML elements
 *     in DDL (Detector Description Language).  In doing so, all DDAlgorithms
 *     or other code generated DD in-memory components should be reproduced 
 *     without the need for them to run again if the geometry is read into
 *     DDLParser again.
 */

struct DDCoreToDDXMLOutput {

  void solid ( const DDSolid& solid, std::ostream& xos );
  
  void material ( const DDMaterial& material, std::ostream& xos );

  void rotation ( DDRotation& rotation, std::ostream& xos );

  void logicalPart ( const DDLogicalPart& lp, std::ostream& xos );

  void position ( const DDLogicalPart& parent
		   , const DDLogicalPart& child
		   , DDPosData* edgeToChild
		  //		   , PIdealGeometry& geom 
		   , int& rotNameSeed
		   , std::ostream& xos );

  void specpar ( const DDSpecifics & sp, std::ostream& xos );

  // data
  //  std::vector<std::string> ddNsName_;
  //  std::map<int, std::string> newddNsName_; // this int is the index in ddNsName_
  
  std::string ns_; // default namespace

};

#endif
