#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoExtended.h"
#include <utility>

CrossingFramePlaybackInfoExtended::CrossingFramePlaybackInfoExtended(int minBunch, int maxBunch, unsigned int maxNbSources):maxNbSources_(maxNbSources),minBunch_(minBunch)
{ 
  //initialise data structures
  nBcrossings_=maxBunch-minBunch+1;
  idFirstPileup_.resize(maxNbSources_);
  for (unsigned int i=0;i<maxNbSources_;++i) {
    idFirstPileup_[i].resize(nBcrossings_);
    for (int j=0;j<nBcrossings_;++j) {
      (idFirstPileup_[i])[j][0]=edm::EventID(0,0);
    }
  }
}


void
CrossingFramePlaybackInfoExtended::setEventStartInfo(std::vector<std::vector<edm::EventID> > &id, const unsigned int s)
{  
  idFirstPileup_[s]=id;
}

