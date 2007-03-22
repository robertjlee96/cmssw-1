/*
 * \file DTDataIntegrityTask.cc
 * 
 * $Date: 2007/03/19 13:39:26 $
 * $Revision: 1.17 $
 * \author M. Zanetti (INFN Padova), S. Bolognesi (INFN Torino)
 *
*/

#include <DQM/DTMonitorModule/interface/DTDataIntegrityTask.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "EventFilter/DTRawToDigi/interface/DTDataMonitorInterface.h"
#include "EventFilter/DTRawToDigi/interface/DTControlData.h"
#include "EventFilter/DTRawToDigi/interface/DTDDUWords.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace edm;

DTDataIntegrityTask::DTDataIntegrityTask(const edm::ParameterSet& ps,edm::ActivityRegistry& reg) {

  reg.watchPostEndJob(this,&DTDataIntegrityTask::postEndJob);
 
  debug = ps.getUntrackedParameter<bool>("debug", "false");
  if (debug)
    cout<<"[DTDataIntegrityTask]: Constructor"<<endl;

  neventsDDU = 0;
  neventsROS25 = 0;

  outputFile = ps.getUntrackedParameter<string>("outputFile", "ROS25Test.root");

  parameters = ps;

  dbe = edm::Service<DaqMonitorBEInterface>().operator->();
  
  edm::Service<MonitorDaemon> daemon;
  daemon.operator->();

}



DTDataIntegrityTask::~DTDataIntegrityTask() {
  if(debug)
    cout<<"[DTDataIntegrityTask]: Destructor. Analyzed "<< neventsDDU <<" events"<<endl;
  //dbe->setCurrentFolder("DT/FED770");
  //dbe->removeContents();
}

/*
Folder Structure:
- One folder for each DDU, named FEDn
- Inside each DDU folder the DDU histos and the ROSn folder
- Inside each ROS folder the ROS histos and the ROBn folder
- Inside each ROB folder one occupancy plot and the TimeBoxes
  with the chosen granularity (simply change the histo name)
*/

void DTDataIntegrityTask::postEndJob(){
 if(debug)
   cout<<"[DTDataIntegrityTask]: postEndJob called!"<<endl;
   dbe->rmdir("DT/FED770");
}

