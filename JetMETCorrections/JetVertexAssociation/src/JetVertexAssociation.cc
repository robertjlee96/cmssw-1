// -*- C++ -*-
//
// Package:    JetVertexAssociation
// Class:      JetVertexAssociation
// 
/**\class JetVertexAssociation JetVertexAssociation.cc JetMETCorrections/JetVertexAssociation/src/JetVertexAssociation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Natalia Ilina
//         Created:  Tue Oct 31 10:52:41 CET 2006
// $Id$
//
//

/**
  * 'JetVertexAssociation' represents the association of the jet with the signal vertex 
  *
  * Parameters of the method: JV_deltaZ, JV_alpha_threshold(alpha_0 or beta_0),
  *                           JV_cone_size, JV_type_Algo ("1" - alpha, "2" - beta) - (the details are in CMS NOTE 2006/091),
  *
  * Output: <pair<double, bool> >.
  *                    The first - variable alpha(beta) for the jet,
  *                    the second - "true" for jet from signal vertex, "false" for jet from pile-up.
  **/

#include <memory>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/Event.h"

#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/JetVertexAssociation/interface/JetVertexAssociation.h"
#include "JetMETCorrections/JetVertexAssociation/interface/JetVertexMain.h"

using namespace std;
using namespace reco;
namespace cms{

  JetVertexAssociation::JetVertexAssociation(const edm::ParameterSet& iConfig): m_algo(iConfig) , jet_algo(iConfig.getParameter<int>("JET_ALGO")) {

    produces<ResultCollection1>("Var");
    produces<ResultCollection2>("JetType");

    
  }

  void JetVertexAssociation::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

   edm::Handle<CaloJetCollection> jets;  
   if (jet_algo == 1)  iEvent.getByLabel("iterativeCone5CaloJets", jets);  
   if (jet_algo == 2)  iEvent.getByLabel("ktCaloJets", jets); 
   if (jet_algo == 3)  iEvent.getByLabel("midPointCone5CaloJets", jets); 
    
   edm::Handle<TrackCollection> tracks;
   iEvent.getByLabel("ctfWithMaterialTracks", tracks);
  
   edm::Handle<VertexCollection> vertexes;
   iEvent.getByLabel("offlinePrimaryVerticesFromCTFTracks", vertexes); 
 
   double SIGNAL_V_Z = 0.;
   double ptmax = -100.;

   VertexCollection::const_iterator vert = vertexes->begin ();
   if(vertexes->size() > 0 )   { 
        for (; vert != vertexes->end (); vert++) {

                SIGNAL_V_Z = vert->z();
                double pt = 0.;
                reco::track_iterator tr = vert->tracks_begin();
                for (; tr != vert->tracks_end(); tr++)  pt += (*tr)->pt();
                if( pt >= ptmax ){
 
	                  ptmax = pt;
		          SIGNAL_V_Z = vert->z();
    
		}
         
	}
   }

   pair<double, bool> result;
   std::auto_ptr<ResultCollection1> result1 (new ResultCollection1) ; 
   std::auto_ptr<ResultCollection2> result2 (new ResultCollection2) ; 

   CaloJetCollection::const_iterator jet = jets->begin ();
  
   if(jets->size() > 0 )   { 
        for (; jet != jets->end (); jet++) {
	     result = m_algo.Main(*jet, tracks, SIGNAL_V_Z);
             result1->push_back(result.first);
             result2->push_back(result.second);
       
	}
   }

   iEvent.put(result1, "Var");
   iEvent.put(result2, "JetType");

  }
}
