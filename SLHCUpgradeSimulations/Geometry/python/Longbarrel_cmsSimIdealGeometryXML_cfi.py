import FWCore.ParameterSet.Config as cms

### Modifed by Pratima Jindal,Purdue University Calumet, July 2009 to include files for Phase 1 geometry

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('SLHCUpgradeSimulations/Geometry/data/longbarrel/materials.xml', 
        'Geometry/CMSCommonData/data/rotations.xml', 
        'Geometry/CMSCommonData/data/normal/cmsextent.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMother.xml', 
        'Geometry/CMSCommonData/data/cmsTracker.xml', 
        'Geometry/CMSCommonData/data/caloBase.xml', 
        'Geometry/CMSCommonData/data/cmsCalo.xml', 
        'Geometry/CMSCommonData/data/muonBase.xml', 
        'Geometry/CMSCommonData/data/cmsMuon.xml', 
        'Geometry/CMSCommonData/data/mgnt.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/beampipe.xml', 
        'Geometry/CMSCommonData/data/cmsBeam.xml', 
        'Geometry/CMSCommonData/data/muonMB.xml', 
        'Geometry/CMSCommonData/data/muonMagnet.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdMaterials.xml', 
        'Geometry/TrackerCommonData/data/pixfwdCommon.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdCylinder.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwd.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdDisks.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdInnerDisk1.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdInnerDisk2.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdInnerDisk3.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdOuterDisk1.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdOuterDisk2.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdOuterDisk3.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdblade1.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdblade2.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdblade3.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarmaterial.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladder.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull0.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull1.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull2.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull3.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarlayer.xml', 
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayer0.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayer1.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayer2.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayer3.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack0.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack0.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack1.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack1.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack2.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack2.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack3.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack3.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack4.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack4.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack5.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack5.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack6.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack6.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack7.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack7.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack8.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack8.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarladderstack9.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbarlayerstack9.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/pixbar.xml',
        'Geometry/TrackerCommonData/data/trackermaterial.xml', 
        'Geometry/TrackerCommonData/data/tracker.xml', 
        'Geometry/TrackerCommonData/data/trackerpixbar.xml', 
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/trackerpixfwd.xml', 
        'Geometry/TrackerCommonData/data/trackerother.xml', 
        'Geometry/EcalCommonData/data/eregalgo.xml', 
        'Geometry/EcalCommonData/data/ebalgo.xml', 
        'Geometry/EcalCommonData/data/ebcon.xml', 
        'Geometry/EcalCommonData/data/ebrot.xml', 
        'Geometry/EcalCommonData/data/eecon.xml', 
        'Geometry/EcalCommonData/data/eefixed.xml', 
        'Geometry/EcalCommonData/data/eehier.xml', 
        'Geometry/EcalCommonData/data/eealgo.xml', 
        'Geometry/EcalCommonData/data/escon.xml', 
        'Geometry/EcalCommonData/data/esalgo.xml', 
        'Geometry/EcalCommonData/data/eeF.xml', 
        'Geometry/EcalCommonData/data/eeB.xml', 
        'Geometry/HcalCommonData/data/hcalrotations.xml', 
        'Geometry/HcalCommonData/data/hcalalgo.xml', 
        'Geometry/HcalCommonData/data/hcalbarrelalgo.xml', 
        'Geometry/HcalCommonData/data/hcalendcapalgo.xml', 
        'Geometry/HcalCommonData/data/hcalouteralgo.xml', 
        'Geometry/HcalCommonData/data/hcalforwardalgo.xml', 
	'Geometry/HcalCommonData/data/average/hcalforwardmaterial.xml',
        'Geometry/MuonCommonData/data/mbCommon.xml', 
        'Geometry/MuonCommonData/data/mb1.xml', 
        'Geometry/MuonCommonData/data/mb2.xml', 
        'Geometry/MuonCommonData/data/mb3.xml', 
        'Geometry/MuonCommonData/data/mb4.xml', 
        'Geometry/MuonCommonData/data/muonYoke.xml', 
        'Geometry/MuonCommonData/data/mf.xml', 
        'Geometry/ForwardCommonData/data/forward.xml', 
	'Geometry/ForwardCommonData/data/bundle/forwardshield.xml',
        'Geometry/ForwardCommonData/data/brmrotations.xml', 
        'Geometry/ForwardCommonData/data/brm.xml', 
        'Geometry/ForwardCommonData/data/totemMaterials.xml', 
        'Geometry/ForwardCommonData/data/totemRotations.xml', 
        'Geometry/ForwardCommonData/data/totemt1.xml', 
        'Geometry/ForwardCommonData/data/totemt2.xml', 
        'Geometry/ForwardCommonData/data/ionpump.xml', 
        'Geometry/MuonCommonData/data/muonNumbering.xml', 
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/trackerStructureTopology.xml', 
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/trackersens.xml', 
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/trackerRecoMaterial.xml', 
        'Geometry/EcalSimData/data/ecalsens.xml', 
	'Geometry/HcalCommonData/data/hcalsenspmf.xml',
	'Geometry/HcalSimData/data/hf.xml',
	'Geometry/HcalSimData/data/hfpmt.xml',
	'Geometry/HcalSimData/data/hffibrebundle.xml',
        'Geometry/HcalSimData/data/CaloUtil.xml', 
        'Geometry/MuonSimData/data/muonSens.xml', 
        'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml', 
        'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml', 
        'Geometry/CSCGeometryBuilder/data/cscSpecs.xml', 
        'Geometry/RPCGeometryBuilder/data/RPCSpecs.xml', 
        'Geometry/ForwardCommonData/data/brmsens.xml', 
        'Geometry/HcalSimData/data/HcalProdCuts.xml', 
        'Geometry/EcalSimData/data/EcalProdCuts.xml', 
	'Geometry/EcalSimData/data/ESProdCuts.xml',
        'SLHCUpgradeSimulations/Geometry/data/longbarrel/trackerProdCuts.xml', 
        'Geometry/TrackerSimData/data/trackerProdCutsBEAM.xml', 
        'Geometry/MuonSimData/data/muonProdCuts.xml',
        'Geometry/ForwardSimData/data/ForwardShieldProdCuts.xml',
        'Geometry/CMSCommonData/data/FieldParameters.xml'),
    rootNodeName = cms.string('cms:OCMS')
)


