import FWCore.ParameterSet.Config as cms

ecalDrivenElectronSeedsParameters = cms.PSet(

    # steering
    fromTrackerSeeds = cms.bool(True),
    initialSeeds = cms.InputTag("newCombinedSeeds"),
    preFilteredSeeds = cms.bool(False),
    useRecoVertex = cms.bool(False),
    vertices = cms.InputTag("offlinePrimaryVerticesWithBS"),
    dynamicPhiRoad = cms.bool(True),
    searchInTIDTEC = cms.bool(True), ##  possibility to inhibit extended forward coverage

    # SC filtering
    SCEtCut = cms.double(4.0),

    # H/E
    applyHOverECut = cms.bool(True),
    hOverEConeSize = cms.double(0.15),
    #maxHOverE = cms.double(0.1),
    maxHOverEBarrel = cms.double(0.15),
    maxHOverEEndcaps = cms.double(0.15),
    maxHBarrel = cms.double(0.0),
    maxHEndcaps = cms.double(0.0),
    # H/E rechits
    hcalRecHits = cms.InputTag("hbhereco"), # OBSOLETE
    hOverEHBMinE = cms.double(0.7),         # OBSOLETE
    hOverEHFMinE = cms.double(0.8),         # OBSOLETE
    # H/E towers
    hcalTowers = cms.InputTag("towerMaker"),
    hOverEPtMin = cms.double(0.),
    
    # r/z windows
    nSigmasDeltaZ1 = cms.double(5.), ## in case beam spot is used for the matching
    deltaZ1WithVertex = cms.double(25.), ## in case reco vertex is used for the matching
    z2MinB = cms.double(-0.09),
    z2MaxB = cms.double(0.09),
    r2MinF = cms.double(-0.15),
    r2MaxF = cms.double(0.15),
    rMinI = cms.double(-0.2), ## intermediate region SC in EB and 2nd hits in PXF
    rMaxI = cms.double(0.2), ## intermediate region SC in EB and 2nd hits in PXF

    # phi windows (dynamic)
    LowPtThreshold = cms.double(5.0),
    HighPtThreshold = cms.double(35.0),
    SizeWindowENeg = cms.double(0.675),
    DeltaPhi1Low = cms.double(0.23),
    DeltaPhi1High = cms.double(0.08),
    DeltaPhi2 = cms.double(0.008),

    # phi windows (non dynamic, overwritten in case dynamic is selected)
    ePhiMin1 = cms.double(-0.125),
    ePhiMax1 = cms.double(0.075),
    pPhiMin1 = cms.double(-0.075),
    pPhiMax1 = cms.double(0.125),
    PhiMin2 = cms.double(-0.002),
    PhiMax2 = cms.double(0.002),
    
)

