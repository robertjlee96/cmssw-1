// -*- c++ -*-
/**\class SiStripMonitorDigi SiStripMonitorDigi.cc DQM/SiStripMonitorDigi/src/SiStripMonitorDigi.cc
 */
// Original Author:  Dorian Kcira
//         Created:  Sat Feb  4 20:49:10 CET 2006
// $Id: SiStripMonitorDigi.cc,v 1.26 2008/07/21 16:44:38 charaf Exp $
#include<fstream>
#include "TNamed.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "DataFormats/Common/interface/DetSetNew.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "DQM/SiStripCommon/interface/SiStripHistoId.h"
#include "DQM/SiStripMonitorDigi/interface/SiStripMonitorDigi.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TMath.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"


//--------------------------------------------------------------------------------------------
SiStripMonitorDigi::SiStripMonitorDigi(const edm::ParameterSet& iConfig) : dqmStore_(edm::Service<DQMStore>().operator->()), conf_(iConfig), show_mechanical_structure_view(true), show_readout_view(false), show_control_view(false), select_all_detectors(true), reset_each_run(false), m_cacheID_(0), folder_organizer() 
{
  firstEvent = -1;
  eventNb = 0;

  //get on/off option for every cluster from cfi
  edm::ParameterSet ParametersNumberOfDigis =  conf_.getParameter<edm::ParameterSet>("TH1NumberOfDigis");
  layerswitchnumdigison = ParametersNumberOfDigis.getParameter<bool>("layerswitchon");
  moduleswitchnumdigison = ParametersNumberOfDigis.getParameter<bool>("moduleswitchon");
  
  edm::ParameterSet ParametersADCsHottestStrip =  conf_.getParameter<edm::ParameterSet>("TH1ADCsHottestStrip");
  layerswitchadchotteston = ParametersADCsHottestStrip.getParameter<bool>("layerswitchon");
  moduleswitchadchotteston = ParametersADCsHottestStrip.getParameter<bool>("moduleswitchon");
  
  edm::ParameterSet ParametersADCsCoolestStrip =  conf_.getParameter<edm::ParameterSet>("TH1ADCsCoolestStrip");
  layerswitchadccooleston = ParametersADCsCoolestStrip.getParameter<bool>("layerswitchon");
  moduleswitchadccooleston = ParametersADCsCoolestStrip.getParameter<bool>("moduleswitchon");
  
  edm::ParameterSet ParametersDigiADCs =  conf_.getParameter<edm::ParameterSet>("TH1DigiADCs");
  layerswitchdigiadcson = ParametersDigiADCs.getParameter<bool>("layerswitchon");
  moduleswitchdigiadcson = ParametersDigiADCs.getParameter<bool>("moduleswitchon");
  
  edm::ParameterSet ParametersStripOccupancy =  conf_.getParameter<edm::ParameterSet>("TH1StripOccupancy");
  layerswitchstripoccupancyon = ParametersStripOccupancy.getParameter<bool>("layerswitchon");
  moduleswitchstripoccupancyon = ParametersStripOccupancy.getParameter<bool>("moduleswitchon");

  edm::ParameterSet ParametersDigiProf = conf_.getParameter<edm::ParameterSet>("TProfNumberOfDigi");
  layerswitchnumdigisprofon = ParametersDigiProf.getParameter<bool>("layerswitchon");
  edm::ParameterSet ParametersDigiADC = conf_.getParameter<edm::ParameterSet>("TProfDigiADC");
  layerswitchdigiadcprofon = ParametersDigiProf.getParameter<bool>("layerswitchon");

  edm::ParameterSet ParametersDetsOn =  conf_.getParameter<edm::ParameterSet>("detectorson");
  tibon = ParametersDetsOn.getParameter<bool>("tibon");
  tidon = ParametersDetsOn.getParameter<bool>("tidon");
  tobon = ParametersDetsOn.getParameter<bool>("tobon");
  tecon = ParametersDetsOn.getParameter<bool>("tecon");

  createTrendMEs = conf_.getParameter<bool>("CreateTrendMEs");
}
//------------------------------------------------------------------------------------------
SiStripMonitorDigi::~SiStripMonitorDigi() { }

