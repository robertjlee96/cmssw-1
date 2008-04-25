#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include <DQM/RPCMonitorDigi/interface/RPCMonitorDigi.h>
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <Geometry/RPCGeometry/interface/RPCGeomServ.h>
#include "DQMServices/Core/interface/MonitorElement.h"

using namespace std;

/// Booking of MonitoringElemnt for one RPCDetId (= roll)



int david=0;


std::map<std::string, MonitorElement*> RPCMonitorDigi::bookDetUnitME(RPCDetId & detId) {
  
  // std::cout <<"Booking ME "<<detId<<std::endl; 
  std::map<std::string, MonitorElement*> meMap;
  
  
  std::string regionName;
  std::string ringType;
  if(detId.region() ==  0) {
    regionName="Barrel";
    ringType="Wheel";
  }else{
    ringType="Disk";
    if(detId.region() == -1) regionName="Encap-";
    if(detId.region() ==  1) regionName="Encap+";
  }
  
  char  folder[220];
  sprintf(folder,"RPC/RecHits/%s/%s_%d/station_%d/sector_%d",regionName.c_str(),ringType.c_str(),
	  detId.ring(),detId.station(),detId.sector());
  
  dbe->setCurrentFolder(folder);
  
  //  char layer[128];
  // sprintf(layer ,"layer_roll%d",detId.roll());
  // std::cout<<"\n rool ->"<<layer<<std::endl;
  // sleep(20);
  
  /// Name components common to current RPDDetId  
  string detUnitLabel;
  string layerUnitLabel;
  
  
  RPCGeomServ RPCname(detId);
  std::string nameRoll = RPCname.name();

  detUnitLabel = nameRoll;
  layerUnitLabel = nameRoll;
  
  string meId;
  string meTitle;
  std::stringstream os;
  
  if (dqmexpert) {
    
    os.str("");
    os<<"Occupancy_"<<detUnitLabel;
    meId = os.str();
    
    os.str("");
    os<<"Occupancy_for_"<<detUnitLabel;
    meTitle = os.str();
    os.str("");
    
    meMap[meId] = dbe->book1D(meId, meTitle, 100, 0.5, 100.5);
    
    os<<"BXN_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 21, -10.5, 10.5);
  }
  
  
  if (dqmsuperexpert) {
    
    os.str("");
    os<<"BXN_vs_strip_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book2D(meId, meTitle,  100, 0.5, 100.5, 21, -10.5, 10.5);
    
  }
  
  if (dqmexpert) {
    
    
    os.str("");
    os<<"ClusterSize_"<<detUnitLabel;
    meId=os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 20, 0.5, 20.5);
    
    //
    //  std::cout <<"detUnitLabel:"<<detUnitLabel
    //  <<"\nLayerLabel "<<layerLabel
    //  <<"\nmeId "<<meId
    //  <<"\nmeTitle "<<meTitle
    //  <<std::endl;
    // 
    
    os.str("");
    os<<"NumberOfClusters_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 10, 0.5, 10.5);


  }
  
  if (dqmsuperexpert) {
    
    os.str("");
    os<<"ClusterSize_vs_LowerSrip_"<<detUnitLabel;
    meId =os.str();
    meMap[meId] = dbe->book2D(meId, meTitle, 100, 0.5, 100.5,11, 0.5, 11.5);
    
    os.str("");
    os<<"ClusterSize_vs_HigherStrip_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book2D(meId, meTitle, 100, 0.5, 100.5,11, 0.5, 11.5);
    
    os.str("");
    os<<"ClusterSize_vs_Strip_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book2D(meId, meTitle, 100, 0.5, 100.5,11, 0.5, 11.5);
    
    os.str("");
    os<<"ClusterSize_vs_CentralStrip_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book2D(meId, meTitle, 100, 0.5, 100.5,11, 0.5, 11.5);

    
  }
    
  if (dqmexpert) {
    
    os.str("");
    os<<"NumberOfDigi_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 10, 0.5, 10.5);
        
    os.str("");
    os<<"CrossTalkLow_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 100, 0.5, 100.5);
    
    os.str("");
    os<<"CrossTalkHigh_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 100, 0.5, 100.5);
    
    os.str("");
    os<<"BXWithData_"<<detUnitLabel;
    meId=os.str();
    meTitle =  meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 11, -5.5, 5.5);
    
  }
  
  if (dqmsuperexpert) {
    
    /// RPCRecHits
    os.str("");
    os<<"MissingHits_"<<detUnitLabel;
    meId=os.str();
    meTitle = meId;
    meMap[meId] = dbe->book2D(meId, meTitle, 100, 0, 100, 2, 0.,2.);
    
    os.str("");
    os<<"RecHitX_vs_dx_"<<detUnitLabel;
    meId = os.str();
    meTitle=meId;
    meMap[meId] = dbe->book2D(meId, meTitle, 30, -100, 100,30,10,10);
    
  }
  
  if (dqmexpert) {
    
    os.str("");
    os<<"RecHitXPosition_"<<detUnitLabel;
    meId=os.str();
    meTitle=meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 80, -120, 120);
    
    os.str("");
    os<<"RecHitDX_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle, 30, -10, 10);
    
    os.str("");
    os<<"RecHitCounter_"<<detUnitLabel;
    meId = os.str();
    meTitle = meId;
    meMap[meId] = dbe->book1D(meId, meTitle,100,-0.5,100.5);
    
  }
  
  
  // sprintf(meId,"RecHitYPosition_%s",detUnitLabel);
  // sprintf(meTitle,"RecHit_Yposition_for_%s",layerLabel);
  // meMap[meId] = dbe->book1D(meId, meTitle, 40, -100, 100);
  
  // sprintf(meId,"RecHitDY_%s",detUnitLabel);
  // sprintf(meTitle,"RecHit_DY_for_%s",layerLabel);
  // meMap[meId] = dbe->book1D(meId, meTitle, 30, -10, 10);
  
  // sprintf(meId,"RecHitDXDY_%s",detUnitLabel);
  // sprintf(meTitle,"RecHit_DXDY_for_%s",layerLabel);
  // meMap[meId] = dbe->book1D(meId, meTitle, 30, -10, 10);
  
  // sprintf(meId,"RecHitY_vs_dY_%s",detUnitLabel);
  // sprintf(meTitle,"RecHit_Yposition_vs_Error_%s",layerLabel);
  // meMap[meId] = dbe->book2D(meId, meTitle, 30, -100, 100,30,10,10);
  
  dbe->setCurrentFolder(GlobalHistogramsFolder);
  
  os.str("");
  os<<"SectorOccupancy_"<<ringType<<"_"<<detId.ring()<<"_Sector_"<<detId.sector();
  meId = os.str();
  meTitle = os.str();
  
  /*
  std::string Yaxis=detUnitLabel;
  Yaxis.erase (1,1);
  Yaxis.erase(0,3);
  Yaxis.replace(Yaxis.find("S"),4,"");
  Yaxis.erase(Yaxis.find("_")+2,8);
  meMap[meId]->setBinLabel(nrnr, Yaxis, 2);
  */
  int nrnr=2;


   cout<<"data "<<david<<" "<<detUnitLabel<<endl;
   david++;
  
  if (detId.sector()==9 || detId.sector()==11 ) {
    
    meMap[meId] = dbe->book2D(meId, meTitle,  96, 0.5, 96.5, 15, 0.5, 15.5);
    
  }
  
  else  if (detId.sector()==4) {
    
    meMap[meId] = dbe->book2D(meId, meTitle,  96, 0.5, 96.5, 22, 0.5, 22.5);
    
  }
  else {
    meMap[meId] = dbe->book2D(meId, meTitle,  96, 0.5, 96.5, 16, 0.5, 16.5);
    
  }
 
  std::cout <<"End of Booking "<<std::endl;
  return meMap;
}


std::map<std::string, MonitorElement*> RPCMonitorDigi::bookRegionRing(int region, int ring) {
  
  std::cout<<"begin of Global folder constructor"<<std::endl;;
  std::map<std::string, MonitorElement*> meMap;
  
  std::string ringType = (region ==  0)?"Wheel":"Disk";
  
  dbe->setCurrentFolder(GlobalHistogramsFolder);
  
  std::stringstream os;
  string meId;
  string meTitle;
  os<<"WheelOccupancyXY_"<<ringType<<"_"<<region<<"_"<<ring;
  meId = os.str();
  os.str("");
  os<<"Wheel_Occupancy_XY_Coordinates_for_"<<ringType<<"_"<<region<<"_"<<ring;
  meTitle = os.str();
  meMap[meId] = dbe->book2D(meId, meTitle, 1000, -800, 800, 1000, -800, 800);

  os.str("");
  os<<"WheelClusterSize_"<<ringType<<"_"<<region<<"_"<<ring;
  meId = os.str();
  meTitle = os.str();
  meMap[meId] = dbe->book1D(meId, meTitle, 20, 0.5, 20.5);
  
  std::cout<<"end of Global folder"<<std::endl;;
  
  return meMap;
  
}
