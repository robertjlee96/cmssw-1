/*
 * \file DTLocalTriggerTask.cc
 * 
 * $Date: 2007/10/09 14:59:36 $
 * $Revision: 1.15 $
 * \author M. Zanetti - INFN Padova
 *
*/

#include "DQM/DTMonitorModule/interface/DTLocalTriggerTask.h"

// Framework
#include "FWCore/Framework/interface/EventSetup.h"

// Digis
#include "DataFormats/DTDigi/interface/DTDigi.h"
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"

// DT trigger
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThDigi.h"
#include "DataFormats/DTDigi/interface/DTLocalTriggerCollection.h"

//Digis & RecHit
#include "DataFormats/LTCDigi/interface/LTCDigi.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"

// Geometry
#include "DataFormats/GeometryVector/interface/Pi.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

#include <sstream>
#include <iostream>
#include <fstream>


using namespace edm;
using namespace std;

DTLocalTriggerTask::DTLocalTriggerTask(const edm::ParameterSet& ps){
  
  debug = ps.getUntrackedParameter<bool>("debug", "false");

  if(debug)   
    cout<<"[DTLocalTriggerTask]: Constructor"<<endl;

  dcc_label  = ps.getUntrackedParameter<string>("dcc_label", "dttpgprod");
  ros_label  = ps.getUntrackedParameter<string>("ros_label", "dtunpacker");
  seg_label  = ps.getUntrackedParameter<string>("seg_label", "dt4DSegments");
  parameters = ps;
  
  dbe = edm::Service<DaqMonitorBEInterface>().operator->();
  edm::Service<MonitorDaemon> daemon; 	 
  daemon.operator->();    

}


DTLocalTriggerTask::~DTLocalTriggerTask() {

  if(debug)
    cout << "[DTLocalTriggerTask]: analyzed " << nevents << " events" << endl;

}


void DTLocalTriggerTask::beginJob(const edm::EventSetup& context){

 if(debug)
    cout<<"[DTLocalTriggerTask]: BeginJob"<<endl;

 context.get<MuonGeometryRecord>().get(muonGeom);
 nevents = 0;

}


void DTLocalTriggerTask::beginLuminosityBlock(const LuminosityBlock& lumiSeg, const EventSetup& context) {

  if(debug)
    cout<<"[DTLocalTriggerTask]: Begin of LS transition"<<endl;
  
  if(lumiSeg.id().luminosityBlock()%parameters.getUntrackedParameter<int>("ResetCycle", 3) == 0) {
    for(map<string, map<uint32_t, MonitorElement*> > ::const_iterator histo = digiHistos.begin();
	histo != digiHistos.end();
	histo++) {
      for(map<uint32_t, MonitorElement*> ::const_iterator ht = (*histo).second.begin();
	  ht != (*histo).second.end();
	  ht++) {
	(*ht).second->Reset();
      }
    }
  }
  
}


void DTLocalTriggerTask::endJob(){

  cout << "[DTLocalTriggerTask]: analyzed " << nevents << " events" << endl;
  dbe->rmdir("DT/DTLocalTriggerTask");

}


void DTLocalTriggerTask::analyze(const edm::Event& e, const edm::EventSetup& c){
  
  nevents++;
    
  string trigsrc= triggerSource(e);  

  if (parameters.getUntrackedParameter<bool>("process_dcc", true) ) {   
    runDCCAnalysis(e,trigsrc);
  }  
  if ( parameters.getUntrackedParameter<bool>("process_ros", true) ) {
    runDDUAnalysis(e,trigsrc);
  }
  if ( parameters.getUntrackedParameter<bool>("process_seg", true) ) {
    runSegmentAnalysis(e,trigsrc);
  } 

}