//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::beginRun(const edm::Run& run, const edm::EventSetup& es){

  if (show_mechanical_structure_view) {
    unsigned long long cacheID = es.get<SiStripDetCablingRcd>().cacheIdentifier();
    if (m_cacheID_ != cacheID) {
      m_cacheID_ = cacheID;       
      edm::LogInfo("SiStripMonitorDigi") <<"SiStripMonitorDigi::beginRun: " 
					 << " Creating MEs for new Cabling ";     
      createMEs(es);
    } 
  } else if (reset_each_run) {
    edm::LogInfo("SiStripMonitorDigi") <<"SiStripMonitorDigi::beginRun: " 
				       << " Resetting MEs ";        
    for (std::map<uint32_t, ModMEs >::const_iterator idet = DigiMEs.begin() ; idet!=DigiMEs.end() ; idet++) {
      ResetModuleMEs(idet->first);
    }
  }
}

//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::endRun(const edm::Run&, const edm::EventSetup&){
}



//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::beginJob(const edm::EventSetup& es){
}


//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::createMEs(const edm::EventSetup& es){

  if ( show_mechanical_structure_view ){
    // take from eventSetup the SiStripDetCabling object - here will use SiStripDetControl later on
    edm::ESHandle<SiStripDetCabling> tkmechstruct;
    es.get<SiStripDetCablingRcd>().get(tkmechstruct);
    
    // get list of active detectors from SiStripDetCabling
    std::vector<uint32_t> activeDets; 
    activeDets.clear(); // just in case
    tkmechstruct->addActiveDetectorsRawIds(activeDets);
    SiStripSubStructure substructure;
    
    std::vector<uint32_t> SelectedDetIds;
    if(select_all_detectors){
      // select all detectors if appropriate flag is set,  for example for the mtcc
      SelectedDetIds = activeDets;
    }else{
      // use SiStripSubStructure for selecting certain regions


      if(tibon) substructure.getTIBDetectors(activeDets, SelectedDetIds, 0, 0, 0, 0); // this adds rawDetIds to SelectedDetIds
      if(tobon) substructure.getTOBDetectors(activeDets, SelectedDetIds, 0, 0, 0);    // this adds rawDetIds to SelectedDetIds
      if(tidon) substructure.getTIDDetectors(activeDets, SelectedDetIds, 0, 0, 0, 0); // this adds rawDetIds to SelectedDetIds
      if(tecon) substructure.getTECDetectors(activeDets, SelectedDetIds, 0, 0, 0, 0, 0, 0); // this adds rawDetIds to SelectedDetIds

    }

    // remove any eventual zero elements - there should be none, but just in case
    for(std::vector<uint32_t>::iterator idets = SelectedDetIds.begin(); idets != SelectedDetIds.end(); idets++){
      if(*idets == 0) SelectedDetIds.erase(idets);
    }
    
    // create SiStripFolderOrganizer
    SiStripFolderOrganizer folder_organizer;
    
    std::vector<uint32_t> tibDetIds;
    // loop over detectors and book MEs
    edm::LogInfo("SiStripTkDQM|SiStripMonitorDigi")<<"nr. of SelectedDetIds:  "<<SelectedDetIds.size();
    for(std::vector<uint32_t>::const_iterator detid_iterator = SelectedDetIds.begin(); detid_iterator!=SelectedDetIds.end(); detid_iterator++){

      uint32_t detid = (*detid_iterator);

      ModMEs local_modmes;
      local_modmes.nStrip = tkmechstruct->nApvPairs(detid) * 2 * 128;

      // set appropriate folder using SiStripFolderOrganizer
      folder_organizer.setDetectorFolder(detid); // pass the detid to this method
      if (reset_each_run) ResetModuleMEs(detid);
      createModuleMEs(local_modmes, detid);
      // append to DigiMEs
      DigiMEs.insert( std::make_pair(detid, local_modmes));

      // Created Layer Level MEs if thet=y are npt created already
      std::pair<std::string,int32_t> det_layer_pair = folder_organizer.GetSubDetAndLayer(detid);
      if (DetectedLayers.find(det_layer_pair) == DetectedLayers.end()){
	DetectedLayers[det_layer_pair]=true;

        int32_t lnumber = det_layer_pair.second;
        std::vector<uint32_t> layerDetIds;
        if (det_layer_pair.first == "TIB")      substructure.getTIBDetectors(SelectedDetIds,layerDetIds,lnumber,0,0,0);
        else if (det_layer_pair.first == "TOB") substructure.getTOBDetectors(SelectedDetIds,layerDetIds,lnumber,0,0);
        else if (det_layer_pair.first == "TID" && lnumber > 0) substructure.getTIDDetectors(SelectedDetIds,layerDetIds,2,lnumber,0,0);
        else if (det_layer_pair.first == "TID" && lnumber < 0) substructure.getTIDDetectors(SelectedDetIds,layerDetIds,1,lnumber,0,0);
        else if (det_layer_pair.first == "TEC" && lnumber > 0) substructure.getTECDetectors(SelectedDetIds,layerDetIds,2,lnumber,0,0,0,0);
        else if (det_layer_pair.first == "TEC" && lnumber < 0) substructure.getTECDetectors(SelectedDetIds,layerDetIds,1,lnumber,0,0,0,0);

	int subdetid;
	int subsubdetid;
	std::string label;
	getLayerLabel(detid, label, subdetid, subsubdetid);
        LayerDetMap[label] = layerDetIds;

        // book Layer plots      
	folder_organizer.setLayerFolder(detid,det_layer_pair.second); 

	createLayerMEs(label, layerDetIds.size());
      }    
    
    }//end of loop over detectors

  }//end of if

}//end of method



