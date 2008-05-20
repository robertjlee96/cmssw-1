import FWCore.ParameterSet.Config as cms

# Magnetic Field
from MagneticField.Engine.volumeBasedMagneticField_cfi import *
# Geometries
from Geometry.CMSCommonData.cmsIdealGeometryXML_cff import *
from Geometry.CommonDetUnit.bareGlobalTrackingGeometry_cfi import *
from Geometry.DTGeometry.dtGeometry_cfi import *
from Geometry.CSCGeometry.cscGeometry_cfi import *
from Geometry.RPCGeometry.rpcGeometry_cfi import *
from RecoMuon.DetLayers.muonDetLayerGeometry_cfi import *
# Old stand alone muon seed producer used priod to 2-X-X
#include "RecoMuon/MuonSeedGenerator/data/standAloneMuonSeeds.cfi"
# New standalone muon producer to be used in 2-X-X
from RecoMuon.MuonSeedGenerator.standAloneMuonSeedProducer_cfi import *
from RecoMuon.MuonSeedGenerator.ptSeedParameterization_cfi import *


