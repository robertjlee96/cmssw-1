
import FWCore.ParameterSet.Config as cms

process = cms.Process( "createScenario" )

# source
process.source = cms.Source( "EmptySource" )
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32( 1 )
)


# db output
process.load( "CondCore.DBCommon.CondDBCommon_cfi" )
process.CondDBCommon.connect = 'sqlite_file:Alignments_S.db'

process.PoolDBOutputService = cms.Service( "PoolDBOutputService",
  process.CondDBCommon,
  toPut = cms.VPSet(
    cms.PSet(
      record = cms.string( 'TrackerAlignmentRcd' ),
      tag = cms.string( 'Alignments' )
    ), 
    cms.PSet(
      record = cms.string( 'TrackerAlignmentErrorRcd' ),
      tag = cms.string( 'AlignmentErrors' )
    )
  )
)


# geometry
process.load( "Geometry.CMSCommonData.cmsIdealGeometryXML_cfi" )
process.load( "Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi" )


process.misalignmentProducer = cms.ESProducer("MisalignedTrackerESProducer",

  seed = cms.int32( 123456 ),
  saveToDbase = cms.untracked.bool( True ),
  distribution = cms.string( 'gaussian' ), # or 'fixed'
                                              
  ## TIB+                                              
  TIB2 = cms.PSet(
    dY = cms.double( 0.1 ),
    dX = cms.double( 0.1 ),
    phiXlocal = cms.double( 0.001 ),
    phiYlocal = cms.double( 0.001 ),
    phiZlocal = cms.double( 0.001 )
  ),

  ## TIB-                                              
  TIB1 = cms.PSet(
    dY = cms.double( 0.1 ),
    dX = cms.double( 0.1 ),
    phiXlocal = cms.double( 0.001 ),
    phiYlocal = cms.double( 0.001 ),
    phiZlocal = cms.double( 0.001 )
  ),

  ## TOB+
  TOB2 = cms.PSet(
    dY = cms.double( 0.1 ),
    dX = cms.double( 0.1 ),
    phiXlocal = cms.double( 0.001 ),
    phiYlocal = cms.double( 0.001 ),
    phiZlocal = cms.double( 0.001 )
  ),

  ## TOB-                                
  TOB1 = cms.PSet(
    dY = cms.double( 0.1 ),
    dX = cms.double( 0.1 ),
    phiXlocal = cms.double( 0.001 ),
    phiYlocal = cms.double( 0.001 ),
    phiZlocal = cms.double( 0.001 )
  ),

  ## TEC+                                   
  TEC1 = cms.PSet(

    phiXlocal = cms.double( 0.0 ),
    phiYlocal = cms.double( 0.0 ),
    phiZlocal = cms.double( 0.0 ),
    dX = cms.double( 0.0 ),
    dY = cms.double( 0.0 ),
    
    TECDisk1 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk2 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk3 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk4 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk5 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk6 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk7 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk8 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk9 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    )

  ),


  ## TEC-                                          
  TEC2 = cms.PSet(

    phiXlocal = cms.double( 0.0 ),
    phiYlocal = cms.double( 0.0 ),
    phiZlocal = cms.double( 0.0 ),
    dX = cms.double( 0.0 ),
    dY = cms.double( 0.0 ),

    TECDisk1 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk2 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk3 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),

    TECDisk4 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk5 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk6 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk7 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk8 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    ),
    
    TECDisk9 = cms.PSet(
      dX = cms.double( 0.1 ),
      dY = cms.double( 0.1 ),
      phiZlocal = cms.double( 0.001 )
    )
    
  )
)

process.test = cms.EDAnalyzer( "TestAnalyzer",
  fileName = cms.untracked.string( 'misaligned.root' )
)

process.p1 = cms.Path( process.test )