void DTLocalTriggerTask::bookHistos(const DTChamberId& dtCh, string folder, string histoTag) {
  
  int wh=dtCh.wheel();		
  int sc=dtCh.sector();	
  stringstream wheel; wheel << wh;	
  stringstream station; station << dtCh.station();	
  stringstream sector; sector << sc;	

  string histoType = histoTag.substr(4,histoTag.find("_",4)-4);

  if (debug)
    cout<<"[DTLocalTriggerTask]: booking"<<endl;

  if ( folder == "SectorGeneral") {   // Booking of sector quantities
    
    dbe->setCurrentFolder("DT/DTLocalTriggerTask/Wheel" + wheel.str() +
			  "/Sector" + sector.str() + "/" + folder );

    if (debug)
      cout << "[DTLocalTriggerTask]: folder "<< "DT/DTLocalTriggerTask/Wheel" << wheel.str() 
	   << "/Sector" << sector.str()  << "/" << folder << endl;
    
    string histoName = histoTag + "_W" + wheel.str() + "_Sec" + sector.str();

    if( histoType == "SCdata") {
      uint32_t indexScWh = 5*(sc-1) + (wh+3) ;    // wheel + sector identifier for specific histograms      
      (digiHistos[histoTag])[indexScWh] = 
	dbe->book1D(histoName,histoName,6,-0.5,5.5);
    }
    
  }
  else {    // Booking of station quantities
    
    dbe->setCurrentFolder("DT/DTLocalTriggerTask/Wheel" + wheel.str() +
			  "/Sector" + sector.str() +
			  "/Station" + station.str() + "/" + folder);

    string histoName = histoTag + "_W" + wheel.str() + "_Sec" + sector.str() + "_St" + station.str();
    
    if (debug){
      cout << "[DTLocalTriggerTask]: folder " << "DT/DTLocalTriggerTask/Wheel" << wheel.str()
	   << "/Sector" << sector.str()
	   << "/Station"<< station.str() << "/" << folder << endl;
      cout << "[DTLocalTriggerTask]: histoType " << histoType << endl; 
      cout << "[DTLocalTriggerTask]: histoName " << histoName << endl;
    }
    
    if ( folder == "LocalTriggerPhi") {
      
      if( histoType == "BXvsQual"){
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,8,-0.5,7.5,101,-50.5,50.5);
	return ;
      }
      if( histoType == "BestQual"){
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book1D(histoName,histoName,8,-0.5,7.5);
	return ;
      }
      if( histoType == "QualvsPhirad"){
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,100,-500.,500.,8,-0.5,7.5);
	return ;
      }
      if( histoType == "QualvsPhibend") { 
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,200,-2.,2.,8,-0.5,7.5);
	return ;
      }
      if( histoType == "Flag1stvsBX") { 
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,101,-50.5,50.5,2,-0.5,1.5);
	return ;
      }
      if( histoType == "Flag1stvsQual") {
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,8,-0.5,7.5,2,-0.5,1.5);
	return ;
      }
      
    }
    else if ( folder == "LocalTriggerTheta")   {
      
      if( histoType == "PositionvsBX") {
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,101,-50.5,50.5,7,-0.5,6.5);
	return ;
      }
      if( histoType == "PositionvsQual") {
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,8,-0.5,7.5,6,-0.5,6.5);
	return ;
      }  
      if( histoType == "ThetaBXvsQual") {
 	(digiHistos[histoTag])[dtCh.rawId()] = 
 	  dbe->book2D(histoName,histoName,8,-0.5,7.5,60,-9.5,50.5);
      }
      if( histoType == "ThetaBestQual"){
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book1D(histoName,histoName,8,-0.5,7.5);
	return ;
      }

    }
    else if ( folder == "Segment")   {
      
      if( histoType.find("TrackThetaPosvsAngle") == 0) {
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,16,-40.,40.,20,-117.5,117.5);
	return ;
      }
      if( histoType.find("TrackPosvsAngle") == 0){
	pair<float,float> range = phiRange(dtCh);
	int nbins = int((range.second - range.first)/15);
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,16,-40.,40.,nbins,range.first,range.second);
	return ;
      }
      if( histoType == "PhitkvsPhitrig"){ 
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,100,-500.,500.,100,-500.,500.);
	return ;
      }
      if( histoType == "PhibtkvsPhibtrig"){ 
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,200,-2.,2.,200,-2.,2.);
	return ;
      }
      if( histoType == "HitstkvsQualtrig"){ 
	(digiHistos[histoTag])[dtCh.rawId()] = 
	  dbe->book2D(histoName,histoName,8,-0.5,7.5,10,0.5,10.5);
	return ;
      }

    }
  }

}

