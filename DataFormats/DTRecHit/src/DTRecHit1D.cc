

/*
 *  See header file for a description of this class.
 *
 *  $Date: $
 *  $Revision: $
 *  \author G. Cerminara - INFN Torino
 */


#include "DataFormats/DTRecHit/interface/DTRecHit1D.h"


using namespace std;



// Constructor from wireId and digi time only. 
DTRecHit1D::DTRecHit1D(const DTWireId& wireId,
		       DTEnums::DTCellSide lr,
		       float digiTime) : theWireId(wireId),
					 theLRSide(lr),
					 theDigiTime(digiTime),
					 theLocalPosition(),
					 theLocalError() {}



// Constructor from a local position, wireId and digi time.
// The 3-dimensional local error is defined as
// resolution (the cell resolution) for the coordinate being measured
// and 0 for the two other coordinates
DTRecHit1D::DTRecHit1D(const DTWireId& wireId,
		       DTEnums::DTCellSide lr,
		       float digiTime,
		       const LocalPoint& pos) : theWireId(wireId),
						theLRSide(lr),
						theDigiTime(digiTime),
						theLocalPosition(pos) {
			 float cellResolution = 0.02 ; //cm  cell resolution = 200 um = 0.02 cm 
			 theLocalError =
			   LocalError(cellResolution*cellResolution, 0., 0.); //FIXME: is it really needed?
		       }



// Constructor from a local position and error, wireId and digi time.
DTRecHit1D::DTRecHit1D(const DTWireId& wireId,
		       DTEnums::DTCellSide lr,
		       float digiTime,
		       const LocalPoint& pos,
		       const LocalError& err) :theWireId(wireId),
					       theLRSide(lr),
					       theDigiTime(digiTime),
					       theLocalPosition(pos),
					       theLocalError(err) {}




// Destructor
DTRecHit1D::~DTRecHit1D(){}



DTRecHit1D * DTRecHit1D::clone() const {
  return new DTRecHit1D(*this);
}



// Return the detId of the Det (a DTLayer).
DetId DTRecHit1D::geographicalId() const {
  return theWireId.layerId();
}



// Access to component RecHits.
// No components rechits: it returns a null vector
vector<const TrackingRecHit*> DTRecHit1D::recHits() const {
  vector<const TrackingRecHit*> nullvector;
  return nullvector; 
}



// Non-const access to component RecHits.
// No components rechits: it returns a null vector
vector<TrackingRecHit*> DTRecHit1D::recHits() {
  vector<TrackingRecHit*> nullvector;
  return nullvector; 
}



// The ostream operator
ostream& operator<<(ostream& os, const DTRecHit1D& hit) {
  os << "pos: " << hit.localPosition().x() ; 
  os << " +/- " << sqrt(hit.localPositionError().xx()) ;
  return os;
}