//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){


  using namespace edm;

  runNb   = iEvent.id().run();
  //  eventNb = iEvent.id().event();
  eventNb++;

  // get all digi collections
  //edm::Handle< edm::DetSetVector<SiStripDigi> > digi_detsetvektor;
  typedef std::vector<edm::ParameterSet> Parameters;
  Parameters DigiProducersList = conf_.getParameter<Parameters>("DigiProducersList");
  Parameters::iterator itDigiProducersList = DigiProducersList.begin();

  for(; itDigiProducersList != DigiProducersList.end(); ++itDigiProducersList ) {

    std::string digiProducer = itDigiProducersList->getParameter<std::string>("DigiProducer");
    std::string digiLabel = itDigiProducersList->getParameter<std::string>("DigiLabel");
    iEvent.getByLabel(digiProducer,digiLabel,digi_detsetvektor);
    
    if (!digi_detsetvektor.isValid()) continue; 
    
    for (std::map<std::string, std::vector< uint32_t > >::const_iterator iterLayer = LayerDetMap.begin();
	 iterLayer != LayerDetMap.end(); iterLayer++) {

      std::string layer_label = iterLayer->first;

      std::vector< uint32_t > layer_dets = iterLayer->second;
      std::map<std::string, LayerMEs>::iterator iLayerME = LayerMEsMap.find(layer_label);
      
      //get Layer MEs 
      LayerMEs local_layermes;
      if(iLayerME == LayerMEsMap.end()) continue;
      else local_layermes = iLayerME->second; 
      int largest_adc_layer= 0;
      int smallest_adc_layer= 99999;
      int ndigi_layer = 0;
      
      uint16_t iDet = 0;
      // loop over all modules in the layer
      for (std::vector< uint32_t >::const_iterator iterDets = layer_dets.begin() ; 
	   iterDets != layer_dets.end() ; iterDets++) {
	iDet++;
	// detid and type of ME
	uint32_t detid = (*iterDets);
	
	// DetId and corresponding set of MEs
	std::map<uint32_t, ModMEs >::iterator pos = DigiMEs.find(detid);
	ModMEs local_modmes = pos->second;
	
	// search  digis of detid
	edm::DetSetVector<SiStripDigi>::const_iterator isearch = digi_detsetvektor->find(detid); 
	
	if(isearch==digi_detsetvektor->end()){
	  
	  // no digis for this detector module, so fill histogram with 0
	  if(moduleswitchnumdigison && (local_modmes.NumberOfDigis != NULL))
	    (local_modmes.NumberOfDigis)->Fill(0.0); 
	  
	  if (layerswitchnumdigisprofon) 
	    local_layermes.LayerNumberOfDigisProfile->Fill(iDet*1.0,0.0);

	  continue; // no digis for this detid => jump to next step of loop
	}//end of if "isearch == digi ..."
	

	//digi_detset is a structure
	//digi_detset.data is a std::vector<SiStripDigi>
	//digi_detset.id is uint32_t
	edm::DetSet<SiStripDigi> digi_detset = (*digi_detsetvektor)[detid]; // the statement above makes sure there exists an element with 'detid'

	// nr. of digis per detector
	if(moduleswitchnumdigison && (local_modmes.NumberOfDigis != NULL)) 
	  (local_modmes.NumberOfDigis)->Fill(static_cast<float>(digi_detset.size()));
        
        ndigi_layer += digi_detset.size();
	if (layerswitchnumdigisprofon) {
	  //	  if (layer_label.find("TOB") != std::string::npos) std::cout  << detid << " " << iDet << " " << digi_detset.size() << std::endl;
          local_layermes.LayerNumberOfDigisProfile->Fill(iDet*1.0,digi_detset.size()*1.0);
        }
	
	// ADCs
	int largest_adc=(digi_detset.data.begin())->adc();
	int smallest_adc=(digi_detset.data.begin())->adc();

	int subdetid;
	int subsubdetid;
	std::string label;
	getLayerLabel(detid, label, subdetid, subsubdetid);
        float det_occupancy = 0.0;

	for(edm::DetSet<SiStripDigi>::const_iterator digiIter = digi_detset.data.begin(); 
	    digiIter!= digi_detset.data.end(); digiIter++ ){
	  
	  int this_adc = digiIter->adc();
	  if (this_adc > 0.0) det_occupancy++;

	  //          if (layer_label.find("TOB") != std::string::npos) {
	    //            std::cout << layer_label<< " "  << detid << " " << digiIter->strip() << " " << digiIter->adc() <<
	  //	      " " << det_occupancy << "  "<< local_modmes.nStrip << std::endl;
	  //          }
	  if(this_adc>largest_adc) largest_adc  = this_adc; 
	  if(this_adc<smallest_adc) smallest_adc  = this_adc; 
	  
	  if(moduleswitchdigiadcson && (local_modmes.DigiADCs != NULL ) )
	    (local_modmes.DigiADCs)->Fill(static_cast<float>(this_adc));
	  	  
	  //Fill #ADCs for this digi at layer level
	  if(layerswitchdigiadcson) {
	    fillME(local_layermes.LayerDigiADCs , this_adc);
	    if (createTrendMEs) fillTrend(local_layermes.LayerDigiADCsTrend, this_adc);
	  }

	  if (layerswitchdigiadcprofon) 
	    local_layermes.LayerDigiADCProfile->Fill(iDet*1.0,this_adc);

	}//end of loop over digis in this det
        
        // Occupancy
        if (local_modmes.nStrip > 0 && det_occupancy > 0 ) {
          det_occupancy = det_occupancy/local_modmes.nStrip;
	  if (moduleswitchstripoccupancyon) {
            (local_modmes.StripOccupancy)->Fill(det_occupancy);
          }
          if (layerswitchstripoccupancyon) {
	    fillME(local_layermes.LayerStripOccupancy, det_occupancy);
            if (createTrendMEs) fillTrend(local_layermes.LayerStripOccupancyTrend, det_occupancy);
          }
        }

        if  (largest_adc > largest_adc_layer) largest_adc_layer = largest_adc;
        if  (smallest_adc < smallest_adc_layer) smallest_adc_layer = smallest_adc;

	// nr. of adcs for hottest strip
	if(moduleswitchadchotteston && (local_modmes.ADCsHottestStrip != NULL)) 
	  (local_modmes.ADCsHottestStrip)->Fill(static_cast<float>(largest_adc));
	
	// nr. of adcs for coolest strip	
	if(moduleswitchadccooleston && (local_modmes.ADCsCoolestStrip != NULL)) 
	  (local_modmes.ADCsCoolestStrip)->Fill(static_cast<float>(smallest_adc));
	
      }//end of loop over DetIds

      if(layerswitchnumdigison) {
	fillME(local_layermes.LayerNumberOfDigis,ndigi_layer);
	if (createTrendMEs) fillTrend(local_layermes.LayerNumberOfDigisTrend,ndigi_layer);
      }
      if(layerswitchadchotteston) {
	fillME(local_layermes.LayerADCsHottestStrip,largest_adc_layer);
	if (createTrendMEs) fillTrend(local_layermes.LayerADCsHottestStripTrend,largest_adc_layer);
      }
      if(layerswitchadccooleston) {
	fillME(local_layermes.LayerADCsCoolestStrip ,smallest_adc_layer);
	if (createTrendMEs) fillTrend(local_layermes.LayerADCsCoolestStripTrend,smallest_adc_layer);
      }
    }

  } //end of loop over digi producers (ZeroSuppressed, VirginRaw, ProcessedRaw, ScopeMode)

}//end of method analyze



