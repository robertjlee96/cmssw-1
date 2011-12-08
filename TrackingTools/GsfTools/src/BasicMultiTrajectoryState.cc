#include "TrackingTools/GsfTools/interface/BasicMultiTrajectoryState.h"

#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace SurfaceSideDefinition;

BasicMultiTrajectoryState::BasicMultiTrajectoryState( const std::vector<TSOS>& tsvec) :
  BasicTrajectoryState(tsvec.front().surface()) {

  theStates.reserve(tsvec.size());
  for (std::vector<TSOS>::const_iterator i=tsvec.begin(); i!=tsvec.end(); i++) {
    if (!i->isValid()) {
      throw cms::Exception("LogicError") << "MultiTrajectoryState constructed with invalid state";
    }
    if (i->hasError() != tsvec.front().hasError()) {
      throw cms::Exception("LogicError") << "MultiTrajectoryState mixes states with and without errors";
    }
    if ( &i->surface() != &tsvec.front().surface()) {
      throw cms::Exception("LogicError") << "MultiTrajectoryState mixes states with different surfaces";
    }
    if ( i->surfaceSide() != tsvec.front().surfaceSide()) {
      throw cms::Exception("LogicError") 
	<< "MultiTrajectoryState mixes states defined before and after material";
    }
    if ( i->localParameters().pzSign()*tsvec.front().localParameters().pzSign()<0. ) {
      throw cms::Exception("LogicError") 
	<< "MultiTrajectoryState mixes states with different signs of local p_z";
    }
    if ( i==tsvec.begin() ) {
      // only accept planes!!
      const BoundPlane* bp = dynamic_cast<const BoundPlane*>(&i->surface());
      if ( bp==0 )
	throw cms::Exception("LogicError") << "MultiTrajectoryState constructed on cylinder";
    }
    theStates.push_back( *i);
  }
  combine();
}



void BasicMultiTrajectoryState::rescaleError(double factor) {

  if (theStates.empty()) {
    edm::LogError("BasicMultiTrajectoryState") << "Trying to rescale errors of empty MultiTrajectoryState!";
    return;
  }
  
  for (std::vector<TSOS>::iterator it = theStates.begin(); it != theStates.end(); it++) {
    it->rescaleError(factor);
  }
  combine();
}


BasicMultiTrajectoryState::combine() const {
  const std::vector<TrajectoryStateOnSurface>& tsos = theStates;

  if (tsos.empty()) {
    edm::LogError("MultiTrajectoryStateCombiner") 
      << "Trying to collapse empty set of trajectory states!";
    return TrajectoryStateOnSurface();
  }

  double pzSign = tsos.front().localParameters().pzSign();
  for (std::vector<TrajectoryStateOnSurface>::const_iterator it = tsos.begin(); 
       it != tsos.end(); it++) {
    if (it->localParameters().pzSign() != pzSign) {
      edm::LogError("MultiTrajectoryStateCombiner") 
	<< "Trying to collapse trajectory states with different signs on p_z!";
      return TrajectoryStateOnSurface();
    }
  }
  
  if (tsos.size() == 1) {
    return TrajectoryStateOnSurface(tsos.front());
  }
  
  double sumw = 0.;
  //int dim = tsos.front().localParameters().vector().num_row();
  AlgebraicVector5 mean;
  AlgebraicSymMatrix55 covarPart1, covarPart2;
  for (std::vector<TrajectoryStateOnSurface>::const_iterator it1 = tsos.begin(); 
       it1 != tsos.end(); it1++) {
    double weight = it1->weight();
    AlgebraicVector5 param = it1->localParameters().vector();
    sumw += weight;
    mean += weight * param;
    covarPart1 += weight * it1->localError().matrix();
    for (std::vector<TrajectoryStateOnSurface>::const_iterator it2 = it1 + 1; 
	 it2 != tsos.end(); it2++) {
      AlgebraicVector5 diff = param - it2->localParameters().vector();
      AlgebraicSymMatrix11 s = AlgebraicMatrixID(); //stupid trick to make CLHEP work decently
      covarPart2 += weight * it2->weight() * 
      				ROOT::Math::Similarity(AlgebraicMatrix51(diff.Array(), 5), s);
                        //FIXME: we can surely write this thing in a better way
    }   
  }
  double sumwI = 1.0/sumw;
  mean *= sumwI;
  covarPart1 *= sumwI; covarPart2 *= (sumwI*sumwI);
  AlgebraicSymMatrix55 covar = covarPart1 + covarPart2;

  BasicTrajectoryState::update(LocalTrajectoryParameters(mean, pzSign), 
			       LocalTrajectoryError(covar), 
			       tsos.front().surface(), 
			       &(tsos.front().globalParameters().magneticField()),
			       tsos.front().surfaceSide(), 
			       sumw);
}

void
BasicMultiTrajectoryState::
update( const LocalTrajectoryParameters& p,
        const Surface& aSurface,
        const MagneticField* field,
        const SurfaceSide side) 
{
  throw cms::Exception("LogicError", 
                       "BasicMultiTrajectoryState::update(LocalTrajectoryParameters, Surface, ...) called even if canUpdateLocalParameters() is false");
}

void
BasicMultiTrajectoryState::
update( const LocalTrajectoryParameters& p,
        const LocalTrajectoryError& err,
        const Surface& aSurface,
        const MagneticField* field,
        const SurfaceSide side,
        double weight) 
{
  throw cms::Exception("LogicError", 
                       "BasicMultiTrajectoryState::update(LocalTrajectoryParameters, LocalTrajectoryError, ...) called even if canUpdateLocalParameters() is false");
}