void DTDataIntegrityTask::bookHistos(string folder, DTROChainCoding code) {

  stringstream dduID_s; dduID_s << code.getDDU();
  stringstream rosID_s; rosID_s << code.getROS();
  stringstream robID_s; robID_s << code.getROB();

  string histoType;
  string histoName;

  // DDU Histograms
  if ( folder == "DDU" ) {
    dbe->setCurrentFolder("DT/FED" + dduID_s.str());

    histoType = "DDUTTSValues";
    histoName = "FED" + dduID_s.str() + "_DDUTTSValues";
    (dduHistos[histoType])[code.getDDUID()] = dbe->book1D(histoName,histoName,7,0,7);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"disconnected",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"warning overflow",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"out of synch",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"busy",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"ready",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"error",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"disconnected",1);	

    histoType = "DDUTTS_2";
    histoName = "FED" + dduID_s.str() + "_DDUTTS_2";
    (dduHistos[histoType])[code.getDDUID()] = dbe->book1D(histoName,histoName,21,0,21);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"L1A mismatch",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"BX mismatch",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"L1A Full ch1-4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"L1A Full ch5-8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"L1A Full ch9-12",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"Input Full ch1-4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"Input Full ch5-8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(8,"Input Full ch9-12",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(9,"Output FIFO Full",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(10,"error ROS 1",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(11,"error ROS 2",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(12,"error ROS 3",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(13,"error ROS 4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(14,"error ROS 5",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(15,"error ROS 6",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(16,"error ROS 7",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(17,"error ROS 8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(18,"error ROS 9",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(19,"error ROS 10",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(20,"error ROS 11",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(21,"error ROS 12",1);

    histoType = "DDUTTS_12";
    histoName = "FED" + dduID_s.str() + "_DDUTTS_12";
    (dduHistos[histoType])[code.getDDUID()] = dbe->book1D(histoName,histoName,21,0,21);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"L1A mismatch",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"BX mismatch",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"L1A Full ch1-4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"L1A Full ch5-8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"L1A Full ch9-12",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"Input Full ch1-4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"Input Full ch5-8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(8,"Input Full ch9-12",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(9,"Output FIFO Full",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(10,"error ROS 1",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(11,"error ROS 2",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(12,"error ROS 3",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(13,"error ROS 4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(14,"error ROS 5",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(15,"error ROS 6",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(16,"error ROS 7",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(17,"error ROS 8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(18,"error ROS 9",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(19,"error ROS 10",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(20,"error ROS 11",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(21,"error ROS 12",1);


    histoType = "DDUEventLenght";
    histoName = "FED" + dduID_s.str() + "_DDUEventLenght";
    (dduHistos[histoType])[code.getDDUID()] = dbe->book1D(histoName,histoName,1000,0,1000);
 
    histoType = "DDUEventType";
    histoName = "FED" + dduID_s.str() + "_DDUEventType";
    (dduHistos[histoType])[code.getDDUID()] = dbe->book1D(histoName,histoName,7,1,8);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"physics",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"calibration",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"test",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"technical",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"simulated",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"traced",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"error",1);	
  
    histoType = "DDUROSList";
    histoName = "FED" + dduID_s.str() + "_DDUROSList";
    (dduHistos[histoType])[code.getDDUID()] = dbe->book1D(histoName,histoName,12,0,12);
    
    histoType = "DDUChannelStatus";
    histoName = "FED" + dduID_s.str() + "_DDUChannelStatus";;
    (dduHistos[histoType])[code.getDDUID()] = dbe->book2D(histoName,histoName,9,0,9,12,0,12);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"ch.enabled",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"timeout",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"ev.trailer lost",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"opt.fiber lost",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"tlk.prop.error",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"tlk.pattern error",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"tlk.sign.lost",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(8,"error from ROS",1);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(9,"if ROS in events",1);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"ROS 1",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"ROS 2",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"ROS 3",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"ROS 4",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"ROS 5",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"ROS 6",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"ROS 7",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(8,"ROS 8",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(9,"ROS 9",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(10,"ROS 10",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(11,"ROS 11",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(12,"ROS 12",2);

    histoType = "DDUFIFOStatus";
    histoName = "FED" + dduID_s.str() + "_DDUFIFOStatus";;
    (dduHistos[histoType])[code.getDDUID()] = dbe->book2D(histoName,histoName,7,0,7,3,0,3);
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"Input ch1-4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"Input ch5-8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"Input ch9-12",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(4,"Error/L1A ch1-4",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(5,"Error/L1A ch5-8",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(6,"Error/L1A ch9-12",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(7,"Output",1);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(1,"Full",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(2,"Almost Full",2);	
    ((dduHistos[histoType])[code.getDDUID()])->setBinLabel(3,"Not Full",2);	
  
  }

  // ROS Histograms
  if ( folder == "ROS" ) {

    dbe->setCurrentFolder("DT/FED" + dduID_s.str() + "/" + folder + rosID_s.str());

    histoType = "ROSTrailerBits";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_ROSTrailerBits";
    (rosHistos[histoType])[code.getROSID()] = dbe->book1D(histoName,histoName,128,0,128);
 

    histoType = "ROSError";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_ROSError";
    string histoTitle = histoName + " (Error type vs ROBID)";
    (rosHistos[histoType])[code.getROSID()] = dbe->book2D(histoName,histoTitle,32,0,32,8,0,8);

    histoType = "ROSDebug_BunchNumber";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_ROSDebug_BunchNumber";
    (rosHistos[histoType])[code.getROSID()] = dbe->book1D(histoName,histoName,2048,0,2048);

    histoType = "ROSDebug_BcntResCntLow";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_ROSDebug_BcntResCntLow";
    (rosHistos[histoType])[code.getROSID()] = dbe->book1D(histoName,histoName,16348,0,16348);

    histoType = "ROSDebug_BcntResCntHigh";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_ROSDebug_BcntResCntHigh";
    (rosHistos[histoType])[code.getROSID()] = dbe->book1D(histoName,histoName,16348,0,16348);
  }  

  // ROB/TDC Histograms
  if ( folder == "ROB_O") {
    
    dbe->setCurrentFolder("DT/FED" + dduID_s.str()+"/ROS"+rosID_s.str()+"/ROB"+robID_s.str());

    histoType = "Occupancy";
    histoName = "FED" + dduID_s.str() + "_ROS" + rosID_s.str() + "_ROB"+robID_s.str()+"_Occupancy";
    string histoTitle = histoName + " (TDC vs TDCchannel)";
    (robHistos[histoType])[code.getROBID()] = dbe->book2D(histoName,histoTitle,32,0,32,4,0,4);

  }

  if ( folder == "ROB_T") {

    dbe->setCurrentFolder("DT/FED" + dduID_s.str()+"/ROS"+rosID_s.str()+"/ROB"+robID_s.str());

    histoType = "TimeBox";
    histoName = "FED" + dduID_s.str() + "_ROS" + rosID_s.str() + "_ROB" + robID_s.str()+"_TimeBox";

    // used only if they have been set (controlled by the switch during filling)
    stringstream tdcID_s; tdcID_s << code.getTDC();
    stringstream chID_s; chID_s << code.getChannel();

    int index;
    switch (parameters.getUntrackedParameter<int>("TBhistoGranularity",1)) {
    case 1: // ROB
      index = code.getROBID();
      break;
    case 2: // TDC
      index = code.getTDCID();
      histoName = "FED" + dduID_s.str() 
	+ "_ROS" + rosID_s.str() 
	+ "_ROB" + robID_s.str()
	+ "_TDC" + tdcID_s.str() + "_TimeBox";
      break;
    case 3: // Ch
      index = code.getChannelID();
      histoName = "FED" + dduID_s.str() 
	+ "_ROS" + rosID_s.str() 
	+ "_ROB" + robID_s.str()
	+ "_TDC" + tdcID_s.str() 
	+ "_Channel" + chID_s.str() + "_TimeBox";
      break;
    default: // ROB
      index = code.getROBID();      
    }
    (robHistos[histoType])[index] = dbe->book1D(histoName,histoName,
						(parameters.getUntrackedParameter<int>("timeBoxUpperBound",10000)-
						 parameters.getUntrackedParameter<int>("timeBoxLowerBound",0))/2,
						parameters.getUntrackedParameter<int>("timeBoxLowerBound",0),
						parameters.getUntrackedParameter<int>("timeBoxUpperBound",10000));
  }
  

  if ( folder == "TDCError") {
    
    dbe->setCurrentFolder("DT/FED" + dduID_s.str()+"/ROS"+rosID_s.str()+"/ROB"+robID_s.str());

    histoType = "TDCError";
    string histoTitle = histoName + " (Error type vs TDC)";
    histoName = "FED" + dduID_s.str() + "_ROS" + rosID_s.str() + "_ROB"+robID_s.str()+"_TDCError";
    (robHistos[histoType])[code.getROBID()] = dbe->book2D(histoName,histoTitle,3000,0,3000,4,0,4);

  }


  // SC Histograms
  if ( folder == "SC" ) {
    // Same numbering for SC as for ROS
    dbe->setCurrentFolder("DT/FED" + dduID_s.str() + "/" + folder + rosID_s.str());

    // the SC histos belong to the ROS map (pay attention) since the data come from the corresponding ROS

    histoType = "SCTriggerBX";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_SCTriggerBX";
    string histoTitle = histoName + " (station vs BX)";
    (rosHistos[histoType])[code.getSCID()] = dbe->book2D(histoName,histoTitle,128,0,128,5,0,5);

    histoType = "SCTriggerQuality";
    histoName = "FED" + dduID_s.str() + "_" + folder + rosID_s.str() + "_SCTriggerQuality";
    histoTitle = histoName + "(quality vs station)";
    (rosHistos[histoType])[code.getSCID()] = dbe->book2D(histoName,histoTitle,5,0,5,8,0,8);

  }

}



void DTDataIntegrityTask::processROS25(DTROS25Data & data, int ddu, int ros) {
  
  neventsROS25++;
  if ((neventsROS25%1000 == 0) &&debug)
    cout<<"[DTDataIntegrityTask]: "<<neventsROS25<<" events analyzed by processROS25"<<endl;
  
  DTROChainCoding code;
  code.setDDU(ddu);
  code.setROS(ros);

  string histoType;

  /// ROS Data
  histoType = "ROSTrailerBits";

  // relic
  int datum = 
    data.getROSTrailer().TFF() << (23-16) | 
    data.getROSTrailer().TPX() << (22-16) |
    data.getROSTrailer().ECHO() << (20-16) |
    data.getROSTrailer().ECLO() << (18-16) |
    data.getROSTrailer().BCO()<< (16-16);

  /// FIXME: EC* data are not correctly treated. One histo each is needed
  if (rosHistos[histoType].find(code.getROSID()) != rosHistos[histoType].end()) {
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(1,data.getROSTrailer().TFF());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(2,data.getROSTrailer().TPX());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(3,data.getROSTrailer().ECHO());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(3,data.getROSTrailer().ECLO());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(3,data.getROSTrailer().BCO());
  }
  else {
    bookHistos( string("ROS"), code);
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(1,data.getROSTrailer().TFF());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(2,data.getROSTrailer().TPX());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(3,data.getROSTrailer().ECHO());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(3,data.getROSTrailer().ECLO());
    (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill(3,data.getROSTrailer().BCO());
  }
  
  histoType = "ROSError";
  for (vector<DTROSErrorWord>::const_iterator error_it = data.getROSErrors().begin();
       error_it != data.getROSErrors().end(); error_it++) {
    if (rosHistos[histoType].find(code.getROSID()) != rosHistos[histoType].end())
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*error_it).robID(), 
									      (*error_it).errorType());
    else {
      bookHistos( string("ROS"), code);
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*error_it).robID(), 
									      (*error_it).errorType());
    }
  }
  
  for (vector<DTROSDebugWord>::const_iterator debug_it = data.getROSDebugs().begin();
       debug_it != data.getROSDebugs().end(); debug_it++) {
    histoType = "ROSDebug_BunchNumber";
    if (rosHistos[histoType].find(code.getROSID()) != rosHistos[histoType].end())
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*debug_it).debugMessage());
    else {
      bookHistos( string("ROS"), code);
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*debug_it).debugMessage());
    }

    histoType = "ROSDebug_BcntResCntLow";
    if (rosHistos[histoType].find(code.getROSID()) != rosHistos[histoType].end())
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*debug_it).debugMessage());
    else {
      bookHistos( string("ROS"), code);
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*debug_it).debugMessage());
    }

    histoType = "ROSDebug_BcntResCntHigh";
    if (rosHistos[histoType].find(code.getROSID()) != rosHistos[histoType].end())
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*debug_it).debugMessage());
    else {
      bookHistos( string("ROS"), code);
      (rosHistos.find(histoType)->second).find(code.getROSID())->second->Fill((*debug_it).debugMessage());
    }
  }
  
  /// TDC Data  
  for (vector<DTTDCData>::const_iterator tdc_it = data.getTDCData().begin();
       tdc_it != data.getTDCData().end(); tdc_it++) {


    DTTDCMeasurementWord tdcDatum = (*tdc_it).second;
    int index;
    switch (parameters.getUntrackedParameter<int>("TBhistoGranularity",1)) {
    case 1:
      code.setROB((*tdc_it).first);
      index = code.getROBID();
      break;
    case 2:
      code.setROB((*tdc_it).first);
      code.setTDC(tdcDatum.tdcID());
      index = code.getTDCID();
      break;
    case 3:
      code.setROB((*tdc_it).first);
      code.setTDC(tdcDatum.tdcID());
      code.setChannel(tdcDatum.tdcChannel());
      index = code.getChannelID();
      break;
    default:
      code.setROB((*tdc_it).first);
      index = code.getROBID();
    }


    histoType = "Occupancy";
    if (robHistos[histoType].find(code.getROBID()) != robHistos[histoType].end()) {
      (robHistos.find(histoType)->second).find(code.getROBID())->second->Fill(tdcDatum.tdcChannel(),
									      tdcDatum.tdcID());
    }
    else {
      bookHistos( string("ROB_O"), code);
      (robHistos.find(histoType)->second).find(code.getROBID())->second->Fill(tdcDatum.tdcChannel(),
									      tdcDatum.tdcID());
    }

    histoType = "TimeBox";
    if (robHistos[histoType].find(index) != robHistos[histoType].end()) {
      (robHistos.find(histoType)->second).find(index)->second->Fill(tdcDatum.tdcTime());

    }
    else {
      bookHistos( string("ROB_T"), code);
      (robHistos.find(histoType)->second).find(index)->second->Fill(tdcDatum.tdcTime());
    }
  }


  /// TDC Error  
  for (vector<DTTDCError>::const_iterator tdc_it = data.getTDCError().begin();
       tdc_it != data.getTDCError().end(); tdc_it++) {

    code.setROB((*tdc_it).first);

    histoType = "TDCError";
    if (robHistos[histoType].find(code.getROBID()) != robHistos[histoType].end()) {
      (robHistos.find(histoType)->second).find(code.getROBID())->second->Fill(((*tdc_it).second).tdcError(), 
									      ((*tdc_it).second).tdcID());
    }
    else {
      bookHistos( string("TDCError"), code);
      (robHistos.find(histoType)->second).find(code.getROBID())->second->Fill(((*tdc_it).second).tdcError(), 
									      ((*tdc_it).second).tdcID());
    }

  }

  /// SC Data
  int stationGroup = 0 ; //= ((*sc_it).second)%2;
  for (vector<DTSectorCollectorData>::const_iterator sc_it = data.getSCData().begin();
       sc_it != data.getSCData().end(); sc_it++) {

    // SC Data words are devided into 2 parts each of 8 bits:
    //  LSB refers to MB1 and MB3
    //  MSB refers to MB2 and MB4

    // fill only the information regarding SC words with trigger
    bool hasTrigger_LSB = ((*sc_it).first).hasTrigger(0);
    bool hasTrigger_MSB = ((*sc_it).first).hasTrigger(1);

    // the quality
    int quality_LSB = ((*sc_it).first).trackQuality(0);
    int quality_MSB = ((*sc_it).first).trackQuality(1);

    
    if (hasTrigger_LSB) {

      histoType = "SCTriggerBX";
      if (rosHistos[histoType].find(code.getSCID()) != rosHistos[histoType].end())
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill((*sc_it).second, 1+stationGroup*2);
      else {									       
	bookHistos( string("SC"), code);
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill((*sc_it).second, 1+stationGroup*2);
      }										       

      histoType = "SCTriggerQuality";						       
      if (rosHistos[histoType].find(code.getSCID()) != rosHistos[histoType].end())      
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill(1+stationGroup*2,quality_LSB);
      else {									       
	bookHistos( string("SC"), code);						       
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill(1+stationGroup*2,quality_LSB);
      }
    }
    
    if (hasTrigger_MSB) {

      histoType = "SCTriggerBX";
      if (rosHistos[histoType].find(code.getSCID()) != rosHistos[histoType].end())
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill((*sc_it).second, 2+stationGroup*2);
      else {									       
	bookHistos( string("SC"), code);	
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill((*sc_it).second, 2+stationGroup*2);
      }										       
      
      histoType = "SCTriggerQuality";						       
      if (rosHistos[histoType].find(code.getSCID()) != rosHistos[histoType].end())      
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill(2+stationGroup*2,quality_MSB);
      else {									       
	bookHistos( string("SC"), code);						       
	(rosHistos.find(histoType)->second).find(code.getSCID())->second->Fill(2+stationGroup*2,quality_MSB);
      }
    }
    stationGroup = (stationGroup == 0 ? 1 : 0);  //switch between MB1-2 and MB3-4 data
  }
  
  if ((neventsROS25%parameters.getUntrackedParameter<int>("saveResultsFrequency", 10000)==0) && (parameters.getUntrackedParameter<bool>("writeHisto", true)) ) 
    dbe->save(parameters.getUntrackedParameter<string>("outputFile", "ROS25Test.root"));
  

}

void DTDataIntegrityTask::processFED(DTDDUData & data, int ddu) {

  neventsDDU++;
  if ((neventsDDU%1000 == 0) && debug)
    cout<<"[DTDataIntegrityTask]: "<<neventsDDU<<" events analyzed by processFED"<<endl;

  DTROChainCoding code;
  code.setDDU(ddu);

  string histoType;

  FEDTrailer trailer = data.getDDUTrailer();
  FEDHeader header = data.getDDUHeader();
  DTDDUSecondStatusWord secondWord = data.getSecondStatusWord();

  //1D HISTO WITH TTS VALUES form trailer (7 bins = 7 values)
  histoType = "DDUTTSValues";
  if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
      bookHistos( string("DDU"), code);
  }
  
  switch(trailer.ttsBits()){
    case 0:{ //disconnected
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(0);
      break;
    }
    case 1:{ //warning overflow
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(1);
      break;
    }
    case 2:{ //out of sinch
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(2);
      break;
    }
    case 4:{ //busy
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(3);
      break;
    }
    case 8:{ //ready
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(4);
      break;
    }
    case 12:{ //error
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(5);
      break;
    }
    case 16:{ //disconnected
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(6);
      break;
    }
    default:{
      cout<<"[DTDataInetegrityTask] DDU control: wrong TTS value "<<trailer.ttsBits()<<endl;
    }
  }
  
  //1D HISTO: IF TTS=2,12 CHECK L1A AND BX MISIMATCH, FIFO AND ROS ERROR (from status words)
  if(trailer.ttsBits()==2){
    histoType = "DDUTTS_2";
  }
  if(trailer.ttsBits()==12){
    histoType = "DDUTTS_12";
  }

  if(trailer.ttsBits()==2 || trailer.ttsBits()==12){
    if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
      bookHistos( string("DDU"), code);
    }
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(0,secondWord.l1AIDError());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(1,secondWord.bxIDError());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(2,(secondWord.fifoFull() & 0x1));
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(3,(secondWord.fifoFull() & 0x2));
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(4,(secondWord.fifoFull() & 0x4));
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(5,(secondWord.inputFifoFull() & 0x1));
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(6,(secondWord.inputFifoFull() & 0x2));
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(7,(secondWord.inputFifoFull() & 0x4));
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(8,secondWord.outputFifoFull());
    int channel=1;
    for (vector<DTDDUFirstStatusWord>::const_iterator fsw_it = data.getFirstStatusWord().begin();
	 fsw_it != data.getFirstStatusWord().end(); fsw_it++) {
      if((*fsw_it).timeout() || (*fsw_it).eventTrailerLost() || (*fsw_it).opticalFiberSignalLost() ||
	 (*fsw_it).opticalFiberSignalLost() || (*fsw_it).tlkPropagationError()||
	 (*fsw_it).tlkPatternError() ||(*fsw_it).tlkSignalLost())
	(dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(8+channel,1);
    }
    channel++;
  }

  //MONITOR TTS VS TIME if the tts value is changed from the last event
 //  if(trailer.ttsBits() != myPrev_ttsValue){
//     ev_ttsChange.push_back(header.lvl1ID());
//     ttsChange.push_back(trailer.ttsBits());
//     myPrev_ttsValue = trailer.ttsBits();
//   }

  //1D HISTOS: EVENT LENGHT from trailer
  //cout<<"1D HISTOS WITH EVENT LENGHT from trailer"<<endl;
  histoType = "DDUEventLenght";
  if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
      bookHistos( string("DDU"), code);
  }
  (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(trailer.lenght());

  //1D HISTO: EVENT TYPE from header
  //cout<<"1D HISTO WITH EVENT TYPE from header"<<endl;
   histoType = "DDUEventType";
  if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
      bookHistos( string("DDU"), code);
  }
  (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(header.triggerType());
  

  //1D HISTO: NUMBER OF ROS IN THE EVENTS from 2nd status word
  int rosList = secondWord.rosList();
  vector<int> rosPositions;
  for(int i=0;i<12;i++){
    if(rosList & 0x1)
      rosPositions.push_back(i);
    rosList >>= 1;
  }

  histoType = "DDUROSList";   
  if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
    bookHistos( string("DDU"), code);
  } 
  (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(rosPositions.size());

  //2D HISTO: ROS VS STATUS (8 BIT = 8 BIN) from 1st-2nd status words (9th BIN FROM LIST OF ROS in 2nd status word)
  histoType = "DDUChannelStatus";   
  if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
    bookHistos( string("DDU"), code);
  } 
  int channel=0;
  for (vector<DTDDUFirstStatusWord>::const_iterator fsw_it = data.getFirstStatusWord().begin();
       fsw_it != data.getFirstStatusWord().end(); fsw_it++) {
    // assuming association one-to-one between DDU channel and ROS
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(0,channel,(*fsw_it).channelEnabled());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(1,channel,(*fsw_it).timeout());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(2,channel,(*fsw_it).eventTrailerLost());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(3,channel,(*fsw_it).opticalFiberSignalLost());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(4,channel,(*fsw_it).tlkPropagationError());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(5,channel,(*fsw_it).tlkPatternError());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(6,channel,(*fsw_it).tlkSignalLost());
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(7,channel,(*fsw_it).errorFromROS());
    channel++;
  }
  //9th BIN FROM LIST OF ROS in 2nd status word
  for(vector<int>::const_iterator channel_it = rosPositions.begin(); channel_it != rosPositions.end(); channel_it++){
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(8,(*channel_it),1);
  }

  //MONITOR ROS LIST VS TIME if the ROS list is changed from the last event
  // if(rosPositions.size() != myPrev_ROSList){
//     ev_ROSListChange.push_back(header.lvl1ID());
//     ROSListChange.push_back(rosPositions.size());
//     myPrev_ROSList = rosPositions.size();
//   }

  //2D HISTO: FIFO STATUS from 2nd status word
  histoType = "DDUFIFOStatus";   
  if (dduHistos[histoType].find(code.getDDUID()) == dduHistos[histoType].end()) {
    bookHistos( string("DDU"), code);
  } 
  
  int inputFifoFull = secondWord.inputFifoFull();
  int inputFifoAlmostFull = secondWord.inputFifoAlmostFull();
  int fifoFull = secondWord.fifoFull();
  int fifoAlmostFull = secondWord.fifoAlmostFull();
  int outputFifoFull = secondWord.outputFifoFull();
  int outputFifoAlmostFull = secondWord.outputFifoAlmostFull();
  for(int i=0;i<3;i++){
    if(inputFifoFull & 0x1)
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(i,0);
    if(inputFifoAlmostFull & 0x1)
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(i,1);
    if(fifoFull & 0x1)
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(3+i,0);
    if(fifoAlmostFull & 0x1)
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(3+i,1);

    if(!(inputFifoFull & 0x1) && !(inputFifoAlmostFull & 0x1))
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(i,2);
    if(!(fifoFull & 0x1) && !(fifoAlmostFull & 0x1))
      (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(3+i,2);
    
    inputFifoFull >>= 1;
    inputFifoAlmostFull >>= 1;
    fifoFull >>= 1;
    fifoAlmostFull >>= 1;
  }

  if(outputFifoFull)
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(6,0);
  if(outputFifoAlmostFull)
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(6,1);
  if(!outputFifoFull && !outputFifoAlmostFull)
    (dduHistos.find(histoType)->second).find(code.getDDUID())->second->Fill(6,2);
}
  
