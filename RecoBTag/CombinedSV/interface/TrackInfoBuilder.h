#ifndef RecoBTag_TrackInfoBuilder_h
#define RecoBTag_TrackInfoBuilder_h

#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

class MagneticField;

class TrackInfoBuilder {
  public:
   /** \class TrackInfoBuilder
     * \short Creates CombinedTrack from a TrackRef
     */
    TrackInfoBuilder();
    
    /**
     *  set per-jet information
     */
    void setJet ( const reco::Vertex & primary, const GlobalVector & jetdir );
    
    /**
     *  Create CombinedTrack from a TrackRef
     */
    combsv::CombinedTrack build(
        const reco::TransientTrack & ) const;

    /**
     *  convenience method for more tracks
     */
    std::vector < combsv::CombinedTrack > build (
        const std::vector < reco::TransientTrack > & ) const;

  private:
    signed d0Sign( const reco::TransientTrack & ) const;

    /**
     * determine rapidity of a track w.r.t. a given direction
     * default value is 5.0
     */

    double computeTrackRapidity( const reco::TransientTrack & track,
                                 const GlobalVector & pAll ) const;
  private:
    reco::Vertex pv_;
    GlobalVector jetdir_;
};

#endif
