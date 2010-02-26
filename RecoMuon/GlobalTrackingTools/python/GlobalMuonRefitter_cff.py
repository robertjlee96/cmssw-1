import FWCore.ParameterSet.Config as cms

GlobalMuonRefitter = cms.PSet(
    DTRecSegmentLabel = cms.InputTag("dt1DRecHits"),
    CSCRecSegmentLabel = cms.InputTag("csc2DRecHits"),
    RPCRecSegmentLabel = cms.InputTag("rpcRecHits"),

    MuonHitsOption = cms.int32(1),
    PtCut = cms.double(0.5),
    Chi2ProbabilityCut = cms.double(30.0),
    Chi2CutCSC = cms.double(9.0),
    Chi2CutDT = cms.double(6.0),
    Chi2CutRPC = cms.double(1.0),
    HitThreshold = cms.int32(1),

    Fitter = cms.string('KFFitterForRefitInsideOut'),
    Smoother = cms.string('KFSmootherForRefitInsideOut'),
    Propagator = cms.string('SmartPropagatorAnyRK'),
    TrackerRecHitBuilder = cms.string('WithTrackAngle'),
    MuonRecHitBuilder = cms.string('MuonRecHitBuilder'),
    DoPredictionsOnly = cms.bool(False),
    RefitDirection = cms.string('insideOut'),
    PropDirForCosmics = cms.bool(False),
    RefitRPCHits = cms.bool(True),

    # muon station to be skipped
    SkipStation		= cms.int32(-1),

    # PXB = 1, PXF = 2, TIB = 3, TID = 4, TOB = 5, TEC = 6
    TrackerSkipSystem	= cms.int32(-1),

    # layer, wheel, or disk depending on the system
    TrackerSkipSection	= cms.int32(-1)
)