void DTLocalTriggerTask::runDCCAnalysis(const edm::Event& e, string& trigsrc){

  string histoType ;
  string histoTag ;

  edm::Handle<L1MuDTChambPhContainer> l1dtlocalphi;
  e.getByLabel(dcc_label, l1dtlocalphi);
  vector<L1MuDTChambPhDigi>*  l1phitrig = l1dtlocalphi->getContainer();

  // define best quality phi trigger segment in any station
  // start from 1 and zero is kept empty
  for (int i=0;i<5;++i)
    for (int j=0;j<6;++j)
      for (int k=0;k<13;++k)
	phcode_best[j][i][k] = -1;

    
  for(vector<L1MuDTChambPhDigi>::const_iterator i = l1phitrig->begin(); i != l1phitrig->end(); i++) {
    int phwheel = i->whNum();
    int phsec   = i->scNum() + 1; // SM The track finder goes from 0 to 11. I need them from 1 to 12 !!!!!
    int phst    = i->stNum();
    int phbx    = i->bxNum();
    int phcode  = i->code();
    int phi1st  = i->Ts2Tag();
    int phphi   = i->phi();
    int phphiB  = i->phiB();

    if(phcode>phcode_best[phwheel+3][phst][phsec] && phcode<7) {
      phcode_best[phwheel+3][phst][phsec]=phcode; 
      iphbest[phwheel+3][phst][phsec] = &(*i);
    }
      
    DTChamberId dtChId(phwheel,phst,phsec);
      
    float x     = phi2Pos(dtChId,phphi);
    float angle = phib2Ang(dtChId,phphiB,phphi);
    uint32_t indexCh = dtChId.rawId();
    //uint32_t indexScWh = 5*(phsec-1) + (phwheel+3) ;    // wheel + sector identifier for specific histograms
      
    // SM BX vs Quality Phi view      
    string histoTag = "DCC_BXvsQual"+ trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerPhi"), histoTag );
    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(phcode,phbx);
          
    // SM Quality vs radial angle Phi view
    histoTag = "DCC_QualvsPhirad" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerPhi"), histoTag );
    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x,phcode);
      
    // SM Quality vs bending Phi view
    histoTag = "DCC_QualvsPhibend" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerPhi"), histoTag );
    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(angle,phcode);
      
    // SM BX 1st trigger track segment, phi view
    histoTag = "DCC_Flag1stvsBX" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerPhi"), histoTag );
    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(phbx,phi1st);
      
    // SM Quality 1st trigger track segment, phi view
    histoTag =  "DCC_Flag1stvsQual" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerPhi"), histoTag );
    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(phcode,phi1st);
      
  } 
    
  edm::Handle<L1MuDTChambThContainer> l1dtlocalth;
  e.getByLabel(dcc_label, l1dtlocalth);
  vector<L1MuDTChambThDigi>*  l1thetatrig = l1dtlocalth->getContainer();
  int thcode[7];

  for (int i=0;i<5;++i)
    for (int j=0;j<6;++j)
      for (int k=0;k<13;++k)
	thcode_best[j][i][k] = -1;

  for(vector<L1MuDTChambThDigi>::const_iterator j = l1thetatrig->begin(); j != l1thetatrig->end(); j++) {
    int thwheel = j->whNum();
    int thsec   = j->scNum() + 1; // SM The track finder goes from 0 to 11. I need them from 1 to 12 !!!!!
    int thst    = j->stNum();
    int thbx    = j->bxNum();
      
    for (int pos=0; pos<7; pos++) {
      thcode[pos] = j->code(pos);

      if(thcode[pos]>thcode_best[thwheel+3][thst][thsec] ) {
	thcode_best[thwheel+3][thst][thsec]=thcode[pos]; 
	ithbest[thwheel+3][thst][thsec] = &(*j);
      }
    } 
      
    DTChamberId dtChId(thwheel,thst,thsec);
    uint32_t indexCh = dtChId.rawId();   
    // uint32_t indexScWh = 5*(thsec-1) + (thwheel+3) ;    // wheel + sector identifier for specific histograms     

    // SM BX vs Position Theta view
    histoTag = "DCC_PositionvsBX"+ trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerTheta"), histoTag );
    for (int pos=0; pos<7; pos++) //SM fill position for non zero position bit in theta view
      if(thcode[pos]>0)
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(thbx,pos);
      
    // SM Code vs Position Theta view
    histoTag =  "DCC_PositionvsQual" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerTheta"), histoTag );
    for (int pos=0; pos<7; pos++) //SM fill position for non zero position bit in theta view
      if(thcode[pos]>0){
	int thqual = (thcode[pos]/2)*2+1;
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(thqual,pos);
      }

    // SM BX vs Code Theta view
    histoTag =  "DCC_ThetaBXvsQual" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( dtChId, string("LocalTriggerTheta"), histoTag );
    for (int pos=0; pos<7; pos++) //SM fill position for non zero position bit in theta view
      if(thcode[pos]>0){
	int thqual = (thcode[pos]/2)*2+1;
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(thqual,thbx);
      }
  }
}

