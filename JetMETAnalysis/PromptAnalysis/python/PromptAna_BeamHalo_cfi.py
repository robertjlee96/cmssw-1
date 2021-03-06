import FWCore.ParameterSet.Config as cms
# File: BeamHaloAnalyzer_cfi.py
# Original Author: R. Remington, The University of Florida
# Description: Module monitor histograms related to the BeamHaloId tools in the RecoMET package 
# Date: Nov. 15, 2009

promptanahalo = cms.EDProducer("PromptAna_BeamHalo",
                                 InputTag  = cms.string(''),
                                 Prefix    = cms.string(''),
                                 Suffix    = cms.string(''),
                                 L1MuGMTReadoutLabel = cms.InputTag("gtDigis"),
                                 CSCRecHitLabel = cms.InputTag("csc2DRecHits"),
                                 EBRecHitLabel = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                 EERecHitLabel = cms.InputTag("ecalRecHit", "EcalRecHitsEE"),
                                 ESRecHitLabel = cms.InputTag("ecalPreshowerRecHit","EcalRecHitsES"),
                                 HBHERecHitLabel = cms.InputTag("hbhereco"),
                                 HORecHitLabel  = cms.InputTag("horeco"),
                                 HFRecHitLabel = cms.InputTag("hfreco"),
                                 CSCSegmentLabel= cms.InputTag("cscSegments"),
                                 metLabel = cms.InputTag("met"),
                                 CaloTowerLabel = cms.InputTag("towerMaker"),
                                 CollisionMuonLabel = cms.InputTag("muons"),
                                 CollisionStandAloneMuonLabel  =  cms.InputTag("standAloneMuons"),
                                 BeamHaloMuonLabel = cms.InputTag("muonsBeamHaloEndCapsOnly"),
                                 CosmicStandAloneMuonLabel = cms.InputTag("cosmicMuons"),
                                 SuperClusterLabel = cms.InputTag("correctedHybridSuperClusters"),
                                 PhotonLabel = cms.InputTag("photons"),
                                 CSCHaloDataLabel = cms.InputTag("CSCHaloData"),
                                 EcalHaloDataLabel = cms.InputTag("EcalHaloData"),
                                 HcalHaloDataLabel = cms.InputTag("HcalHaloData"),
                                 GlobalHaloDataLabel = cms.InputTag("GlobalHaloData"),
                                 BeamHaloSummaryLabel = cms.InputTag("BeamHaloSummary")
                                 )
