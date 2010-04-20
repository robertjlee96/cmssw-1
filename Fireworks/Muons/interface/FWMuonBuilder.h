#ifndef Fireworks_Muons_FWMuonBuilder_h
#define Fireworks_Muons_FWMuonBuilder_h
// -*- C++ -*-
//
// Package:     Muons
// Class  :     FWMuonBuilder
//
// $Id: FWMuonBuilder.h,v 1.6 2010/04/16 10:29:09 yana Exp $
//
#include "Fireworks/Core/interface/FWEvePtr.h"

// forward declarations
namespace reco {
   class Muon;
}

class FWEventItem;
class TEveElementList;
class TEveTrackPropagator;
class FWMagField;
class FWPRoxyBuilderBase;

class FWMuonBuilder
{

public:
   FWMuonBuilder();
   virtual ~FWMuonBuilder();

   // ---------- const member functions ---------------------

   // ---------- static member functions --------------------

   // ---------- member functions ---------------------------
   void buildMuon(FWProxyBuilderBase*,
                  const reco::Muon* muon,
                  TEveElement* tList,
                  bool showEndcap,
                  bool onlyTracks = false);

private:
   FWMuonBuilder(const FWMuonBuilder&);    // stop default

   const FWMuonBuilder& operator=(const FWMuonBuilder&);    // stop default

   void calculateField(const reco::Muon& iData, FWMagField* field);

   // ---------- member data --------------------------------
   FWEvePtr<TEveTrackPropagator> m_trackerPropagator;
};

#endif
