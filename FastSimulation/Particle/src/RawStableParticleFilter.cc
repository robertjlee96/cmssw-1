#include "FastSimulation/Particle/interface/RawStableParticleFilter.h"

#include <iostream>

bool RawStableParticleFilter::isOKForMe(const RawParticle* p) const
{
  return (p->status() == 1) ;
}
