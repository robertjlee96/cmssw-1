#include "SiPixelRawToDigi.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigiCollection.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelFedCablingMap.h"
#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"

#include "EventFilter/SiPixelRawToDigi/interface/PixelDataFormatter.h"

#include "EventFilter/SiPixelRawToDigi/interface/R2DTimerObserver.h"

#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"

using namespace std;

// -----------------------------------------------------------------------------
SiPixelRawToDigi::SiPixelRawToDigi( const edm::ParameterSet& conf ) 
  : eventCounter_(0), 
    config_(conf),
    fedCablingMap_(0),
    hCPU(0), hDigi(0), rootFile(0),
    theTimer(0)
{
  edm::LogInfo("SiPixelRawToDigi")<< " HERE ** constructor!" << endl;
  produces< edm::DetSetVector<PixelDigi> >();

  bool timing = config_.getUntrackedParameter<bool>("Timing",false);
  cout <<"HERE timing is: " << timing << endl;
  if (timing) {
    theTimer = new R2DTimerObserver("**** MY TIMING REPORT ***");
    rootFile = new TFile("analysis.root", "RECREATE", "my histograms");
    hCPU = new TH1D ("hCPU","hCPU",60,0.,0.030);
    hDigi = new TH1D("hDigi","hDigi",50,0.,15000.);
  }
}


// -----------------------------------------------------------------------------
SiPixelRawToDigi::~SiPixelRawToDigi() {
  edm::LogInfo("SiPixelRawToDigi")  << " HERE ** SiPixelRawToDigi destructor!";

  if (theTimer) {
    rootFile->Write();
    delete theTimer;
  }

  cout << " end.."<<endl;
}


// -----------------------------------------------------------------------------
void SiPixelRawToDigi::beginJob(const edm::EventSetup& c) 
{
}

// -----------------------------------------------------------------------------
void SiPixelRawToDigi::produce( edm::Event& ev,
                              const edm::EventSetup& es) 
{

  edm::ESHandle<SiPixelFedCablingMap> map;
  es.get<SiPixelFedCablingMapRcd>().get( map );
  cout << map->version() << endl;

  edm::Handle<FEDRawDataCollection> buffers;
  static string label = config_.getUntrackedParameter<string>("InputLabel","source");
  static string instance = config_.getUntrackedParameter<string>("InputInstance","");
  ev.getByLabel( label, instance, buffers);

// create product (digis)
  std::auto_ptr< edm::DetSetVector<PixelDigi> > collection( new edm::DetSetVector<PixelDigi> );
  static int ndigis = 0;
  static int nwords = 0;

  PixelDataFormatter formatter(map.product());

  if (theTimer) theTimer->start();

  FEDNumbering fednum;
  pair<int,int> fedIds = fednum.getSiPixelFEDIds();
  fedIds.first = 0;
  fedIds.second = 39; //  temporary FIX !!!!
  
  for (int fedId = fedIds.first; fedId <= fedIds.second; fedId++) {
    LogDebug("SiPixelRawToDigi")<< " PRODUCE DIGI FOR FED: " <<  fedId << endl;
    PixelDataFormatter::Digis digis;
     
    //get event data for this fed
    const FEDRawData& fedRawData = buffers->FEDData( fedId );

    //convert data to digi
    formatter.interpretRawData( fedId, fedRawData, digis);

    //pack digi into collection
    typedef PixelDataFormatter::Digis::iterator ID;
    for (ID it = digis.begin(); it != digis.end(); it++) {
//      uint32_t detid = it->id;
      uint32_t detid = it->first;
      edm::DetSet<PixelDigi>& detSet = collection->find_or_insert(detid);
//      detSet.data = it->data;
      detSet.data = it->second;
    } 
  }

  if (theTimer) {
    theTimer->stop();
    cout << "TIMING IS: (real)" << theTimer->lastMeasurement().real() << endl;
    ndigis += formatter.nDigis();
    nwords += formatter.nWords();
    cout << " (Words/Digis) this ev: "<<formatter.nWords()<<"/"<<formatter.nDigis()
         << "--- all :"<<nwords<<"/"<<ndigis<<endl;
    hCPU->Fill( theTimer->lastMeasurement().real() ); 
    hDigi->Fill(formatter.nDigis());
  }

  //send digis back to framework 
  ev.put( collection );
}

