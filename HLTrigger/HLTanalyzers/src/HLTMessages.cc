#include "HLTMessages.h"

const char * kSubEventMap                 = "subevent map";
const char * kHLTjets                     = "uncorrected HLT jets";
const char * kHLTCorjets                  = "corrected HLT jets";
const char * kHLTCorL1L2L3jets            = "corrected, L1L2L3, HLT jets";
const char * kRho                         = "rho";
const char * kRecjets                     = "uncorrected reconstructed jets";
const char * kRecCorjets                  = "corrected reconstructed jets";
const char * kGenjets                     = "GEN jets";
const char * kRecmet                      = "reconstructed MET";
const char * kPFMet                       = "reconstructed PF MET";
const char * kGenmet                      = "GEN MET";
const char * kCaloTowers                  = "calo towers";
const char * kCaloTowersUpperR45          = "calo towers upper R45 cleaning";
const char * kCaloTowersLowerR45          = "calo towers lower R45 cleaning";
const char * kCaloTowersNoR45             = "calo towers no R45 cleaning";
const char * kHt                          = "HT object";
const char * kRecoPFJets                  = "RECO particle flow jets"; 
const char * kMuon                        = "RECO muon candidates";
const char * kpfMuon                      = "pf muon candidates";
const char * kTaus                        = "tau candidates";
const char * kPFTaus                      = "pftau candidates";
const char * kPFTausTightCone             = "pftau candidates tight cone";
const char * kPFJets                      = "particle flow jets";
const char * kRecoPFTaus                            = "RECO pftau candidates";
const char * ktheRecoPFTauDiscrByTanCOnePercent     = "RECO PFTau Discriminator By TanC One Percent"; 
const char * ktheRecoPFTauDiscrByTanCHalfPercent    = "RECO PFTau Discriminator By TanC Half Percent"; 
const char * ktheRecoPFTauDiscrByTanCQuarterPercent = "RECO PFTau Discriminator By TanC Quarter Percent"; 
const char * ktheRecoPFTauDiscrByTanCTenthPercent   = "RECO PFTau Discriminator By TanC Tenth Percent"; 
const char * ktheRecoPFTauDiscrByIsolation          = "RECO PFTau Discriminator By Isolation";
const char * ktheRecoPFTauDiscrAgainstMuon          = "RECO PFTau Discriminator Against Muon";
const char * ktheRecoPFTauDiscrAgainstElec          = "RECO PFTau Discriminator Against Elec"; 
const char * kHltresults                  = "HLT results";
const char * kL1extemi                    = "L1 isolated EM objects";
const char * kL1extemn                    = "L1 non isolated EM objects";
const char * kL1extmu                     = "L1 muon objects";
const char * kL1extjetc                   = "L1 central jet objects";
const char * kL1extjetf                   = "L1 forward jet objects";
const char * kL1extjet                    = "L1 jet objects";
const char * kL1exttaujet                 = "L1 tau jet objects";
const char * kL1extmet                    = "L1 EtMiss object";
const char * kL1extmht                    = "L1 HtMiss object";
const char * kL1GtRR                      = "L1 GT readout record";
const char * kL1GtOMRec                   = "L1 GT object map";
const char * kL1GctBitCounts              = "L1 GCT HF bit counts";
const char * kL1GctRingSums               = "L1 GCT HF ring sums";
const char * kMctruth                     = "GEN particles";
const char * kSimhit                      = "SIM information";
const char * kGenEventInfo                = "GEN information";
const char * kMucands2                    = "L2 muon candidates";
const char * kMucands3                    = "L3 muon candidates";
const char * kMunovtxcands2               = "L2 no-vertex muon candidates"; 
const char * kIsoMap2                     = "L2 muon isolation map";
const char * kIsoMap3                     = "L3 muon isolation map";
const char * kIsoTrk10Map3                = "L3 muon Trk10 isolation map"; 
const char * kMulinks                     = "L3 muon link";
const char * kOniaPixelCands              = "Pixel track candidates in resonance with a L3 muon";
const char * kOniaTrackCands              = "Strip track candidates in resonance with a L3 muon";
const char * kDimuvtxcands3               = "L3 dimuon vertex";
const char * kTrkMucands                  = "Unseeded trackerMuons";  

