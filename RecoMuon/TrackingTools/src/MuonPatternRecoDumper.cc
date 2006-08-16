
// This Class Header 
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

// Collaborating Class Header
#include "Geometry/Surface/interface/BoundCylinder.h"
#include "Geometry/Surface/interface/BoundDisk.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"

#include <sstream>

using namespace std;

// Constructor 
MuonPatternRecoDumper::MuonPatternRecoDumper() {
}

// Destructor
MuonPatternRecoDumper::~MuonPatternRecoDumper() {
}

// Operations

string MuonPatternRecoDumper::dumpLayer(const DetLayer* layer) const {
  stringstream output;
  
  BoundSurface* sur=0;
  BoundCylinder* bc=0;
  BoundDisk* bd=0;

  // FIXME
  //  output << " Next layer: " << layer->part() << " " << layer->module() << ":" ;

  sur = (BoundSurface*)&(layer->surface());
  if ( (bc = dynamic_cast<BoundCylinder*>(sur)) ) {
    output << "  Cylinder of radius: " << bc->radius() << endl;
  }
  else if ( (bd = dynamic_cast<BoundDisk*>(sur)) ) {
    output << "  Disk at: " <<  bd->position().z() << endl;
  }
  return output.str();
}

string MuonPatternRecoDumper::dumpFTS(FreeTrajectoryState& fts) const {
  stringstream output;
  
  output  << 
    " pos: " << fts.position() << 
    " radius: " << fts.position().perp() << endl << 
    " charge*pt: " << fts.momentum().perp()*fts.parameters().charge() <<
    " eta: " << fts.momentum().eta() <<
    " phi: " << fts.momentum().phi() << endl;

  return output.str();
}

string MuonPatternRecoDumper::dumpTSOS(TrajectoryStateOnSurface& tsos) const{
  stringstream output;
  
  output<<"dir: "<<tsos.globalDirection();
  dumpFTS(*tsos.freeTrajectoryState());

  return output.str();
}

string MuonPatternRecoDumper::dumpMuonId(const DetId &id) const{
  stringstream output;
  
  if(id.subdetId() == MuonSubdetId::DT ){
    DTChamberId chamberId(id.rawId());
    output<<"(DT): "<<chamberId<<endl;  
  }
  else if(id.subdetId() == MuonSubdetId::CSC){
    CSCDetId chamberId(id.rawId());
    output<<"(CSC): "<<chamberId<<endl;  
  }
  else if(id.subdetId() == MuonSubdetId::RPC){
    RPCDetId chamberId(id.rawId());
    output<<"(RPC): "<<chamberId<<endl;  
  }
  else output<<"The DetLayer is not a valid Muon DetLayer. ";

  return output.str();
}