//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::endJob(void){
  bool outputMEsInRootFile = conf_.getParameter<bool>("OutputMEsInRootFile");
  std::string outputFileName = conf_.getParameter<std::string>("OutputFileName");
  if(outputMEsInRootFile){
    std::ofstream monitor_summary("monitor_digi_summary.txt");
    monitor_summary<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
    monitor_summary<<"SiStripMonitorDigi::endJob DigiMEs.size()="<<DigiMEs.size()<<std::endl;

    for(std::map<uint32_t, ModMEs>::const_iterator idet = DigiMEs.begin(); idet!= DigiMEs.end(); idet++ ){

      monitor_summary<<"SiStripTkDQM|SiStripMonitorDigi"<<"      ++++++detid  "<<idet->first<<std::endl<<std::endl;

      if(moduleswitchnumdigison) {     
	monitor_summary<<"SiStripTkDQM|SiStripMonitorDigi"<<"              +++ NumberOfDigis "<<(idet->second).NumberOfDigis->getEntries()<<" "<<(idet->second).NumberOfDigis->getMean()<<" "<<(idet->second).NumberOfDigis->getRMS()<<std::endl;
      }

      if(moduleswitchadchotteston) {     
	monitor_summary<<"SiStripTkDQM|SiStripMonitorDigi"<<"              +++ ADCsHottestStrip "<<(idet->second).ADCsHottestStrip->getEntries()<<" "<<(idet->second).ADCsHottestStrip->getMean()<<" "<<(idet->second).ADCsHottestStrip->getRMS()<<std::endl;
      }

      if(moduleswitchadccooleston) {     
	monitor_summary<<"SiStripTkDQM|SiStripMonitorDigi"<<"              +++ ADCsCoolestStrip "<<(idet->second).ADCsCoolestStrip->getEntries()<<" "<<(idet->second).ADCsCoolestStrip->getMean()<<" "<<(idet->second).ADCsCoolestStrip->getRMS()<<std::endl;
      }

      if(moduleswitchdigiadcson) {     
	monitor_summary<<"SiStripTkDQM|SiStripMonitorDigi"<<"              +++ DigiADCs         "<<(idet->second).DigiADCs->getEntries()<<" "<<(idet->second).DigiADCs->getMean()<<" "<<(idet->second).DigiADCs->getRMS()<<std::endl;
      }
    
    }//end of loop over MEs

    monitor_summary<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
    
    // save histograms in a file
    dqmStore_->save(outputFileName);

  }//end of if

  
}//end of method
//--------------------------------------------------------------------------------------------
void SiStripMonitorDigi::ResetModuleMEs(uint32_t idet){
  std::map<uint32_t, ModMEs >::iterator pos = DigiMEs.find(idet);
  ModMEs mod_me = pos->second;

  if(moduleswitchnumdigison) mod_me.NumberOfDigis->Reset();
  if(moduleswitchadchotteston) mod_me.ADCsHottestStrip->Reset();
  if(moduleswitchadccooleston) mod_me.ADCsCoolestStrip->Reset();
  if(moduleswitchdigiadcson) mod_me.DigiADCs->Reset();
  if(moduleswitchstripoccupancyon) mod_me.StripOccupancy->Reset();

}
//------------------------------------------------------------------------------------------
MonitorElement* SiStripMonitorDigi::bookMETrend(const char* ParameterSetLabel, const char* HistoName)
{
  Parameters =  conf_.getParameter<edm::ParameterSet>(ParameterSetLabel);
  edm::ParameterSet ParametersTrend =  conf_.getParameter<edm::ParameterSet>("Trending");
  MonitorElement* me = dqmStore_->bookProfile(HistoName,HistoName,
					      ParametersTrend.getParameter<int32_t>("Nbins"),
					      // 					      0,
					      ParametersTrend.getParameter<double>("xmin"),
					      ParametersTrend.getParameter<double>("xmax"),
					      // 					      ParametersTrend.getParameter<int32_t>("Nbins"),
					      100, //that parameter should not be there !?
					      ParametersTrend.getParameter<double>("ymin"),
					      ParametersTrend.getParameter<double>("ymax"),
					      "" );
  if(!me) return me;
  char buffer[256];
  sprintf(buffer,"EventId/%d",ParametersTrend.getParameter<int32_t>("Steps"));
  me->setAxisTitle(std::string(buffer),1);
  return me;
}

