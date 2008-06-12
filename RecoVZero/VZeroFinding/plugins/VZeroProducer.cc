#include "VZeroProducer.h"

#include "RecoVZero/VZeroFinding/interface/VZeroFinder.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include<iostream>

/*****************************************************************************/
VZeroProducer::VZeroProducer(const edm::ParameterSet& pset)
  : pset_(pset)
{
  edm::LogInfo("VZeroProducer") << " constructor";
  produces<reco::VZeroCollection>();

  // Get track level cuts
  minImpactPositiveDaughter =
    pset.getParameter<double>("minImpactPositiveDaughter");
  minImpactNegativeDaughter =
    pset.getParameter<double>("minImpactNegativeDaughter");
}

/*****************************************************************************/
VZeroProducer::~VZeroProducer()
{
  edm::LogInfo("VZeroProducer") << " destructor";
}

void VZeroProducer::produce(edm::Event& ev, const edm::EventSetup& es)
{
  LogDebug("VZeroProducer, produce")<<"event# :"<<ev.id();

  //std::cerr << "[V0 finder]" << std::endl;

  // Get tracks
  edm::Handle<reco::TrackCollection> trackCollection;
  ev.getByLabel("globalSecoTracks",  trackCollection);
  const reco::TrackCollection tracks = *(trackCollection.product());

  // Get primary vertices
  edm::Handle<reco::VertexCollection> vertexCollection;
  ev.getByType(vertexCollection);
  const reco::VertexCollection* vertices = vertexCollection.product();

  // Find vzeros
  VZeroFinder theFinder(es,pset_);

  // Selection based on track impact parameter
  reco::TrackRefVector positives;
  reco::TrackRefVector negatives;

  for(unsigned int i=0; i<tracks.size(); i++)
  {
    if(tracks[i].charge() > 0 &&
       fabs(tracks[i].d0()) > minImpactPositiveDaughter)
      positives.push_back(reco::TrackRef(trackCollection, i));

    if(tracks[i].charge() < 0 &&
       fabs(tracks[i].d0()) > minImpactNegativeDaughter)
      negatives.push_back(reco::TrackRef(trackCollection, i));
  }

  LogTrace("MinBiasTracking") << "[VZeroProducer] using tracks :"
       << " +" << positives.size()
       << " -" << negatives.size();

  std::auto_ptr<reco::VZeroCollection> result(new reco::VZeroCollection);

  // Check all combination of positives and negatives
  if(positives.size() > 0 && negatives.size() > 0)
    for(reco::track_iterator ipos = positives.begin();
                             ipos!= positives.end(); ipos++)
    for(reco::track_iterator ineg = negatives.begin();
                             ineg!= negatives.end(); ineg++)
    {
      reco::VZeroData data;

      if(theFinder.checkTrackPair(**ipos,**ineg, vertices, data) == true)
      {
        // Create vertex (creation point)
        reco::Vertex vertex(reco::Vertex::Point(data.crossingPoint.x(),
                                                data.crossingPoint.y(),
                                                data.crossingPoint.z()),
                            reco::Vertex::Error(), 0.,0.,0);

        // Add references to daughters
        vertex.add(reco::TrackBaseRef(*ipos));
        vertex.add(reco::TrackBaseRef(*ineg));

        // Store vzero
        result->push_back(reco::VZero(vertex,data));
      }
    }

  LogTrace("MinBiasTracking")
    << "[VZeroProducer] found candidates : " << result->size();

  // Put result back to the event
  ev.put(result);
}

