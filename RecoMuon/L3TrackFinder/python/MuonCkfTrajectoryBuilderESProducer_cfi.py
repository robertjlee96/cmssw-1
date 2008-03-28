import FWCore.ParameterSet.Config as cms

MuonCkfTrajectoryBuilder = cms.ESProducer("MuonCkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    trajectoryFilterName = cms.string('muonCkfTrajectoryFilter'),
    maxCand = cms.int32(5),
    ComponentName = cms.string('muonCkfTrajectoryBuilder'),
    intermediateCleaning = cms.bool(False),
    #would skip the first layer to search for measurement if bare TrajectorySeed
    useSeedLayer = cms.bool(False),
    MeasurementTrackerName = cms.string(''),
    estimator = cms.string('Chi2'),
    TTRHBuilder = cms.string('WithTrackAngle'),
    #propagator used only if useSeedLayer=true
    propagatorProximity = cms.string('SteppingHelixPropagatorAny'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(True),
    #would rescale the error to find measurements is failing 
    #1.0 would skip this step completely
    rescaleErrorIfFail = cms.double(1.0),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


