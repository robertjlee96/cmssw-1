import FWCore.ParameterSet.Config as cms

from DQM.EcalBarrelMonitorClient.PNIntegrityClient_cfi import ecalPnIntegrityClient
from DQM.EcalBarrelMonitorClient.LaserClient_cfi import ecalLaserClient
from DQM.EcalBarrelMonitorClient.LedClient_cfi import ecalLedClient
from DQM.EcalBarrelMonitorClient.TestPulseClient_cfi import ecalTestPulseClient
from DQM.EcalBarrelMonitorClient.PedestalClient_cfi import ecalPedestalClient

laserWavelengths = [3]
ledWavelengths = [1, 2]
testPulseMGPAGains = [12]
testPulseMGPAGainsPN = [16]
pedestalMGPAGains = []
pedestalMGPAGainsPN = []
activeSources = ['Laser', 'Led', 'TestPulse']

ecalCalibrationSummaryClient = cms.untracked.PSet(
    laserWavelengths = cms.untracked.vint32(laserWavelengths),
    ledWavelengths = cms.untracked.vint32(ledWavelengths),
    testPulseMGPAGains = cms.untracked.vint32(testPulseMGPAGains),
    testPulseMGPAGainsPN = cms.untracked.vint32(testPulseMGPAGainsPN),
    pedestalMGPAGains = cms.untracked.vint32(pedestalMGPAGains),
    pedestalMGPAGainsPN = cms.untracked.vint32(pedestalMGPAGainsPN),
    activeSources = cms.untracked.vstring(activeSources),
    sources = cms.untracked.PSet(
        Laser = ecalLaserClient.MEs.QualitySummary,
        LaserPN = ecalLaserClient.MEs.PNQualitySummary,
        Led = ecalLedClient.MEs.QualitySummary,
        LedPN = ecalLedClient.MEs.PNQualitySummary,
        Pedestal = ecalPedestalClient.MEs.QualitySummary,        
        PedestalPN = ecalPedestalClient.MEs.PNQualitySummary,
        PNIntegrity = ecalPnIntegrityClient.MEs.QualitySummary,
        TestPulse = ecalTestPulseClient.MEs.QualitySummary,        
        TestPulsePN = ecalTestPulseClient.MEs.PNQualitySummary
    ),
    MEs = cms.untracked.PSet(
        PNQualitySummary = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sSummaryClient/%(prefix)s PN global quality'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('MEM2P'),
            btype = cms.untracked.string('Crystal'),
            description = cms.untracked.string('Summary of the calibration data quality for the PN diodes. Channel is red if it is red in any of the Laser 3, Led 1 and 2, Pedestal gain 12, and Test Pulse gain 12 quality summary.')
        ),
        QualitySummary = cms.untracked.PSet(
            path = cms.untracked.string('%(subdet)s/%(prefix)sSummaryClient/%(prefix)s global calibration quality%(suffix)s'),
            kind = cms.untracked.string('TH2F'),
            otype = cms.untracked.string('Ecal3P'),
            btype = cms.untracked.string('SuperCrystal'),
            description = cms.untracked.string('Summary of the calibration data quality. Channel is red if it is red in any of the Laser 3, Led 1 and 2, Pedestal gain 12, and Test Pulse gain 12 quality summary.')
        )
    )
)
