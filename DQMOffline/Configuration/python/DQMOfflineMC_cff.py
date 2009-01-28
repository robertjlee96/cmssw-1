import FWCore.ParameterSet.Config as cms

from DQMOffline.Configuration.DQMOffline_cff import *

siStripFEDCheck.RawDataTag = 'rawDataCollector'
SiPixelHLTSource.RawInput = 'rawDataCollector'
dqmCSCClient.InputObjects = 'rawDataCollector'
cscDQMEvF.InputObjects = 'rawDataCollector'
ecalBarrelHltTask.FEDRawDataCollection = 'rawDataCollector'
ecalBarrelSelectiveReadoutTask.FEDRawDataCollection = 'rawDataCollector'
ecalEndcapHltTask.FEDRawDataCollection = 'rawDataCollector'
ecalEndcapSelectiveReadoutTask.FEDRawDataCollection = 'rawDataCollector'
dtDataIntegrityUnpacker.inputLabel = cms.untracked.InputTag('rawDataCollector')
hcalMonitor.FEDRawDataCollection = cms.untracked.InputTag('rawDataCollector')