//------------------------------------------------------------------------------------------
MonitorElement* SiStripMonitorDigi::bookME1D(const char* ParameterSetLabel, const char* HistoName)
{
  Parameters =  conf_.getParameter<edm::ParameterSet>(ParameterSetLabel);
  return dqmStore_->book1D(HistoName,HistoName,
			   Parameters.getParameter<int32_t>("Nbinx"),
			   Parameters.getParameter<double>("xmin"),
			   Parameters.getParameter<double>("xmax")
			   );
}

//--------------------------------------------------------------------------------
void SiStripMonitorDigi::fillTrend(MonitorElement* me ,float value)
{
  if(!me) return;
  //check the origin and check options
  int option = conf_.getParameter<edm::ParameterSet>("Trending").getParameter<int32_t>("UpdateMode");
  if(firstEvent==-1) firstEvent = eventNb;
  int CurrentStep = atoi(me->getAxisTitle(1).c_str()+8);
  int firstEventUsed = firstEvent;
  int presentOverflow = (int)me->getBinEntries(me->getNbinsX()+1);
  if(option==2) firstEventUsed += CurrentStep * int(me->getBinEntries(me->getNbinsX()+1));
  else if(option==3) firstEventUsed += CurrentStep * int(me->getBinEntries(me->getNbinsX()+1)) * me->getNbinsX();
  //fill
  me->Fill((eventNb-firstEventUsed)/CurrentStep,value);

  if(eventNb-firstEvent<1) return;
  // check if we reached the end
  if(presentOverflow == me->getBinEntries(me->getNbinsX()+1)) return;
  switch(option) {
  case 1:
    {
      // mode 1: rebin and change X scale
      int NbinsX = me->getNbinsX();
      float entries = 0.;
      float content = 0.;
      float error = 0.;
      int bin = 1;
      int totEntries = int(me->getEntries());
      for(;bin<=NbinsX/2;++bin) {
	content = (me->getBinContent(2*bin-1) + me->getBinContent(2*bin))/2.; 
	error   = pow((me->getBinError(2*bin-1)*me->getBinError(2*bin-1)) + (me->getBinError(2*bin)*me->getBinError(2*bin)),0.5)/2.; 
	entries = me->getBinEntries(2*bin-1) + me->getBinEntries(2*bin);
	me->setBinContent(bin,content*entries);
	me->setBinError(bin,error);
	me->setBinEntries(bin,entries);
      }
      for(;bin<=NbinsX+1;++bin) {
	me->setBinContent(bin,0);
	me->setBinError(bin,0);
	me->setBinEntries(bin,0); 
      }
      me->setEntries(totEntries);
      char buffer[256];
      sprintf(buffer,"EventId/%d",CurrentStep*2);
      me->setAxisTitle(std::string(buffer),1);
      break;
    }
  case 2:
    {
      // mode 2: slide
      int bin=1;
      int NbinsX = me->getNbinsX();
      for(;bin<=NbinsX;++bin) {
	me->setBinContent(bin,me->getBinContent(bin+1)*me->getBinEntries(bin+1));
	me->setBinError(bin,me->getBinError(bin+1));
	me->setBinEntries(bin,me->getBinEntries(bin+1));
      }
      break;
    }
  case 3:
    {
      // mode 3: reset
      int NbinsX = me->getNbinsX();
      for(int bin=0;bin<=NbinsX;++bin) {
	me->setBinContent(bin,0);
	me->setBinError(bin,0);
	me->setBinEntries(bin,0); 
      }
      break;
    }
  }
}

