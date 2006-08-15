#include <DQM/HcalMonitorModule/src/HcalMonitorModule.h>

/*
 * \file HcalMonitorModule.cc
 * 
 * $Date: 2006/08/15 19:58:37 $
 * $Revision: 1.12 $
 * \author W Fisher
 *
*/

HcalMonitorModule::HcalMonitorModule(const edm::ParameterSet& ps){

  // verbosity switch
  m_verbose = ps.getUntrackedParameter<bool>("verbose", false);

  if(m_verbose) cout << "HcalMonitorModule: constructor...." << endl;

  m_logFile.open("HcalMonitorModule.log");

  m_dbe = 0;
  if ( ps.getUntrackedParameter<bool>("DaqMonitorBEInterface", false) ) {
    m_dbe = edm::Service<DaqMonitorBEInterface>().operator->();
    m_dbe->setVerbose(0);
  }

  m_monitorDaemon = false;
  if ( ps.getUntrackedParameter<bool>("MonitorDaemon", false) ) {
    edm::Service<MonitorDaemon> daemon;
    daemon.operator->();
    m_monitorDaemon = true;
  }

  m_outputFile = ps.getUntrackedParameter<string>("outputFile", "");
  if ( m_outputFile.size() != 0 ) {
    cout << "Hcal Monitoring histograms will be saved to " << m_outputFile.c_str() << endl;
  }

  
  m_runNum = 0; m_meStatus=0;
  m_meRunNum=0; m_meRunType=0;
  m_meEvtNum=0; m_meEvtMask=0;
  
  if ( m_dbe ) {
    m_dbe->setCurrentFolder("HcalMonitor");
    m_meStatus  = m_dbe->bookInt("STATUS");
    m_meRunNum  = m_dbe->bookInt("RUN NUMBER");
    m_meRunType = m_dbe->bookInt("RUN TYPE");
    m_meEvtNum  = m_dbe->bookInt("EVT NUMBER");
    m_meEvtMask = m_dbe->bookInt("EVT MASK");
    m_meBeamE   = m_dbe->bookInt("BEAM ENERGY");

    m_meStatus->Fill(-1);
    m_meRunNum->Fill(0);
    m_meRunType->Fill(-1);
    m_meEvtNum->Fill(-1);
    m_meEvtMask->Fill(-1);
    m_meBeamE->Fill(-1);
  }
  
  m_evtSel = new HcalMonitorSelector(ps);
  m_digiMon = NULL; m_dfMon = NULL; 
  m_rhMon = NULL;   m_pedMon = NULL; 
  m_ledMon = NULL;

  if ( ps.getUntrackedParameter<bool>("RecHitMonitor", false) ) {
    m_rhMon = new HcalRecHitMonitor();
    m_rhMon->setup(ps, m_dbe);
  }

  if ( ps.getUntrackedParameter<bool>("DigiMonitor", false) ) {
    m_digiMon = new HcalDigiMonitor();
    m_digiMon->setup(ps, m_dbe);
  }

  if ( ps.getUntrackedParameter<bool>("DataFormatMonitor", false) ) {
    m_dfMon = new HcalDataFormatMonitor();
    m_dfMon->setup(ps, m_dbe);
  }

  if ( ps.getUntrackedParameter<bool>("PedestalMonitor", false) ) {
    m_pedMon = new HcalPedestalMonitor();
    m_pedMon->setup(ps, m_dbe);
  }

  if ( ps.getUntrackedParameter<bool>("LEDMonitor", false) ) {
    m_ledMon = new HcalLEDMonitor();
    m_ledMon->setup(ps, m_dbe);
  }

  offline_ = ps.getUntrackedParameter<bool>("OffLine", false);

  if ( m_dbe ) m_dbe->showDirStructure();
  
}

HcalMonitorModule::~HcalMonitorModule(){
  
  if(m_verbose) printf("HcalMonitorModule: Destructor.....");

  if ( offline_ ) sleep(35); 
  
  sleep(10);

  if(m_digiMon!=NULL) { delete m_digiMon; m_digiMon=NULL; }
  if(m_dfMon!=NULL) { delete m_dfMon; m_dfMon=NULL; }
  if(m_pedMon!=NULL) { delete m_pedMon; m_pedMon=NULL; }
  if(m_ledMon!=NULL) { delete m_ledMon; m_ledMon=NULL; }
  if(m_rhMon!=NULL) { delete m_rhMon; m_rhMon=NULL; }
  delete m_evtSel;

  if ( m_dbe ) {
    m_dbe->setCurrentFolder("HcalMonitor");
    m_dbe->removeContents();
  }

  m_logFile.close();
}

