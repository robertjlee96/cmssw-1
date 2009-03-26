import FWCore.ParameterSet.Config as cms

# Set the HLT paths
import HLTrigger.HLTfilters.hltHighLevel_cfi
ALCARECOSiStripCalZeroBiasHLT = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
    andOr = True, # choose logical OR between Triggerbits
#    HLTPaths = [
#        #SiStripCalZeroBias
#        "HLT_ZeroBias",
#        #Random Trigger for Cosmic Runs
#        'RandomPath'
#        ],
    eventSetupPathsKey='SiStripCalZeroBias',
    throw = False # tolerate triggers stated above, but not available
)

# Include masking only from Cabling and O2O
import CalibTracker.SiStripESProducers.SiStripQualityESProducer_cfi
siStripQualityESProducerUnbiased = CalibTracker.SiStripESProducers.SiStripQualityESProducer_cfi.siStripQualityESProducer.clone()
siStripQualityESProducerUnbiased.appendToDataLabel = cms.string('unbiased')
siStripQualityESProducerUnbiased.ListOfRecordToMerge = cms.VPSet(
    cms.PSet(
        record = cms.string( 'SiStripDetCablingRcd' ), # bad components from cabling
        tag = cms.string( '' )
    ),
    cms.PSet(
        record = cms.string( 'SiStripBadChannelRcd' ), # bad components from O2O
        tag = cms.string( '' )
    )
)


# Clusterizer #
import RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi 
calZeroBiasClusters = RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi.siStripClusters.clone()
calZeroBiasClusters.QualityLabel = 'unbiased'

# SiStripQuality (only to test the different data labels)#
qualityStatistics = cms.EDFilter("SiStripQualityStatistics",
    TkMapFileName = cms.untracked.string(''),
    dataLabel = cms.untracked.string('unbiased')
)

# Sequence #
seqALCARECOSiStripCalZeroBias = cms.Sequence(ALCARECOSiStripCalZeroBiasHLT*calZeroBiasClusters)