//
// -- Create Module Level MEs
//
void SiStripMonitorDigi::createModuleMEs(ModMEs& mod_single, uint32_t detid) {

  // use SistripHistoId for producing histogram id (and title)
  SiStripHistoId hidmanager;
  std::string hid;
  
  //nr. of digis per module
  if(moduleswitchnumdigison) {
    hid = hidmanager.createHistoId("NumberOfDigis","det",detid);
    mod_single.NumberOfDigis = bookME1D("TH1NumberOfDigis", hid.c_str());
    dqmStore_->tag(mod_single.NumberOfDigis, detid);
    mod_single.NumberOfDigis->setAxisTitle("number of digis in one detector module");
    mod_single.NumberOfDigis->getTH1()->StatOverflows(kTRUE);  // over/underflows in Mean calculation
  }
  
  //#ADCs for hottest strip
  if(moduleswitchadchotteston) {
    hid = hidmanager.createHistoId("ADCsHottestStrip","det",detid);
    mod_single.ADCsHottestStrip = bookME1D("TH1ADCsHottestStrip", hid.c_str());
    dqmStore_->tag(mod_single.ADCsHottestStrip, detid); // 6 APVs -> 768 strips
    mod_single.ADCsHottestStrip->setAxisTitle("number of ADCs for hottest strip");
  }
  
  //#ADCs for coolest strip
  if(moduleswitchadccooleston) {
    hid = hidmanager.createHistoId("ADCsCoolestStrip","det",detid);
    mod_single.ADCsCoolestStrip = bookME1D("TH1ADCsCoolestStrip", hid.c_str());
    dqmStore_->tag(mod_single.ADCsCoolestStrip, detid);
    mod_single.ADCsCoolestStrip->setAxisTitle("number of ADCs for coolest strip");
  }
  
  //#ADCs for each digi
  if(moduleswitchdigiadcson) {
    hid = hidmanager.createHistoId("DigiADCs","det",detid);
    mod_single.DigiADCs = bookME1D("TH1DigiADCs", hid.c_str());
    dqmStore_->tag(mod_single.DigiADCs, detid);
    mod_single.DigiADCs->setAxisTitle("number of ADCs for each digi");
  }
  
  //Strip occupancy
  if(moduleswitchstripoccupancyon) {
    hid = hidmanager.createHistoId("StripOccupancy","det",detid);
    mod_single.StripOccupancy = bookME1D("TH1StripOccupancy", hid.c_str());
    dqmStore_->tag(mod_single.StripOccupancy, detid);
    mod_single.StripOccupancy->setAxisTitle("strip occupancy");
  }
  
}
  
