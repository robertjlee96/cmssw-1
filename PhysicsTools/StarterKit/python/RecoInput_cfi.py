import FWCore.ParameterSet.Config as cms

# from 

def RecoInput() : 
 return cms.Source("PoolSource",
                   debugVerbosity = cms.untracked.uint32(200),
                   debugFlag = cms.untracked.bool(True),
                   
                   fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/08D532C8-9C60-DD11-AB1C-000423D99996.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/D48F2870-9B60-DD11-8E69-000423D99E46.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0E2A35B0-9D60-DD11-B8EA-001617C3B6E8.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/184B8DD2-9D60-DD11-919F-000423D991F0.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/2CCA71A6-9D60-DD11-8E50-000423D99F3E.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/48159DDC-9D60-DD11-868E-000423D8FA38.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/48191714-9D60-DD11-AEDF-001617C3B66C.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/50EAD32B-A460-DD11-A064-000423D33970.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/6020B3A8-9D60-DD11-9895-000423D9970C.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/8AE5E5AE-9D60-DD11-88DF-000423D99BF2.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/8E4F413E-A460-DD11-A1C3-001617E30D52.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/90FAC32C-A460-DD11-82FC-000423D9939C.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/B2B2DE69-A460-DD11-B799-000423D991F0.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/BCB4EB5C-A760-DD11-9F8E-000423D6CAF2.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/C80B0129-A460-DD11-A6EE-001617E30D40.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/CA07D963-9D60-DD11-88B5-001617DBD332.root',
       '/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/CEE70B9D-9D60-DD11-AE9D-001617E30D0A.root'
       )
                   )