const char * kBTagJets                    = "L2 b-jet collection";
const char * kBTagCorrectedJets           = "L2 calibrated b-jet collection";
const char * kBTagCorrectedJetsL1FastJet  = "L2 calibrated b-jet collection (L1FastJet)";
const char * kBTagPFJets                  = "L2 b-jet collection (PF jets)";
const char * kBTagLifetimeBJetsL25        = "L2.5 b-jet lifetime tags";
const char * kBTagLifetimeBJetsL3         = "L3 b-jet lifetime tags";
const char * kBTagLifetimeBJetsL25L1FastJet        = "L2.5 b-jet lifetime tags (L1FastJet)";
const char * kBTagLifetimeBJetsL3L1FastJet         = "L3 b-jet lifetime tags (L1FastJet)";
const char * kBTagLifetimePFBJetsL3         = "L3 b-jet lifetime tags (PF jets)";
const char * kBTagLifetimeBJetsL25SingleTrack = "L2.5 b-jet lifetime tags (SingleTrack)";
const char * kBTagLifetimeBJetsL3SingleTrack  = "L3 b-jet lifetime tags (SingleTrack)";
const char * kBTagSoftmuonBJetsL25        = "L2.5 b-jet soft muon tags";
const char * kBTagSoftmuonBJetsL3         = "L3 b-jet soft muon tags";
const char * kBTagPerformanceBJetsL25     = "L2.5 b-jet perf. meas. tag";
const char * kBTagPerformanceBJetsL3      = "L3 b-jet perf. meas. tag";
const char * kBTagPerformanceBJetsL25L1FastJet     = "L2.5 b-jet perf. meas. tag (L1FastJet)";
const char * kBTagPerformanceBJetsL3L1FastJet      = "L3 b-jet perf. meas. tag (L1FastJet)";

const char * kElectrons                   = "RECO electron candidates";
const char * kPhotons                     = "RECO photon candidates";
const char * kCandIso                     = "isol eg candidate";
const char * kCandNonIso                  = "non-isol eg candidate";
const char * kEcalIso                     = "Ecal isol map";
const char * kEcalNonIso                  = "Ecal non-isol map";
const char * kHcalIsoPho                  = "Hcal isol photon map";
const char * kHcalNonIsoPho               = "Hcal non-isol photon map";
const char * kIsoPhoTrackIsol             = "Track isol photon map";
const char * kNonIsoPhoTrackIsol          = "Track non-isol photon map";
const char * kHFECALClusters              = "HF ECAL clusters";   
const char * kHFElectrons                 = "HF Electrons"; 
const char * kIsoElectron                 = "isol electron";
const char * kNonIsoElectron              = "isol electron";
const char * kIsoEleHcal                  = "isol Hcal electron";
const char * kNonIsoEleHcal               = "isol Hcal electron"; 
const char * kIsoEleTrackIsol             = "isol Track electron";
const char * kNonIsoEleTrackIsol          = "isol Track electron";
const char * kL1IsoPixelSeeds             = "pixelSeed-SC association map for electron";
const char * kL1NonIsoPixelSeeds          = "pixelSeed-SC for electron";
const char * kNonIsoR9                    = "Spike-cleaning";
const char * kIsoR9                       = "Spike-cleaning"; 
const char * kNonIsoR9ID                  = "isol R9 ID";
const char * kIsoR9ID                     = "non-isol R9 ID";
const char * kIsoHoverEH                  = "H for H/E isol photon map";
const char * kNonIsoHoverEH               = "H for H/E non-isol photon map";

/*
const char * kEErechits                   = "ECAL Endcap RecHits";
const char * kEBrechits                   = "ECAL Barrel RecHits"; 
const char * kHBHErechits                 = "HCAL Endcap-Barrel RecHits"; 
const char * kHOrechits                   = "HCAL HO RecHits";  
const char * kHFrechits                   = "HCAL HF RecHits"; 
const char * kpi0EErechits                = "ECAL pi0 Endcap RecHits"; 
const char * kpi0EBrechits                = "ECAL pi0 Barrel RecHits";  
*/
const char * kIsoPixelTracksL3            = "L3 Iso Pixel Tracks"; 
const char * kIsoPixelTracksL2            = "L2 Iso Pixel Tracks";
const char * kIsoPixelTrackVertices       = "Pixel Vertices";
const char * kPixelTracksL3               = "L3 Pixel Tracks"; 
const char * kPixelFEDSize                = "Pixel FED size";
const char * kPixelClusters               = "Pixel Clusters"; 

const char * kRecoVerticesHLT             = "HLT primary vertices"; 
const char * kRecoVerticesOffline0        = "RECO primary vertices, Offline0";
const char * kECALActivity                = "ECAL Actvity clust";
const char * kECALActivityEcalIso         = "ECAL Actvity EIso";
const char * kECALActivityHcalIso         = "ECAL Activity HIso";
const char * kECALActivityTrackIso          = "ECAL Activity TIso";
const char * kECALActivityR9              = "ECAL Activity R9 spike cleaning";
const char * kECALActivityR9ID          = "ECAL Activity R9ID";
const char * kECALActivityHoverEH          = "ECAL Activity H for HoverE";