//
// -- Create Module Level MEs
//  

void SiStripMonitorDigi::createLayerMEs(std::string label, int ndets) {

  std::map<std::string, LayerMEs>::iterator iLayerME  = LayerMEsMap.find(label);
  if(iLayerME==LayerMEsMap.end()){
    SiStripHistoId hidmanager;
    LayerMEs layerMEs; 

    //#Digis
    if(layerswitchnumdigison) {
      layerMEs.LayerNumberOfDigis=bookME1D("TH1NumberOfDigis", hidmanager.createHistoLayer("Summary_TotalNumberOfDigis","layer",label,"").c_str()); 
      if (createTrendMEs) layerMEs.LayerNumberOfDigisTrend=bookMETrend("TH1NumberOfDigis", hidmanager.createHistoLayer("Trend_NumberOfDigis","layer",label,"").c_str()); 
    }

    //#ADCs for hottest strip
    if(layerswitchadchotteston) {
      layerMEs.LayerADCsHottestStrip=bookME1D("TH1ADCsHottestStrip", hidmanager.createHistoLayer("Summary_ADCsHottestStrip","layer",label,"").c_str()); 
      if (createTrendMEs) layerMEs.LayerADCsHottestStripTrend=bookMETrend("TH1ADCsHottestStrip", hidmanager.createHistoLayer("Trend_ADCsHottestStrip","layer",label,"").c_str()); 
    }

    //#ADCs for coolest strip
    if(layerswitchadccooleston) {
      layerMEs.LayerADCsCoolestStrip=bookME1D("TH1ADCsCoolestStrip", hidmanager.createHistoLayer("Summary_ADCsCoolestStrip","layer",label,"").c_str());
      if (createTrendMEs) layerMEs.LayerADCsCoolestStripTrend=bookMETrend("TH1ADCsCoolestStrip", hidmanager.createHistoLayer("Trend_ADCsCoolestStrip","layer",label,"").c_str());
    }

    //#ADCs for each digi
    if(layerswitchdigiadcson) {
      layerMEs.LayerDigiADCs=bookME1D("TH1DigiADCs", hidmanager.createHistoLayer("Summary_DigiADCs","layer",label,"").c_str());
      if (createTrendMEs) layerMEs.LayerDigiADCsTrend=bookMETrend("TH1DigiADCs", hidmanager.createHistoLayer("Trend_DigiADCs","layer",label,"").c_str());
    }

    //Strip Occupancy
    if(layerswitchstripoccupancyon) {
      layerMEs.LayerStripOccupancy=bookME1D("TH1StripOccupancy", hidmanager.createHistoLayer("Summary_StripOccupancy","layer",label,"").c_str());  
      if (createTrendMEs) layerMEs.LayerStripOccupancyTrend=bookMETrend("TH1StripOccupancy", hidmanager.createHistoLayer("Trend_StripOccupancy","layer",label,"").c_str());  
      
    }
    // # of Digis 
    if(layerswitchnumdigisprofon) {
      layerMEs.LayerNumberOfDigisProfile = dqmStore_->bookProfile("NumberOfDigiProfile","NumberOfDigiProfile",
                           ndets, 0.5, ndets+0.5,50, -5.0, 95.0);      
    }

    // # of Digis 
    if(layerswitchdigiadcprofon) {
      layerMEs.LayerDigiADCProfile = dqmStore_->bookProfile("DigiADCProfile","DigiADCProfile", ndets, 0.5, ndets+0.5, 50, 0.0, 255.0);      
    }

    LayerMEsMap[label]=layerMEs;
  }

}

