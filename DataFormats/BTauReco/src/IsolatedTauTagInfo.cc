#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/GenVector/VectorUtil.h>
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

using namespace edm;
using namespace reco;

RefVector<TrackCollection> IsolatedTauTagInfo::tracksInCone( const math::XYZVector myVector, const float size,  const float pt_min) const { 
  
  RefVector<TrackCollection> tmp;
 
  RefVector<TrackCollection>::const_iterator myTrack = selectedTracks_.begin();
  for(;myTrack != selectedTracks_.end(); myTrack++)
    {
      const math::XYZVector trackMomentum = (*myTrack)->momentum() ;
      float pt_tk = (*myTrack)->pt();
      float deltaR = ROOT::Math::VectorUtil::DeltaR(myVector, trackMomentum);
      if ( deltaR < size && pt_tk > pt_min) tmp.push_back( *myTrack);
      }

  return tmp;
}


TrackRef IsolatedTauTagInfo::leadingSignalTrack(const float rm_cone, const float pt_min) const {

  const Jet & myjet = m_jetTag->jet(); 
  math::XYZVector jet3Vec   (myjet.px(),myjet.py(),myjet.pz()) ;

  RefVector<TrackCollection>  sTracks = tracksInCone(jet3Vec, rm_cone, pt_min);
  TrackRef leadTk;
  float pt_cut = pt_min;
  if (sTracks.size() >0) 
    {
      RefVector<TrackCollection>::const_iterator myTrack =sTracks.begin();
      for(;myTrack!=sTracks.end();myTrack++)
	{
	  if((*myTrack)->pt() > pt_cut) {
	    leadTk = *myTrack;
	    pt_cut = (*myTrack)->pt();
	  }
	}
    }
  return leadTk;
}

double IsolatedTauTagInfo::discriminator(float m_cone, float sig_cone, float iso_cone, float pt_min_lt, float pt_min_tk) const
{
  double myDiscriminator = 0;

  TrackRef leadTk = leadingSignalTrack(m_cone, pt_min_lt);
  if(!leadTk) return myDiscriminator;

  math::XYZVector trackMomentum = leadTk->momentum() ;
  RefVector<TrackCollection> signalTracks = tracksInCone(trackMomentum, sig_cone , pt_min_tk);
  RefVector<TrackCollection> isolationTracks =tracksInCone(trackMomentum, iso_cone , pt_min_tk); 
  
  if(signalTracks.size() > 0 && (signalTracks.size() == isolationTracks.size()) )
    myDiscriminator=1;

  return myDiscriminator;




}