void DTLocalTriggerTask::runDDUAnalysis(const edm::Event& e, string& trigsrc){
    
  string histoType ;
  string histoTag ;
  
  Handle<DTLocalTriggerCollection> dtTrigs;
  e.getByLabel(ros_label,dtTrigs);
  DTLocalTriggerCollection::DigiRangeIterator detUnitIt;
    
  for (int i=0;i<5;++i)
    for (int j=0;j<6;++j)
      for (int k=0;k<13;++k)
	dduphcode_best[j][i][k] = -1;

  for (int i=0;i<5;++i)
    for (int j=0;j<6;++j)
      for (int k=0;k<13;++k)
	dduthcode_best[j][i][k] = -1;
    
  for (detUnitIt=dtTrigs->begin();
       detUnitIt!=dtTrigs->end();
       ++detUnitIt){
      
    const DTChamberId& id = (*detUnitIt).first;
    const DTLocalTriggerCollection::Range& range = (*detUnitIt).second;
    uint32_t indexCh = id.rawId();  
      
    for (DTLocalTriggerCollection::const_iterator trigIt = range.first;
	 trigIt!=range.second;
	 ++trigIt){
	
      int bx = trigIt->bx();
      int quality = trigIt->quality();
      int thqual = trigIt->trTheta();
      int flag1st = 0;
	    
      int wh = id.wheel();
      int sec = id.sector();
      int st = id.station();

      uint32_t indexScWh = 5*(sec-1) + (wh+3) ;    // wheel + sector identifier for specific histograms      

      // check if SC data exist: fill for any trigger
      if(quality<7 ) {	  // it is a phi trigger


	if(quality>dduphcode_best[wh+3][st][sec] && quality<7) { // find best ddu trigger in phi view
	    
	  if(dduphcode_best[wh+3][st][sec]== -1) {  // see if at least a Phi trigger 
	    histoTag = "DDU_SCdata" + trigsrc;
	    if ((digiHistos[histoTag].find(indexScWh) == digiHistos[histoTag].end())) {
	      bookHistos( id, string("SectorGeneral"), histoTag);
	    }	    
	    (digiHistos.find(histoTag)->second).find(indexScWh)->second->Fill(st);
	  }
	     
	  dduphcode_best[wh+3][st][sec]=quality; 
	}
	  
	histoTag =  "DDU_BXvsQual" + trigsrc;
	if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	  bookHistos( id, string("LocalTriggerPhi"), histoTag );
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(quality,bx);

	if(trigIt->secondTrack())  flag1st = 1;  // it is a second trigger track
           
	histoTag =  "DDU_Flag1stvsBX" + trigsrc;
	if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	  bookHistos( id, string("LocalTriggerPhi"), histoTag );
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(bx,flag1st);

	histoTag = "DDU_Flag1stvsQual" + trigsrc;
	if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	  bookHistos( id, string("LocalTriggerPhi"), histoTag );
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(quality,flag1st);	  
      }
      if( thqual>0) {  // it is a theta trigger
	  
	if(thqual>dduthcode_best[wh+3][st][sec] ) { // find best ddu trigger in theta view
	  dduthcode_best[wh+3][st][sec]=thqual; 
	}

	histoTag = "DDU_ThetaBXvsQual" + trigsrc;
	if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	  bookHistos( id, string("LocalTriggerTheta"), histoTag );
	(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(thqual,bx);
      }
    }

    // Fill Quality plots with best ddu triggers in phi & theta
    histoTag =  "DDU_BestQual" + trigsrc;
    if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
      bookHistos( id, string("LocalTriggerPhi"), histoTag );
    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(dduphcode_best[id.wheel()+3][id.station()][id.sector()]);

    if (dduthcode_best[id.wheel()+3][id.station()][id.sector()]>0){
      histoTag = "DDU_ThetaBestQual" + trigsrc;
      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	bookHistos( id, string("LocalTriggerTheta"), histoTag );
      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(dduthcode_best[id.wheel()+3][id.station()][id.sector()]);
    }  
  } 
}

void DTLocalTriggerTask::runSegmentAnalysis(const edm::Event& e, string& trigsrc){    

  string histoType ;
  string histoTag ;

  Handle<DTRecSegment4DCollection> all4DSegments;
  e.getByLabel(seg_label, all4DSegments);  
  DTRecSegment4DCollection::const_iterator track;
  // it tells whether there is a track in a station.
  memset(track_flag,false,450*sizeof(bool));

  // First loop useful to compute trigger efficiency
  memset(track_ok,false,450*sizeof(bool));
  for ( track = all4DSegments->begin(); track != all4DSegments->end(); ++track){
    if((*track).hasPhi()) {
      int wheel = (*track).chamberId().wheel();
      int sector = (*track).chamberId().sector();
      int station = (*track).chamberId().station();
	
      if (sector==13){
	sector=4;
      }
      else if (sector==14){
	sector=10;
      }

      if (track_ok[wheel+3][station][sector]==false && (*track).phiSegment()->degreesOfFreedom()>=2){
	track_ok[wheel+3][station][sector]=true;
      }
    }
  }
    
  for ( track = all4DSegments->begin(); track != all4DSegments->end(); ++track){
      
    if((*track).hasPhi()) { // Phi component
	
      int wheel = (*track).chamberId().wheel();
      int sector = (*track).chamberId().sector();
      int station = (*track).chamberId().station();
      int scsector;
      float x_track, y_track, x_angle, y_angle;
      computeCoordinates((*track),scsector,x_track,x_angle,y_track,y_angle);

      if(!track_flag[wheel+3][station][sector]) {      /* if no track already found in this station */
	track_flag[wheel+3][station][sector] = true;   /* the 1st track is always the best          */
	
	DTChamberId dtChId(wheel,station,scsector);  // get chamber for histograms
	uint32_t indexCh = dtChId.rawId(); 

	
	if (parameters.getUntrackedParameter<bool>("process_dcc", true) &&
	    phcode_best[wheel+3][station][scsector] > -1 && 
	    phcode_best[wheel+3][station][scsector] < 7 ) {
	    
	  int phphi = iphbest[wheel+3][station][scsector]->phi();
	  float x_trigger = phi2Pos(dtChId,phphi);
	  float angle_trigger = phib2Ang(dtChId,iphbest[wheel+3][station][scsector]->phiB(),phphi);
	    
	  // SM phi of the track vs phi of the trigger
	  histoTag = "DCC_PhitkvsPhitrig"+ trigsrc;
	  if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	    bookHistos( dtChId, string("Segment"), histoTag );
	  (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x_trigger,x_track);
	    
	  // SM phib of the track vs phib of the trigger
	  histoTag = "DCC_PhibtkvsPhibtrig"+ trigsrc;
	  if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	    bookHistos( dtChId, string("Segment"), histoTag );
	  (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(angle_trigger,x_angle);
	    
	  // SM hits of the track vs quality of the trigger
	  histoTag =  "DCC_HitstkvsQualtrig" + trigsrc;
	  if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	    bookHistos( dtChId, string("Segment"), histoTag );	     
	  (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(iphbest[wheel+3][station][scsector]->code(),
									  (*track).phiSegment()->degreesOfFreedom()+2 );
	    
	}

	// look for triggers in other stations of the same sector to compute efficiency
	bool trig_flag = false;	
	if (parameters.getUntrackedParameter<bool>("process_ros", true)) {
	  for (int ist=1; ist<5; ist++){
	    if (ist!=station &&
		dduphcode_best[wheel+3][ist][scsector]>=2 && 
		dduphcode_best[wheel+3][ist][scsector]<7 &&
		track_ok[wheel+3][ist][scsector]==true
		){
	      trig_flag = true;
	      break;
	    }
	  }
	}
	

	if (parameters.getUntrackedParameter<bool>("process_dcc", true) && trig_flag==0) {
	  for (int ist=1; ist<5; ist++){
	    if (ist!=station &&
		phcode_best[wheel+3][ist][scsector]>=2 && 
		phcode_best[wheel+3][ist][scsector]<7){
	      trig_flag = true;
	      break;
	    }
	  }
	}
	  
	// compute plots to calculate efficiency using segments
	if (trig_flag && fabs(x_angle)<40. && (*track).phiSegment()->degreesOfFreedom()>=5){
	  
	  // position of track for reconstruced tracks (denom. for trigger efficiency)
	  histoTag =  "SEG_TrackPosvsAngle"+ trigsrc;
	  if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	    bookHistos( dtChId, string("Segment"), histoTag );        
	  (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x_angle,x_track);
	  
	  if (parameters.getUntrackedParameter<bool>("process_dcc", true) ) {
	    if (phcode_best[wheel+3][station][scsector] > -1 && phcode_best[wheel+3][station][scsector] < 7) {
	      
	      histoTag =  "DCC_TrackPosvsAngleandTrig"+ trigsrc;
	      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		bookHistos( dtChId, string("Segment"), histoTag );        
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x_angle,x_track);

	      if (phcode_best[wheel+3][station][scsector] > 4){  //HH & HL Triggers
	      histoTag =  "DCC_TrackPosvsAngleandTrigHHHL"+ trigsrc;
	      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		bookHistos( dtChId, string("Segment"), histoTag );        
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x_angle,x_track);
	      }	      
	    }
	  }
	  if ( parameters.getUntrackedParameter<bool>("process_ros", true) ) {	    
	    if (dduphcode_best[wheel+3][station][scsector] >= 2 && dduphcode_best[wheel+3][station][scsector] < 7) {
	      
	      histoTag =  "DDU_TrackPosvsAngleandTrig"+ trigsrc;
	      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		bookHistos( dtChId, string("Segment"), histoTag );        
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x_angle,x_track);

	      if (dduphcode_best[wheel+3][station][scsector] > 4){ // HH & HL Triggers
		histoTag =  "DDU_TrackPosvsAngleandTrigHHHL"+ trigsrc;
	      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		bookHistos( dtChId, string("Segment"), histoTag );        
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(x_angle,x_track);

	      }	      
	    }
	  }
	}

	// compute plots to calculate theta efficiency using segments 
	if ((*track).hasZed() && trig_flag && fabs(y_angle)<40. && (*track).zSegment()->degreesOfFreedom()>=1){
	    
	  // position of track for reconstruced tracks (denom. for trigger efficiency) along theta direction
	  histoTag = "SEG_TrackThetaPosvsAngle" + trigsrc;
	  if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
	    bookHistos( dtChId, string("Segment"), histoTag );        
	  (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(y_angle,y_track);
	    	  
	  if (parameters.getUntrackedParameter<bool>("process_dcc", true) ) {
	    if (thcode_best[wheel+3][station][scsector] > 0) {		
		
	      histoTag = "DCC_TrackThetaPosvsAngleandTrig" + trigsrc;
	      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		bookHistos( dtChId, string("Segment"), histoTag );
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(y_angle,y_track);
		
	      if (thcode_best[wheel+3][station][scsector] == 3) {
		histoTag = "DCC_TrackThetaPosvsAngleandTrigH" + trigsrc;
		if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		  bookHistos( dtChId, string("Segment"), histoTag );
		(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(y_angle,y_track);
 	      }		
		
	    }
	   
	  }
	  if ( parameters.getUntrackedParameter<bool>("process_ros", true) ) {	    
	    if (dduthcode_best[wheel+3][station][scsector] > 0) {
		
	      histoTag = "DDU_TrackThetaPosvsAngleandTrig" + trigsrc;
	      if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		bookHistos( dtChId, string("Segment"), histoTag );
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(y_angle,y_track);
				
	      if (dduthcode_best[wheel+3][station][scsector]==3) {		
		histoTag = "DDU_TrackThetaPosvsAngleandTrigH" + trigsrc;
		if ((digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()))
		  bookHistos( dtChId, string("Segment"), histoTag );
		(digiHistos.find(histoTag)->second).find(indexCh)->second->Fill(y_angle,y_track);

	      }		    
	    }
	  }  
	}
      }
    }
  }
}


void DTLocalTriggerTask::computeCoordinates(const DTRecSegment4D& track, 
					    int& scsector, 
					    float& phpos, 
					    float& phdir, 
					    float& zpos, 
					    float& zdir){


  float xcenter;	
  LocalPoint lpos;
  int wheel = track.chamberId().wheel();          
  int sector = track.chamberId().sector();
  int station = track.chamberId().station();
  const DTChamber* chamb;
  const DTChamber* scchamb;
  phdir = atan(track.localDirection().x()/ track.localDirection().z())/Geom::pi()*180.;
  zdir = atan(track.localDirection().y()/ track.localDirection().z())/Geom::pi()*180.;


  if (station == 4){
    switch (sector) {
    case 4:
      scsector = 4;
      chamb   = muonGeom->chamber(DTChamberId(wheel,station,13));
      scchamb = muonGeom->chamber(DTChamberId(wheel,station,4));
      xcenter = scchamb->toLocal(chamb->position()).x()*.5;
      phpos = track.localPosition().x()-xcenter;
      zpos  = track.localPosition().y();
      break;
    case 10:
      scsector = 10;
      chamb   = muonGeom->chamber(DTChamberId(wheel,station,14));
      scchamb = muonGeom->chamber(DTChamberId(wheel,station,10));
      xcenter = scchamb->toLocal(chamb->position()).x()*.5;
      phpos = track.localPosition().x()-xcenter;
      zpos = track.localPosition().y();
      break;
    case 13:
      scsector = 4;
      chamb   = muonGeom->chamber(DTChamberId(wheel,station,sector));
      scchamb = muonGeom->chamber(DTChamberId(wheel,station,scsector));
      lpos = scchamb->toLocal(chamb->toGlobal(track.localPosition()));
      xcenter = scchamb->toLocal(chamb->position()).x()*.5;
      phpos = lpos.x()-xcenter;
      zpos  = lpos.y();
      break;
    case 14:
      scsector = 10;
      chamb   = muonGeom->chamber(DTChamberId(wheel,station,sector));
      scchamb = muonGeom->chamber(DTChamberId(wheel,station,scsector));
      lpos = scchamb->toLocal(chamb->toGlobal(track.localPosition()));
      xcenter = scchamb->toLocal(chamb->position()).x()*.5;
      phpos = lpos.x()-xcenter;
      zpos  = lpos.y();
      break;
    default:
      scsector = sector;
      phpos = track.localPosition().x();
      zpos  = track.localPosition().y();
    }
  }
  else {
    scsector = sector;
    phpos = track.localPosition().x();
    zpos  = track.localPosition().y();
  }

}


pair<float,float> DTLocalTriggerTask::phiRange(const DTChamberId& id){

  float min,max;
  int station = id.station();
  int sector  = id.sector(); 
  int wheel   = id.wheel();
  
  const DTLayer  *layer = muonGeom->layer(DTLayerId(id,1,1));
  DTTopology topo = layer->specificTopology();
  min = topo.wirePosition(topo.firstChannel());
  max = topo.wirePosition(topo.lastChannel());

  if (station == 4){
    
    const DTLayer *layer2;
    float lposx;
    
    if (sector == 4){
      layer2  = muonGeom->layer(DTLayerId(wheel,station,13,1,1));
      lposx = layer->toLocal(layer2->position()).x();
    }
    else if (sector == 10){
      layer2 = muonGeom->layer(DTLayerId(wheel,station,14,1,1));
      lposx = layer->toLocal(layer2->position()).x();
    }
    else
      return make_pair(min,max);
    
    DTTopology topo2 = layer2->specificTopology();

    if (lposx>0){
      max =  lposx*.5 + topo2.wirePosition(topo2.lastChannel());
      min -= lposx*.5;
    }
    else{ 
      min =  lposx*.5 + topo2.wirePosition(topo2.firstChannel());
      max -= lposx*.5;
    }
  }

  return make_pair(min,max);

}

float DTLocalTriggerTask::phi2Pos(const DTChamberId & id, int phi){

  float phin = (id.sector()-1)*Geom::pi()/6;
  GlobalPoint gpos = muonGeom->chamber(id)->position();
  float deltaphi =  gpos.phi()-phin;

  if (id.station() == 4 && ( id.sector() == 4 || id.sector() == 10))
    deltaphi = 0;

  float x = (tan(phi/4096.)-tan(deltaphi))*gpos.mag()*cos(deltaphi);
  
  if (id.wheel()>0 || (id.wheel()==0 && id.sector()%4>1)) 
    x= -x;

  return x;

}


float DTLocalTriggerTask::phib2Ang(const DTChamberId & id, int phib, double phi){
  
  float fphi = phib/512.+phi/4096.+(id.sector()-4)*Geom::pi()/6.;

  if (fphi > Geom::pi()*1.5) fphi-=Geom::pi()*2.;
  if (fphi < -Geom::pi()*.5) fphi+=Geom::pi()*2.;
  if (fphi >  Geom::pi()*.5) fphi-=Geom::pi();

  if (id.wheel()<0 || (id.wheel()==0 && id.sector()%4<=1)) 
    fphi = -fphi;

  return fphi;

}


string DTLocalTriggerTask::triggerSource(const edm::Event& e) {
  
  string l1ASource;
  
  if ( !parameters.getUntrackedParameter<bool>("localrun", true) ){
    
    Handle<LTCDigiCollection> ltcdigis;
    e.getByType(ltcdigis);
    
    for (std::vector<LTCDigi>::const_iterator ltc_it = ltcdigis->begin(); ltc_it != ltcdigis->end(); ltc_it++){
      
      int otherTriggerSum=0;
      for (int i = 1; i < 6; i++) {
	otherTriggerSum += int((*ltc_it).HasTriggered(i));
      }
      if ((*ltc_it).HasTriggered(0) && otherTriggerSum == 0) 
	l1ASource = "_DTonly";
      else if (!(*ltc_it).HasTriggered(0))
	l1ASource = "_NoDT";
      else if ((*ltc_it).HasTriggered(0) && otherTriggerSum > 0)
	l1ASource = "_DTalso";
      
    }
    return l1ASource;
  }
  return "";

}
