import FWCore.ParameterSet.Config as cms

# Examples for configurations of the trigger match for various physics objects
#
# A detailed description is given in
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATTrigger#PATTriggerMatcher
# Cuts on the parameters
# - 'maxDPtRel' and
# - 'maxDeltaR'
# are NOT tuned (using old values from TQAF MC match, January 2008)


# matches to Egamma triggers

# matches to CandHLT1ElectronStartup
electronTriggerMatchCandHLT1ElectronStartup = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'CandHLT1ElectronStartup' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT1Photon
photonTriggerMatchHLT1Photon = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatPhotons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1Photon' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT1PhotonRelaxed
photonTriggerMatchHLT1PhotonRelaxed = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatPhotons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1PhotonRelaxed' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT2Photon
photonTriggerMatchHLT2Photon = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatPhotons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2Photon' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT2PhotonRelaxed
photonTriggerMatchHLT2PhotonRelaxed = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatPhotons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2PhotonRelaxed' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 1.0 ),
    maxDeltaR = cms.double( 0.2 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT1Electron
electronTriggerMatchHLT1Electron = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1Electron' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT1ElectronRelaxed
# including example of "wrong" match (jets which fired electron trigger),
electronTriggerMatchHLT1ElectronRelaxed = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1ElectronRelaxed' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)
jetTriggerMatchHLT1ElectronRelaxed = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatJets" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1ElectronRelaxed' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT2Electron
electronTriggerMatchHLT2Electron = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2Electron' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT2ElectronRelaxed
electronTriggerMatchHLT2ElectronRelaxed = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2ElectronRelaxed' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to Muon triggers

# matches to HLT1MuonIso
muonTriggerMatchHLT1MuonIso = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1MuonIso' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT1MuonNonIso
muonTriggerMatchHLT1MuonNonIso = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1MuonNonIso' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT2MuonNonIso
muonTriggerMatchHLT2MuonNonIso = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2MuonNonIso' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to BTau triggers

# matches to HLT1Tau
tauTriggerMatchHLT1Tau = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatTaus" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1Tau' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT2TauPixel
tauTriggerMatchHLT2TauPixel = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatTaus" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2TauPixel' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to JetMET triggers

# matches to HLT2jet
jetTriggerMatchHLT2jet = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatJets" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT2jet' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 3.0 ),
    maxDeltaR = cms.double( 0.4 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT3jet
jetTriggerMatchHLT3jet = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatJets" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT3jet' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 3.0 ),
    maxDeltaR = cms.double( 0.24 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT4jet
jetTriggerMatchHLT4jet = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatJets" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT4jet' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 3.0 ),
    maxDeltaR = cms.double( 0.4 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT1MET65
# including example of "wrong" match (cleanPatMuons which fired MET trigger),
metTriggerMatchHLT1MET65 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "patAK5CaloMETs" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1MET65' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 99.0 ),
    maxDeltaR = cms.double( 99.0 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)
muonTriggerMatchHLT1MET65 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT1MET65' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)


# matches to ALL

electronTriggerMatchHltElectrons = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
#     filterIdsEnum  = cms.vstring( '*' ),
#     filterIds      = cms.vint32( +82 ),
    filterIdsEnum  = cms.vstring( 'TriggerElectron' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Ele10_SW_L1R' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

electronTriggerMatchL1Electrons = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
#     filterIdsEnum  = cms.vstring( '*' ),
#     filterIds      = cms.vint32( +82 ),
    filterIdsEnum  = cms.vstring( 'TriggerElectron' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( '*' ),
    collectionTags = cms.vstring( 'hltPixelMatchStartUpElectronsL1Iso::HLT', 'hltL1NonIsoRecoEcalCandidate::HLT' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

muonTriggerMatchL1Muons = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
#     filterIdsEnum  = cms.vstring( '*' ),
#     filterIds      = cms.vint32( -81 ),
    filterIdsEnum  = cms.vstring( 'TriggerL1Mu' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( 'hltMuLevel1PathL1Filtered' ),
    pathNames      = cms.vstring( '*' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

muonTriggerMatchAll = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( True ),
#     filterIdsEnum  = cms.vstring( '*' ),
#     filterIds      = cms.vint32( -81 ),
    filterIdsEnum  = cms.vstring( 'TriggerL1Mu' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( 'hltMuLevel1PathL1Filtered' ),
    pathNames      = cms.vstring( '*' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

muonTriggerMatchNone = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
#     filterIdsEnum  = cms.vstring( '*' ),
#     filterIds      = cms.vint32( -81 ),
    filterIdsEnum  = cms.vstring( 'TriggerL1Mu' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( 'hltMuLevel1PathL1Filtered' ),
    pathNames      = cms.vstring( 'someFunnyPath' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

tauTriggerMatchTriggerTaus = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatTaus" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
#     filterIdsEnum  = cms.vstring( '*' ),
#     filterIds      = cms.vint32( -86, +84 ),
    filterIdsEnum  = cms.vstring( 'TriggerL1TauJet', 'TriggerTau' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( '*' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)


## patTuple ##

# matches to HLT_IsoMu11
muonTriggerMatchHLTIsoMu11 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_IsoMu11' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_Mu11
muonTriggerMatchHLTMu11 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Mu11' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_DoubleIsoMu3
muonTriggerMatchHLTDoubleIsoMu3 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_DoubleIsoMu3' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_DoubleMu3
muonTriggerMatchHLTDoubleMu3 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatMuons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_DoubleMu3' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_IsoEle15_LW_L1I
electronTriggerMatchHLTIsoEle15LWL1I = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_IsoEle15_LW_L1I' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_Ele15_LW_L1R
electronTriggerMatchHLTEle15LWL1R = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_Ele15_LW_L1R' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_DoubleIsoEle10_LW_L1I
electronTriggerMatchHLTDoubleIsoEle10LWL1I = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_DoubleIsoEle10_LW_L1I' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_DoubleEle5_SW_L1R
electronTriggerMatchHLTDoubleEle5SWL1R = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatElectrons" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_DoubleEle5_SW_L1R' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_LooseIsoTau_MET30_L1MET
tauTriggerMatchHLTLooseIsoTauMET30L1MET = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatTaus" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_LooseIsoTau_MET30_L1MET' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)

# matches to HLT_DoubleIsoTau_Trk3
tauTriggerMatchHLTDoubleIsoTauTrk3 = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "cleanPatTaus" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( '*' ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( '*' ),
    pathNames      = cms.vstring( 'HLT_DoubleIsoTau_Trk3' ),
    collectionTags = cms.vstring( '*' ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( False )
)


# sequences
# patTriggerPhotonMatcher = cms.Sequence(
# )
patTriggerElectronMatcher = cms.Sequence(
   electronTriggerMatchHltElectrons +
   electronTriggerMatchL1Electrons
)
patTriggerMuonMatcher = cms.Sequence(
   muonTriggerMatchL1Muons +
   muonTriggerMatchAll     +
   muonTriggerMatchNone
)
patTriggerTauMatcher = cms.Sequence(
    tauTriggerMatchTriggerTaus
)
# patTriggerJetMatcher = cms.Sequence(
# )
# patTriggerMETMatcher = cms.Sequence(
# )

# patTriggerMatcher = cms.Sequence(
#     patTriggerPhotonMatcher   +
#     patTriggerElectronMatcher +
#     patTriggerMuonMatcher     +
#     patTriggerTauMatcher      +
#     patTriggerJetMatcher      +
#     patTriggerMETMatcher
# )
patTriggerMatcher = cms.Sequence(
    patTriggerElectronMatcher +
    patTriggerMuonMatcher     +
    patTriggerTauMatcher
)
