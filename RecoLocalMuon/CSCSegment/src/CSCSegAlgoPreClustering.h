#ifndef CSCSegment_CSCSegAlgoPreClustering_h
#define CSCSegAlgoPreClustering_h
/**
 * \file CSCSegAlgoPreClustering.h
 *
 *  \authors: S. Stoynev  - NU
 *            I. Bloch    - FNAL
 *            E. James    - FNAL
 *            D. Fortin   - UC Riverside
 *
 * See header file for description.
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <vector>

class CSCChamber;

class CSCSegAlgoPreClustering {

 public:

  typedef std::vector<const CSCRecHit2D*> ChamberHitContainer;

  /// constructor
  explicit CSCSegAlgoPreClustering(const edm::ParameterSet& ps);

  /// destructor
  ~CSCSegAlgoPreClustering();

  /// clusterize
  std::vector< std::vector<const CSCRecHit2D*> > clusterHits( const CSCChamber* aChamber, ChamberHitContainer rechits,
                                                              std::vector<CSCSegment> testSegments );
 private:

  CSCSegment leastSquares(ChamberHitContainer proto_segment);

  bool    debug;
  double  dXclusBoxMax;
  double  dYclusBoxMax;

  float mean_x, mean_y, err_x, err_y;
  const CSCChamber* theChamber;

};
#endif