//-------------------------------------------------------------------------------------------
void SiStripMonitorDigi::getLayerLabel(uint32_t detid, std::string& label, int& subdetid, int& subsubdetid) {
  StripSubdetector subdet(detid);
  std::ostringstream label_str;

  //subdetid = ((detid>>25)&0x7);
  //This is the id of the layer or of the wheel
  subsubdetid = 14;
  
  if(subdet.subdetId() == StripSubdetector::TIB ){
    // ---------------------------  TIB  --------------------------- //
    TIBDetId tib1 = TIBDetId(detid);
    label_str << "TIB__layer__" << tib1.layer();
    subdetid = 3;
    subsubdetid = tib1.layer()-1;
  }else if(subdet.subdetId() == StripSubdetector::TID){
    // ---------------------------  TID  --------------------------- //
    TIDDetId tid1 = TIDDetId(detid);
    label_str << "TID__side__" << tid1.side() << "__wheel__" << tid1.wheel();
    subdetid = 4;
    subsubdetid = (tid1.wheel()-1)+3*(tid1.side()-1);
  }else if(subdet.subdetId() == StripSubdetector::TOB){
    // ---------------------------  TOB  --------------------------- //
    TOBDetId tob1 = TOBDetId(detid);
    label_str << "TOB__layer__" << tob1.layer();
    subdetid = 5;
    subsubdetid = tob1.layer()-1;
  }else if(subdet.subdetId() == StripSubdetector::TEC) {
    // ---------------------------  TEC  --------------------------- //
    TECDetId tec1 = TECDetId(detid);
    label_str << "TEC__side__"<<tec1.side() << "__wheel__" << tec1.wheel();
    subdetid = 6;
    subsubdetid = (tec1.wheel()-1)+9*(tec1.side()-1);
  }else{
    // ---------------------------  ???  --------------------------- //
    edm::LogError("SiStripTkDQM|WrongInput")<<"no such subdetector type :"<<subdet.subdetId()<<" no folder set!"<<std::endl;
    label_str << "";
  }
  label = label_str.str();
}

    



//define this as a plug-in
DEFINE_FWK_MODULE(SiStripMonitorDigi);
