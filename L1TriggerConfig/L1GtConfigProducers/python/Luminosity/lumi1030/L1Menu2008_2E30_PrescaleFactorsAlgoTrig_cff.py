import FWCore.ParameterSet.Config as cms

from L1TriggerConfig.L1GtConfigProducers.l1GtPrescaleFactorsAlgoTrig_cfi import *
l1GtPrescaleFactorsAlgoTrig.PrescaleFactorsSet = cms.VPSet(cms.PSet(
    PrescaleFactors = cms.vint32(300000, 300000, 10, 1, 10, 
        10, 1, 100, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 10, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 80, 
        80, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 
        1, 150, 80, 1, 20, 
        20, 1, 1, 1, 1, 
        1, 1, 1, 10, 1, 
        1, 100, 1, 12, 10, 
        20, 20, 20, 20, 1, 
        1, 1, 1, 1, 1, 
        1, 1, 1)
))

