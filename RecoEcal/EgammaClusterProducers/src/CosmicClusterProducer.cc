// C/C++ headers
#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// Reconstruction Classes
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

// Geometry
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"

// EgammaCoreTools
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "RecoEcal/EgammaCoreTools/interface/ClusterShapeAlgo.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"

// Class header file
#include "RecoEcal/EgammaClusterProducers/interface/CosmicClusterProducer.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"


CosmicClusterProducer::CosmicClusterProducer(const edm::ParameterSet& ps)
{
  // The verbosity level
  std::string verbosityString = ps.getParameter<std::string>("VerbosityLevel");
  if      (verbosityString == "DEBUG")   verbosity = CosmicClusterAlgo::pDEBUG;
  else if (verbosityString == "WARNING") verbosity = CosmicClusterAlgo::pWARNING;
  else if (verbosityString == "INFO")    verbosity = CosmicClusterAlgo::pINFO;
  else                                   verbosity = CosmicClusterAlgo::pERROR;

  //TEMP JHAUPT 4-27
  maskedChannels_ = ps.getUntrackedParameter<std::vector<int> >("maskedChannels");//TEMP JHAUPT 4-27
  
  // Parameters to identify the hit collections
  barrelHitProducer_   = ps.getParameter<std::string>("barrelHitProducer");
  endcapHitProducer_   = ps.getParameter<std::string>("endcapHitProducer");
  barrelHitCollection_ = ps.getParameter<std::string>("barrelHitCollection");
  endcapHitCollection_ = ps.getParameter<std::string>("endcapHitCollection");

  // The names of the produced cluster collections
  barrelClusterCollection_  = ps.getParameter<std::string>("barrelClusterCollection");
  endcapClusterCollection_  = ps.getParameter<std::string>("endcapClusterCollection");

  // Island algorithm parameters
  double barrelSeedThreshold   = ps.getParameter<double>("BarrelSeedThr");
  double barrelSingleThreshold = ps.getParameter<double>("BarrelSingleThr");
  double barrelSecondThreshold = ps.getParameter<double>("BarrelSecondThr");
  double endcapSeedThreshold   = ps.getParameter<double>("EndcapSeedThr");
  double endcapSingleThreshold = ps.getParameter<double>("EndcapSingleThr");
  double endcapSecondThreshold = ps.getParameter<double>("EndcapSecondThr");
  

  // Parameters for the position calculation:
  std::map<std::string,double> providedParameters;
  providedParameters.insert(std::make_pair("LogWeighted",ps.getParameter<bool>("posCalc_logweight")));
  providedParameters.insert(std::make_pair("T0_barl",ps.getParameter<double>("posCalc_t0_barl")));
  providedParameters.insert(std::make_pair("T0_endc",ps.getParameter<double>("posCalc_t0_endc")));
  providedParameters.insert(std::make_pair("T0_endcPresh",ps.getParameter<double>("posCalc_t0_endcPresh")));
  providedParameters.insert(std::make_pair("W0",ps.getParameter<double>("posCalc_w0")));
  providedParameters.insert(std::make_pair("X0",ps.getParameter<double>("posCalc_x0")));
  posCalculator_ = PositionCalc(providedParameters);
  shapeAlgo_ = ClusterShapeAlgo(providedParameters);

  clustershapecollectionEB_ = ps.getParameter<std::string>("clustershapecollectionEB");
  clustershapecollectionEE_ = ps.getParameter<std::string>("clustershapecollectionEE");

  //AssociationMap
  barrelClusterShapeAssociation_ = ps.getParameter<std::string>("barrelShapeAssociation");
  endcapClusterShapeAssociation_ = ps.getParameter<std::string>("endcapShapeAssociation");

  // Produces a collection of barrel and a collection of endcap clusters

  produces< reco::ClusterShapeCollection>(clustershapecollectionEE_);
  produces< reco::BasicClusterCollection >(endcapClusterCollection_);
  produces< reco::ClusterShapeCollection>(clustershapecollectionEB_);
  produces< reco::BasicClusterCollection >(barrelClusterCollection_);
  produces< reco::BasicClusterShapeAssociationCollection >(barrelClusterShapeAssociation_);
  produces< reco::BasicClusterShapeAssociationCollection >(endcapClusterShapeAssociation_);

  island_p = new CosmicClusterAlgo(barrelSeedThreshold, barrelSingleThreshold, barrelSecondThreshold, endcapSeedThreshold, endcapSingleThreshold, endcapSecondThreshold, posCalculator_,verbosity);

  nEvt_ = 0;
}


CosmicClusterProducer::~CosmicClusterProducer()
{
  delete island_p;
}
 

void CosmicClusterProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  clusterizeECALPart(evt, es, endcapHitProducer_, endcapHitCollection_, endcapClusterCollection_, endcapClusterShapeAssociation_, CosmicClusterAlgo::endcap); 
  clusterizeECALPart(evt, es, barrelHitProducer_, barrelHitCollection_, barrelClusterCollection_, barrelClusterShapeAssociation_, CosmicClusterAlgo::barrel);
  nEvt_++;
}


const EcalRecHitCollection * CosmicClusterProducer::getCollection(edm::Event& evt,
                                                                  const std::string& hitProducer_,
                                                                  const std::string& hitCollection_)
{
  edm::Handle<EcalRecHitCollection> rhcHandle;
  try
    {
      evt.getByLabel(hitProducer_, hitCollection_, rhcHandle);
      if (!(rhcHandle.isValid())) 
	{
	  std::cout << "could not get a handle on the EcalRecHitCollection!" << std::endl;
	  return 0;
	}
    }
  catch ( cms::Exception& ex ) 
    {
      edm::LogError("CosmicClusterProducerError") << "Error! can't get the product " << hitCollection_.c_str() ;
      return 0;
    }
  return rhcHandle.product();
}


void CosmicClusterProducer::clusterizeECALPart(edm::Event &evt, const edm::EventSetup &es,
                                               const std::string& hitProducer,
                                               const std::string& hitCollection,
                                               const std::string& clusterCollection,
					       const std::string& clusterShapeAssociation,
                                               const CosmicClusterAlgo::EcalPart& ecalPart)
{
  // get the hit collection from the event:

  const EcalRecHitCollection *hitCollection_p = getCollection(evt, hitProducer, hitCollection);

  // get the geometry and topology from the event setup:
  edm::ESHandle<CaloGeometry> geoHandle;
  es.get<CaloGeometryRecord>().get(geoHandle);

  const CaloSubdetectorGeometry *geometry_p;
  CaloSubdetectorTopology *topology_p;

  std::string clustershapetag;
  if (ecalPart == CosmicClusterAlgo::barrel) 
    {
      geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
      topology_p = new EcalBarrelTopology(geoHandle);
    }
  else
    {
      geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
      topology_p = new EcalEndcapTopology(geoHandle); 
   }

  const CaloSubdetectorGeometry *geometryES_p;
  geometryES_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  
  // Intercalib constants
  edm::ESHandle<EcalIntercalibConstants> pIcal;
  es.get<EcalIntercalibConstantsRcd>().get(pIcal);
  const EcalIntercalibConstants* ical = pIcal.product();
  const EcalIntercalibConstantMap& icalMap=ical->getMap();

  // Run the clusterization algorithm:
  reco::BasicClusterCollection clusters;
  clusters = island_p->makeClusters(hitCollection_p, geometry_p, topology_p, geometryES_p,  ecalPart, maskedChannels_, icalMap);
  
  //Create associated ClusterShape objects.
  std::vector <reco::ClusterShape> ClusVec;
 
  for (int erg=0;erg<int(clusters.size());++erg){
    reco::ClusterShape TestShape = shapeAlgo_.Calculate(clusters[erg],hitCollection_p,geometry_p,topology_p);
    ClusVec.push_back(TestShape);
  }
  
  //Put clustershapes in event, but retain a Handle on them.
  std::auto_ptr< reco::ClusterShapeCollection> clustersshapes_p(new reco::ClusterShapeCollection);
  clustersshapes_p->assign(ClusVec.begin(), ClusVec.end());
  edm::OrphanHandle<reco::ClusterShapeCollection> clusHandle; 
  if (ecalPart == CosmicClusterAlgo::barrel) 
    clusHandle= evt.put(clustersshapes_p, clustershapecollectionEB_);
  else
    clusHandle= evt.put(clustersshapes_p, clustershapecollectionEE_);
  
  // create an auto_ptr to a BasicClusterCollection, copy the barrel clusters into it and put in the Event:
  std::auto_ptr< reco::BasicClusterCollection > clusters_p(new reco::BasicClusterCollection);
  clusters_p->assign(clusters.begin(), clusters.end());
  edm::OrphanHandle<reco::BasicClusterCollection> bccHandle;
  
  if (ecalPart == CosmicClusterAlgo::barrel) 
    bccHandle = evt.put(clusters_p, barrelClusterCollection_);
  else
    bccHandle = evt.put(clusters_p, endcapClusterCollection_);

  
  // BasicClusterShapeAssociationMap 
  std::auto_ptr<reco::BasicClusterShapeAssociationCollection> shapeAssocs_p(new reco::BasicClusterShapeAssociationCollection);
  for (unsigned int i = 0; i < clusHandle->size(); i++){
    shapeAssocs_p->insert(edm::Ref<reco::BasicClusterCollection>(bccHandle,i),edm::Ref<reco::ClusterShapeCollection>(clusHandle,i));
  }  
  evt.put(shapeAssocs_p,clusterShapeAssociation);
  
  delete topology_p;
}
