import FWCore.ParameterSet.Config as cms

from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *
from RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi import *
from RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi import *
from RecoLocalTracker.SiPixelRecHits.PixelCPEParmError_cfi import *
from RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi import *
from RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi import *
from RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi import *
from TrackingTools.MaterialEffects.MaterialPropagator_cfi import *
from RecoTracker.TkSeedingLayers.PixelLessLayerPairs_cfi import *

from RecoTracker.TkSeedGenerator.SeedGeneratorFromRegionHitsEDProducer_cfi import *
globalPixelLessSeeds = RecoTracker.TkSeedGenerator.SeedGeneratorFromRegionHitsEDProducer_cfi.seedGeneratorFromRegionHitsEDProducer.clone(
    OrderedHitsFactoryPSet = cms.PSet(
      ComponentName = cms.string('StandardHitPairGenerator'),
      SeedingLayers = cms.string('PixelLessLayerPairs')
    )
)
