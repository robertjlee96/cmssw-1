import FWCore.ParameterSet.Config as cms

# L1 Emulator sequence running on unpacked data
#    each emulator run on the unpacked data of the previous (in the hardware chain) subsystem 
#
#    Order if using the standard sequence
#    RawToDigi,ValL1Emulator
#
# V.M. Ghete 2009-11-15

# ECAL TPG sequence
import SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cfi
valEcalTriggerPrimitiveDigis = SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cfi.simEcalTriggerPrimitiveDigis.clone()
#
valEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
valEcalTriggerPrimitiveDigis.InstanceEB = 'ebDigis'
valEcalTriggerPrimitiveDigis.InstanceEE = 'eeDigis'

# HCAL TPG sequence
from SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff import *
valHcalTriggerPrimitiveDigis = SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cfi.simHcalTriggerPrimitiveDigis.clone()
#
valHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag(cms.InputTag('hcalDigis'),cms.InputTag('hcalDigis'))


# HCAL Tech Trig sequence
import SimCalorimetry.HcalTrigPrimProducers.hcalTTPDigis_cfi
valHcalTTPDigis = SimCalorimetry.HcalTrigPrimProducers.hcalTTPDigis_cfi.simHcalTTPDigis.clone()
#
valHcalTTPDigis.HFDigiCollection = cms.InputTag('hcalDigis')


# RCT emulator
import L1Trigger.RegionalCaloTrigger.rctDigis_cfi
valRctDigis = L1Trigger.RegionalCaloTrigger.rctDigis_cfi.rctDigis.clone()
#
valRctDigis.ecalDigis = cms.VInputTag(cms.InputTag('ecalDigis:EcalTriggerPrimitives'))
valRctDigis.hcalDigis = cms.VInputTag(cms.InputTag('hcalDigis'))


# GCT emulator
# RCT data used as input for GCT emulator are part of the GCT FED
import L1Trigger.GlobalCaloTrigger.gctDigis_cfi
valGctDigis = L1Trigger.GlobalCaloTrigger.gctDigis_cfi.gctDigis.clone()
#
valGctDigis.inputLabel = 'gctDigis'
valGctDigis.preSamples = cms.uint32(0)
valGctDigis.postSamples = cms.uint32(0)


# DT TP emulator
from L1Trigger.DTTrigger.dtTriggerPrimitiveDigis_cfi import *
valDtTriggerPrimitiveDigis = L1Trigger.DTTrigger.dtTriggerPrimitiveDigis_cfi.dtTriggerPrimitiveDigis.clone()


# CSC TP emulator
from L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi import *
valCscTriggerPrimitiveDigis = L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi.cscTriggerPrimitiveDigis.clone()
#
valCscTriggerPrimitiveDigis.CSCComparatorDigiProducer = cms.InputTag('muonCSCDigis',
                                                                     'MuonCSCComparatorDigi')
valCscTriggerPrimitiveDigis.CSCWireDigiProducer = cms.InputTag('muonCSCDigis',
                                                               'MuonCSCWireDigi')

# CSC Track Finder - digi track generation 
# currently used also by DT TF to generate CSCTF stubs
import L1Trigger.CSCTrackFinder.csctfTrackDigis_cfi
valCsctfTrackDigis = L1Trigger.CSCTrackFinder.csctfTrackDigis_cfi.csctfTrackDigis.clone()
#
valCsctfTrackDigis.SectorReceiverInput = 'csctfDigis'
valCsctfTrackDigis.DTproducer = 'dttfDigis'


# DT Track Finder emulator
# currently generates CSCTF stubs by running CSCTF emulator
import L1Trigger.DTTrackFinder.dttfDigis_cfi
valDttfDigis = L1Trigger.DTTrackFinder.dttfDigis_cfi.dttfDigis.clone()
#
valDttfDigis.DTDigi_Source = 'dttfDigis'
valDttfDigis.CSCStub_Source = 'valCsctfTrackDigis'


# CSC Track Finder emulator
import L1Trigger.CSCTrackFinder.csctfDigis_cfi
valCsctfDigis = L1Trigger.CSCTrackFinder.csctfDigis_cfi.csctfDigis.clone()
#
valCsctfDigis.CSCTrackProducer = 'valCsctfTrackDigis'



# RPC PAC Trigger emulator
from L1Trigger.RPCTrigger.rpcTriggerDigis_cff import *
valRpcTriggerDigis = L1Trigger.RPCTrigger.rpcTriggerDigis_cff.rpcTriggerDigis.clone()
#
valRpcTriggerDigis.label = 'muonRPCDigis'


# Global Muon Trigger emulator - input from common GMT/GT unpacker (gtDigis) 
import L1Trigger.GlobalMuonTrigger.gmtDigis_cfi
valGmtDigis = L1Trigger.GlobalMuonTrigger.gmtDigis_cfi.gmtDigis.clone()
#
valGmtDigis.DTCandidates = cms.InputTag('gtDigis','DT')
valGmtDigis.CSCCandidates = cms.InputTag('gtDigis','CSC')
valGmtDigis.RPCbCandidates = cms.InputTag('gtDigis','RPCb')
valGmtDigis.RPCfCandidates = cms.InputTag('gtDigis','RPCf')
valGmtDigis.MipIsoData = 'gctDigis'

# producers for technical triggers 
#


# BSC Technical Trigger - no data to run on


# RPC Technical Trigger
import L1Trigger.RPCTechnicalTrigger.rpcTechnicalTrigger_cfi
valRpcTechTrigDigis = L1Trigger.RPCTechnicalTrigger.rpcTechnicalTrigger_cfi.rpcTechnicalTrigger.clone()

# HCAL Technical Trigger
import SimCalorimetry.HcalTrigPrimProducers.hcalTTPRecord_cfi
valHcalTechTrigDigis = SimCalorimetry.HcalTrigPrimProducers.hcalTTPRecord_cfi.simHcalTTPRecord.clone()



# Global Trigger emulator
import L1Trigger.GlobalTrigger.gtDigis_cfi
valGtDigis = L1Trigger.GlobalTrigger.gtDigis_cfi.gtDigis.clone()
#
valGtDigis.GmtInputTag = 'gtDigis'
valGtDigis.GctInputTag = 'gctDigis'
valGtDigis.TechnicalTriggersInputTags = cms.VInputTag(
                                                    cms.InputTag('valRpcTechTrigDigis'),
                                                    cms.InputTag('valHcalTechTrigDigis')                                                    
                                                    )


# L1 Trigger sequences
ValL1MuTriggerPrimitives = cms.Sequence(valCscTriggerPrimitiveDigis+valDtTriggerPrimitiveDigis)
ValL1MuTrackFinders = cms.Sequence(valCsctfTrackDigis*valCsctfDigis*valDttfDigis)

ValL1TechnicalTriggers = cms.Sequence(valRpcTechTrigDigis+valHcalTechTrigDigis)

ValL1Emulator = cms.Sequence(
    valEcalTriggerPrimitiveDigis
    *valHcalTriggerPrimitiveDigis
    *valHcalTTPDigis
    *valRctDigis*valGctDigis
    *ValL1MuTriggerPrimitives*ValL1MuTrackFinders*valRpcTriggerDigis*valGmtDigis
    *ValL1TechnicalTriggers
    *valGtDigis)





