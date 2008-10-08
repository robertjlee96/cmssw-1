import FWCore.ParameterSet.Config as cms

#HIT REMOVAL
fourthClusters = cms.EDFilter("TrackClusterRemover",
    oldClusterRemovalInfo = cms.InputTag("thClusters"),
    trajectories = cms.InputTag("thStep"),
    pixelClusters = cms.InputTag("thClusters"),
    Common = cms.PSet(
        maxChi2 = cms.double(30.0)
    ),
    stripClusters = cms.InputTag("thClusters")
)


#TRACKER HITS
import RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi
import RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi
fourthPixelRecHits = RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi.siPixelRecHits.clone()
fourthStripRecHits = RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi.siStripMatchedRecHits.clone()
fourthPixelRecHits.src = 'fourthClusters'
fourthStripRecHits.ClusterProducer = 'fourthClusters'




#SEEDS
import RecoTracker.TkSeedGenerator.GlobalMixedSeeds_cfi
fourthPLSeeds = RecoTracker.TkSeedGenerator.GlobalMixedSeeds_cfi.globalMixedSeeds.clone()
import RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi
fourthPLSeeds.OrderedHitsFactoryPSet.SeedingLayers = 'FourthLayerPairs'
fourthPLSeeds.RegionFactoryPSet.RegionPSet.ptMin = 0.6
fourthPLSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 10.0
fourthPLSeeds.RegionFactoryPSet.RegionPSet.originRadius = 2.0


#TRAJECTORY MEASUREMENT
fourthMeasurementTracker = RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi.MeasurementTracker.clone()
import TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi
fourthMeasurementTracker.ComponentName = 'fourthMeasurementTracker'
fourthMeasurementTracker.pixelClusterProducer = 'fourthClusters'
fourthMeasurementTracker.stripClusterProducer = 'fourthClusters'

#TRAJECTORY FILTER
fourthCkfTrajectoryFilter = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.clone()
import RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi
fourthCkfTrajectoryFilter.ComponentName = 'fourthCkfTrajectoryFilter'
fourthCkfTrajectoryFilter.filterPset.maxLostHits = 0
fourthCkfTrajectoryFilter.filterPset.minimumNumberOfHits = 5
fourthCkfTrajectoryFilter.filterPset.minPt = 0.3

#TRAJECTORY BUILDER
fourthCkfTrajectoryBuilder = RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi.GroupedCkfTrajectoryBuilder.clone()
import RecoTracker.CkfPattern.CkfTrackCandidates_cfi
fourthCkfTrajectoryBuilder.ComponentName = 'fourthCkfTrajectoryBuilder'
fourthCkfTrajectoryBuilder.MeasurementTrackerName = 'fourthMeasurementTracker'
fourthCkfTrajectoryBuilder.trajectoryFilterName = 'fourthCkfTrajectoryFilter'


#TRACK CANDIDATES
fourthTrackCandidates = RecoTracker.CkfPattern.CkfTrackCandidates_cfi.ckfTrackCandidates.clone()
import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
fourthTrackCandidates.SeedProducer = 'fourthPLSeeds'
fourthTrackCandidates.TrajectoryBuilder = 'fourthCkfTrajectoryBuilder'


#TRACKS
fourthWithMaterialTracks = RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi.ctfWithMaterialTracks.clone()
fourthWithMaterialTracks.src = 'fourthTrackCandidates'
fourthWithMaterialTracks.clusterRemovalInfo = 'fourthClusters'
fourthWithMaterialTracks.AlgorithmName = cms.string('iter4') 

#SEEDING LAYERS
fourthlayerpairs = cms.ESProducer("PixelLessLayerPairsESProducer",
    ComponentName = cms.string('FourthLayerPairs'),
    layerList = cms.vstring('TIB1+TIB2',
        'TIB1+TID1_pos','TIB1+TID1_neg',
        'TID1_pos+TID2_pos','TID2_pos+TID3_pos','TID3_pos+TEC1_pos',
        'TEC1_pos+TEC2_pos','TEC2_pos+TEC3_pos','TEC3_pos+TEC4_pos','TEC3_pos+TEC5_pos','TEC4_pos+TEC5_pos',
        'TID1_neg+TID2_neg','TID2_neg+TID3_neg','TID3_neg+TEC1_neg',
        'TEC1_neg+TEC2_neg','TEC2_neg+TEC3_neg','TEC3_neg+TEC4_neg','TEC3_neg+TEC5_neg','TEC4_neg+TEC5_neg'),
    TIB = cms.PSet(
        TTRHBuilder = cms.string('WithTrackAngle'),
        matchedRecHits = cms.InputTag("fourthStripRecHits","matchedRecHit")
    ),
    TID = cms.PSet(
        matchedRecHits = cms.InputTag("fourthStripRecHits","matchedRecHit"),
        useRingSlector = cms.untracked.bool(True),
        TTRHBuilder = cms.string('WithTrackAngle'),
        minRing = cms.int32(1),
        maxRing = cms.int32(2)
    ),
    TEC = cms.PSet(
        matchedRecHits = cms.InputTag("fourthStripRecHits","matchedRecHit"),
        useRingSlector = cms.untracked.bool(True),
        TTRHBuilder = cms.string('WithTrackAngle'),
        minRing = cms.int32(1),
        maxRing = cms.int32(2)
    )
)

import RecoTracker.FinalTrackSelectors.selectHighPurity_cfi
pixellessStep = RecoTracker.FinalTrackSelectors.selectHighPurity_cfi.selectHighPurity.clone()
pixellessStep.src = 'fourthWithMaterialTracks'
pixellessStep.copyTrajectories = True
pixellessStep.chi2n_par = 0.3
pixellessStep.res_par = ( 0.003, 0.001 )
pixellessStep.minNumberLayers = 5
pixellessStep.d0_par1 = ( 1.0, 4.0 )
pixellessStep.dz_par1 = ( 1.0, 4.0 )
pixellessStep.d0_par2 = ( 1.0, 4.0 )
pixellessStep.dz_par2 = ( 1.0, 4.0 )

fourthStep = cms.Sequence(fourthClusters*
                          fourthPixelRecHits*fourthStripRecHits*
                          fourthPLSeeds*
                          fourthTrackCandidates*
                          fourthWithMaterialTracks*
                          pixellessStep)
