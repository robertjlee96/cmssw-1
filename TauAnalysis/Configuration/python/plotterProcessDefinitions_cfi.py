import FWCore.ParameterSet.Config as cms
import copy


#--------------------------------------------------------------------------------
# define data plotter PSet
#--------------------------------------------------------------------------------

process_Data = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Data')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Data'),
        legendEntry = cms.string('Data'),
        type = cms.string('Data') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

#--------------------------------------------------------------------------------
# define names of different Monte Carlo processes
#--------------------------------------------------------------------------------

# Minimum bias MC
process_MinBias = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('MinBias')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/MinBias'),
        legendEntry = cms.string('min bias'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# Z --> tau+ tau- generated with Pythia + Tauola (all decay modes)
process_Ztautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Ztautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Ztautau'),
        legendEntry = cms.string('Z #rightarrow #tau^{+} #tau^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# Z --> tau+ tau- generated by replacing muons reconstructed in selected Z --> mu+ mu- events
# by simulated tau decay products and re-reconstructing the event
process_Ztautau_from_selZmumu = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Ztautau_from_selZmumu')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Ztautau_from_selZmumu'),
        legendEntry = cms.string('Z #rightarrow #tau^{+} #tau^{-} (from sel. Z #rightarrow #mu^{+} #mu^{-})'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# Z --> mu+ mu- generated with Pythia
process_Zmumu = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Zmumu')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Zmumu'),
        legendEntry = cms.string('Z #rightarrow #mu^{+} #mu^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# Z --> e+ e- generated with Pythia
process_Zee = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Zee')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Zee'),
        legendEntry = cms.string('Z #rightarrow e^{+} e^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# Z + jets generated with Madgraph
process_ZplusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('ZplusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/ZplusJets'),
        legendEntry = cms.string('Z + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

process_ZeePlusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('ZeePlusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/ZeePlusJets'),
        legendEntry = cms.string('Z #rightarrow e^{+} e^{-} + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

process_ZmumuPlusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('ZmumuPlusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/ZmumuPlusJets'),
        legendEntry = cms.string('Z #rightarrow #mu^{+} #mu^{-} + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

process_ZtautauPlusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('ZtautauPlusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/ZtautauPlusJets'),
        legendEntry = cms.string('Z #rightarrow #tau^{+} #tau^{-} + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# W + jets generated with Madgraph
process_WplusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('WplusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/WplusJets'),
        legendEntry = cms.string('W + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# W -> tau nu
process_Wtaunu = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Wtaunu')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Wtaunu'),
        legendEntry = cms.string('W -> tau nu'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# W -> mu nu
process_Wmunu = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Wmunu')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Wmunu'),
        legendEntry = cms.string('W -> mu nu'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)


# W -> e nu
process_Wenu = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Wenu')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Wenu'),
        legendEntry = cms.string('W -> e nu'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# W/Z + c cbar/b bbar
process_Vqq = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('Vqq')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/Vqq'),
        legendEntry = cms.string('W/Z + c#bar{c}/b#bar{b} jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# QCD generated specifically for W --> tau nu
# (preselected by requiring charged particle of Pt > 15 GeV on generator level)
process_qcd_W = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('qcd_W')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/qcd_W'),
        legendEntry = cms.string('QCD pt 15'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# gamma + jets 
process_gammaPlusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('gammaPlusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/gammaPlusJets'),
        legendEntry = cms.string('#gamma + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# WW/WZ/ZZ 
process_VV = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('VV')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/VV'),
        legendEntry = cms.string('WW/WZ/ZZ'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# TT + jets 
process_TTplusJets = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('TTplusJets')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/TTplusJets'),
        legendEntry = cms.string('TT + jets'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# electron enriched QCD generated with Pythia
# in the range 20 GeV < Pt(hat) < 30 GeV
process_QCD_EMenriched_Pt20to30 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('QCD_EMenriched_Pt20to30')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/QCD_EMenriched_Pt20to30'),
        legendEntry = cms.string('eQCD 20 < #hat{P}_T < 30 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# electron enriched QCD generated with Pythia
# in the range 30 GeV < Pt(hat) < 80 GeV
process_QCD_EMenriched_Pt30to80 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('QCD_EMenriched_Pt30to80')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/QCD_EMenriched_Pt30to80'),
        legendEntry = cms.string('eQCD 30 < #hat{P}_T < 80 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# electron enriched QCD generated with Pythia
# in the range 80 GeV < Pt(hat) < 170 GeV
process_QCD_EMenriched_Pt80to170 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('QCD_EMenriched_Pt80to170')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/QCD_EMenriched_Pt80to170'),
        legendEntry = cms.string('eQCD 80 < #hat{P}_T < 170 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# electron from b/c QCD generated with Pythia
# in the range 20 GeV < Pt(hat) < 30 GeV
process_QCD_BCtoE_Pt20to30 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('QCD_BCtoE_Pt20to30')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/QCD_BCtoE_Pt20to30'),
        legendEntry = cms.string('eQCD 20 < #hat{P}_T < 30 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# electron from b/c QCD generated with Pythia
# in the range 30 GeV < Pt(hat) < 80 GeV
process_QCD_BCtoE_Pt30to80 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('QCD_BCtoE_Pt30to80')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/QCD_BCtoE_Pt30to80'),
        legendEntry = cms.string('eQCD 30 < #hat{P}_T < 80 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# electron from b/c QCD generated with Pythia
# in the range 80 GeV < Pt(hat) < 170 GeV
process_QCD_BCtoE_Pt80to170 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('QCD_BCtoE_Pt80to170')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/QCD_BCtoE_Pt80to170'),
        legendEntry = cms.string('eQCD 80 < #hat{P}_T < 170 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# muon enriched QCD generated with Pythia
process_InclusivePPmuX = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('InclusivePPmuX')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/InclusivePPmuX'),
        legendEntry = cms.string('pp#muX'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
)

# muon enriched QCD generated with Pythia
# in the range Pt(hat) > 20 GeV
process_PPmuXptGt20 = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('PPmuXptGt20')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('harvested/PPmuXptGt20'),
        legendEntry = cms.string('pp#muX #hat{P}_T > 20 GeV'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

# MSSM Higgs A/H --> tau+ tau-
process_AH115bb_tautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('AH115bb_tautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('AH115bb_tautau'),
        legendEntry = cms.string('bb + H(115) to #tau^{+} #tau^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

process_AH160bb_tautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('AH160bb_tautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('AH160bb_tautau'),
        legendEntry = cms.string('bb + H(160) to #tau^{+} #tau^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

process_AHbb_tautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('AHbb_tautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('AHbb_tautau'),
        legendEntry = cms.string('bb + A/H to #tau^{+} #tau^{-}'),
        type = cms.string('bsmMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

process_AHbb120_tautau = copy.deepcopy(process_AHbb_tautau)
process_AHbb120_tautau.config_dqmFileLoader.dqmDirectory_store = cms.string('AHbb120_tautau')
process_AHbb120_tautau.config_dqmHistPlotter.dqmDirectory = cms.string('AHbb120_tautau')

process_AH115_tautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('AH115_tautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('AH115_tautau'),
        legendEntry = cms.string('gg-H(115) to #tau^{+} #tau^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

process_AH160_tautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('AH160_tautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('AH160_tautau'),
        legendEntry = cms.string('gg-H(160) to #tau^{+} #tau^{-}'),
        type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

process_AH_tautau = cms.PSet(
    config_dqmFileLoader = cms.PSet(
        inputFileNames = cms.vstring(''),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('AH_tautau')
    ),
    config_dqmHistPlotter = cms.PSet(
        dqmDirectory = cms.string('AH_tautau'),
        legendEntry = cms.string('gg-A/H to #tau^{+} #tau^{-}'),
        type = cms.string('bsmMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    ) 
)

process_AH120_tautau = copy.deepcopy(process_AH_tautau)
process_AH120_tautau.config_dqmFileLoader.dqmDirectory_store = cms.string('AH120_tautau')
process_AH120_tautau.config_dqmHistPlotter.dqmDirectory = cms.string('AH120_tautau')
