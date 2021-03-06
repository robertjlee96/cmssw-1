#include <algorithm>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimTracker/TrackHistory/interface/HistoryBase.h"


void HistoryBase::traceGenHistory(HepMC::GenParticle const * genParticle)
{
    // Third stop criteria: status abs(depth_) particles after the hadronization.
    // The after hadronization is done by detecting the pdg_id pythia code from 88 to 99
    if ( genParticle->status() <= abs(depth_) && (genParticle->pdg_id() < 88 || genParticle->pdg_id() > 99) )
    {
        genParticleTrail_.push_back(genParticle);
        // Get the producer vertex and trace it history
        traceGenHistory( genParticle->production_vertex() );
    }
}


void HistoryBase::traceGenHistory(HepMC::GenVertex const * genVertex)
{
    // Verify if has a vertex associated
    if (genVertex)
    {
        // Skip if already exist in the collection
        if ( genVertexTrailHelper_.find(genVertex) != genVertexTrailHelper_.end() )
            return;
        // Add vertex to the history
        genVertexTrail_.push_back(genVertex);
        genVertexTrailHelper_.insert(genVertex);
        // Verify if the vertex has incoming particles
        if ( genVertex->particles_in_size() )
            traceGenHistory( *(genVertex->particles_in_const_begin()) );
    }
}


bool HistoryBase::traceSimHistory(TrackingParticleRef const & trackingParticle, int depth)
{
    // first stop condition: if the required depth is reached
    if ( depth == depth_ && depth_ >= 0 ) return true;

    // sencond stop condition: if a gen particle is associated to the TP
    if ( !trackingParticle->genParticle().empty() )
    {
        LogDebug("TrackHistory") << "Particle " << trackingParticle->pdgId() << " has a GenParicle image." << std::endl;
        traceGenHistory(&(**(trackingParticle->genParticle_begin())));
        return true;
    }

    LogDebug("TrackHistory") << "No GenParticle image for " << trackingParticle->pdgId() << std::endl;

    // get a reference to the TP's parent vertex and trace it history
    return traceSimHistory( trackingParticle->parentVertex(), depth );
}


bool HistoryBase::traceSimHistory(TrackingVertexRef const & trackingVertex, int depth)
{
    // verify if the parent vertex exists
    if ( trackingVertex.isNonnull() )
    {
        // save the vertex in the trail
        simVertexTrail_.push_back(trackingVertex);

        if ( !trackingVertex->sourceTracks().empty() )
        {
            LogDebug("TrackHistory") << "Moving on to the parent particle." << std::endl;

            // select the original source in case of combined vertices
            bool flag = false;
            TrackingVertex::tp_iterator itd, its;

            for (its = trackingVertex->sourceTracks_begin(); its != trackingVertex->sourceTracks_end(); its++)
            {
                for (itd = trackingVertex->daughterTracks_begin(); itd != trackingVertex->daughterTracks_end(); itd++)
                    if (itd != its)
                    {
                        flag = true;
                        break;
                    }
                if (flag)
                    break;
            }

            // verify if the new particle is not in the trail (looping partiles)
            if (
                std::find(
                    simParticleTrail_.begin(),
                    simParticleTrail_.end(),
                    *its
                ) != simParticleTrail_.end()
            )
            {
                LogDebug("TrackHistory") <<  "WARNING: Looping track found." << std::endl;
                return false;
            }

            // save particle in the trail
            simParticleTrail_.push_back(*its);
            return traceSimHistory (*its, --depth);
        }
        else if ( !trackingVertex->genVertices().empty() )
        {
            // navigate over all the associated generated vertexes
            LogDebug("TrackHistory") << "Vertex has " << trackingVertex->genVertices().size() << "GenVertex image." << std::endl;
            for (
                TrackingVertex::genv_iterator ivertex = trackingVertex->genVertices_begin();
                ivertex != trackingVertex->genVertices_end();
                ++ivertex
            )
                traceGenHistory(&(**(ivertex)));
            return true;
        }
        else
        {
            LogDebug("TrackHistory") <<  "WARNING: Source track for tracking vertex cannot be found." << std::endl;
        }
    }
    else
    {
        LogDebug("TrackHistory") << " WARNING: Vertex cannot be found.";
    }

    return false;
}

