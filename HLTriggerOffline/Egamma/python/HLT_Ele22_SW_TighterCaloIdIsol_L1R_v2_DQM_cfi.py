#----------------------------------------
# path HLT_Ele22_SW_TighterCaloIdIsol_L1R_v2
#----------------------------------------

import FWCore.ParameterSet.Config as cms

HLT_Ele22_SW_TighterCaloIdIsol_L1R_v2_DQM = cms.EDAnalyzer("EmDQM",
    genEtaAcc = cms.double(2.5),
    triggerobject = cms.InputTag("hltTriggerSummaryRAW","","HLT"),
    cutnum = cms.int32(1),
    genEtMin = cms.untracked.double(22.0),
    cutcollection = cms.InputTag("fiducialWenu"),
    genEtAcc = cms.double(2.0),
    reqNum = cms.uint32(1),
    filters = cms.VPSet(cms.PSet(
        PlotBounds = cms.vdouble(0.0, 0.0),
        theHLTOutputTypes = cms.int32(-82),
        IsoCollections = cms.VInputTag(cms.InputTag("none")),
        HLTCollectionLabels = cms.InputTag("hltL1sL1SingleEG8","","HLT")
    ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("none")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolL1MatchFilterRegional","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("none")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolEtFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("hltL1IsoR9shape"), cms.InputTag("hltL1NonIsoR9shape")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolR9ShapeFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("hltL1IsoHLTClusterShape"), cms.InputTag("hltL1NonIsoHLTClusterShape")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolClusterShapeFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("hltL1IsolatedPhotonEcalIsol"), cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloldIsolEcalIsolFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("hltL1IsolatedPhotonHcalForHE"), cms.InputTag("hltL1NonIsolatedPhotonHcalForHE")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolHEFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("hltL1IsolatedPhotonHcalIsol"), cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolHcalIsolFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(92),
            IsoCollections = cms.VInputTag(cms.InputTag("none")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolPixelMatchFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(82),
            IsoCollections = cms.VInputTag(cms.InputTag("none")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolOneOEMinusOneOPFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(82),
            IsoCollections = cms.VInputTag(cms.InputTag("hltElectronL1IsoDetaDphi","Deta"), cms.InputTag("hltElectronL1NonIsoDetaDphi","Deta")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolDetaFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(82),
            IsoCollections = cms.VInputTag(cms.InputTag("hltElectronL1IsoDetaDphi","Dphi"), cms.InputTag("hltElectronL1NonIsoDetaDphi","Dphi")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolDphiFilter","","HLT")
        ), 
        cms.PSet(
            PlotBounds = cms.vdouble(0.0, 0.0),
            theHLTOutputTypes = cms.int32(82),
            IsoCollections = cms.VInputTag(cms.InputTag("hltL1IsoElectronTrackIsol"), cms.InputTag("hltL1NonIsoElectronTrackIsol")),
            HLTCollectionLabels = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt22TighterCaloIdIsolTrackIsolFilter","","HLT")
        )),
    PtMax = cms.untracked.double(100.0),
    pdgGen = cms.int32(11)
)