void HcalMonitorModule::beginJob(const edm::EventSetup& c){
  m_ievt = 0;
  
  if(m_verbose) cout << "HcalMonitorModule: begin job...." << endl;
  
  if ( m_meStatus ) m_meStatus->Fill(0);
  if ( m_meEvtNum ) m_meEvtNum->Fill(m_ievt);

  return;
}

void HcalMonitorModule::endJob(void) {

  if(m_verbose) cout << "HcalMonitorModule: end job...." << endl;  
  cout << "HcalMonitorModule::endJob, analyzed " << m_ievt << " events" << endl;
  
  if ( m_meStatus ) m_meStatus->Fill(2);
  if ( m_meRunNum ) m_meRunNum->Fill(m_runNum);
  if ( m_meEvtNum ) m_meEvtNum->Fill(m_ievt);

  sleep(10);  

  if(m_rhMon!=NULL) m_rhMon->done();
  if(m_digiMon!=NULL) m_digiMon->done();
  if(m_dfMon!=NULL) m_dfMon->done();
  if(m_pedMon!=NULL) m_pedMon->done();
  if(m_ledMon!=NULL) m_ledMon->done();

  if ( m_outputFile.size() != 0  && m_dbe ) m_dbe->save(m_outputFile);

  return;
}

void HcalMonitorModule::analyze(const edm::Event& e, const edm::EventSetup& eventSetup){

  if(m_verbose) cout << "HcalMonitorModule: analyze...." << endl;

  // Do default setup...
  m_ievt++;
  m_evtSel->processEvent(e);
  int evtMask = m_evtSel->getEventMask();

  edm::EventID id_ = e.id();
  m_runNum = (int)(id_.run());

  if ( m_dbe ){ 
    m_meStatus->Fill(1);
    m_meRunNum->Fill(m_runNum);
    m_meEvtNum->Fill(m_ievt);
    m_meEvtMask->Fill(evtMask);
  }
  
  // To get information from the event setup, you must request the "Record"
  // which contains it and then extract the object you need
  // edm::ESHandle<CaloGeometry> geometry;
  // eventSetup.get<IdealGeometryRecord>().get(geometry);

  // get the hcal mapping
  edm::ESHandle<HcalDbService> pSetup;
  eventSetup.get<HcalDbRecord>().get( pSetup );
  const HcalElectronicsMap* readoutMap=pSetup->getHcalMapping();
  
  // get conditions
  edm::ESHandle<HcalDbService> conditions;
  eventSetup.get<HcalDbRecord>().get(conditions);

  // get digis if necessary
  edm::Handle<HBHEDigiCollection> hbhe;
  edm::Handle<HODigiCollection> ho;
  edm::Handle<HFDigiCollection> hf;
  if((evtMask&DO_HCAL_DIGIMON) 
     || (evtMask&DO_HCAL_PED_CALIBMON) 
     || (evtMask&DO_HCAL_LED_CALIBMON)){
    try{e.getByType(hbhe);} catch(...){};
    try{e.getByType(hf);} catch(...){};
    try{e.getByType(ho);} catch(...){};
  }

  // Digi-dependent monitor tasks
  if((m_digiMon!=NULL) && (evtMask&DO_HCAL_DIGIMON)) m_digiMon->processEvent(*hbhe,*ho,*hf);
  if((m_pedMon!=NULL) && (evtMask&DO_HCAL_PED_CALIBMON)) m_pedMon->processEvent(*hbhe,*ho,*hf,*conditions);
  if((m_ledMon!=NULL) && (evtMask&DO_HCAL_LED_CALIBMON)) m_ledMon->processEvent(*hbhe,*ho,*hf,*conditions);

  // Data Format monitor task
  if((m_dfMon != NULL) && (evtMask&DO_HCAL_DFMON)){
    edm::Handle<FEDRawDataCollection> rawraw;  
    try{e.getByType(rawraw);} catch(...){};           
    m_dfMon->processEvent(*rawraw,*readoutMap);
  }

  // Rec Hit monitor task
  if((m_rhMon != NULL) && (evtMask&DO_HCAL_RECHITMON)){
    edm::Handle<HBHERecHitCollection> hb_hits;
    edm::Handle<HORecHitCollection> ho_hits;
    edm::Handle<HFRecHitCollection> hf_hits;
    try{e.getByType(hb_hits);} catch(...){}; 
    try{e.getByType(ho_hits);} catch(...){}; 
    try{e.getByType(hf_hits);} catch(...){}; 
    m_rhMon->processEvent(*hb_hits,*ho_hits,*hf_hits);
  }

  if(m_ievt%1000 == 0)
    cout << "HcalMonitorModule: analyzed " << m_ievt << " events" << endl;

  if(offline_) usleep(30);

  return;
}

// Here are the necessary incantations to declare your module to the
// framework, so it can be referenced in a cmsRun file.
//
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(HcalMonitorModule)

