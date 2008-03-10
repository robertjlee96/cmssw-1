#include "EventFilter/RPCRawToDigi/interface/RPCPackingModule.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CondFormats/RPCObjects/interface/RPCEMap.h"
#include "CondFormats/DataRecord/interface/RPCEMapRcd.h"

#include "EventFilter/RPCRawToDigi/interface/RPCRecordFormatter.h"

#include <string>
#include <sstream>


using namespace std;
using namespace edm;
using namespace rpcrawtodigi;

typedef uint64_t Word64;

RPCPackingModule::RPCPackingModule( const ParameterSet& pset ) :
//  digiLabel_(""),
  eventCounter_(0)
{

  cabling = new RPCReadOutMapping("");
  // Set some private data members
//  digiLabel_ = pset.getParameter<InputTag>("DigiProducer");

  // Define EDProduct type
  produces<FEDRawDataCollection>();

}

RPCPackingModule::~RPCPackingModule(){}


void RPCPackingModule::produce( edm::Event& ev,
                              const edm::EventSetup& es)
{
  eventCounter_++;
  LogInfo("RPCPackingModule") << "[RPCPackingModule::produce] " 
                              << "event counter: " << eventCounter_;

  Handle< RPCDigiCollection > digiCollection;
  ev.getByType(digiCollection);

//  ESHandle<RPCReadOutMapping> readoutMapping;
//  es.get<RPCReadOutMappingRcd>().get(readoutMapping);
  ESHandle<RPCEMap> readoutMapping;
  es.get<RPCEMapRcd>().get(readoutMapping);
  const RPCEMap* eMap=readoutMapping.product();

  if (eMap->theVersion != cabling->version()) {
    delete cabling;
    cabling = eMap->convert();
  }

  auto_ptr<FEDRawDataCollection> buffers( new FEDRawDataCollection );

//  pair<int,int> rpcFEDS=FEDNumbering::getRPCFEDIds();
  pair<int,int> rpcFEDS(790,792);
  for (int id= rpcFEDS.first; id<=rpcFEDS.second; ++id){

//    RPCRecordFormatter formatter(id, readoutMapping.product()) ;
    RPCRecordFormatter formatter(id, cabling) ;
    unsigned int lvl1_ID = ev.id().event();
    FEDRawData *  rawData =  RPCPackingModule::rawData(id, lvl1_ID, digiCollection.product(), formatter);
    FEDRawData& fedRawData = buffers->FEDData(id);

    fedRawData = *rawData;
    delete rawData;
  }
  ev.put( buffers );  
}


FEDRawData * RPCPackingModule::rawData( int fedId, unsigned int lvl1_ID, const RPCDigiCollection * digis, const RPCRecordFormatter & formatter)
{
  //
  // get merged records
  //
  int trigger_BX = 200;   // FIXME - set event by event but correct bx assigment in digi
  vector<EventRecords> merged = RPCPackingModule::eventRecords(fedId,trigger_BX,digis,formatter);

  //
  // create data words
  //
  vector<Word64> dataWords;
  DataRecord empty;
  typedef vector<EventRecords>::const_iterator IR;
  for (IR ir = merged.begin(), irEnd =  merged.end() ; ir != irEnd; ++ir) {
    Word64 w = ( ( (Word64(ir->recordBX().data()) << 16) | ir->recordSLD().data() ) << 16
                    | ir->recordCD().data() ) << 16 | empty.data();
    dataWords.push_back(w);
  }

  //
  // create raw data
  //
  int nHeaders = 1;
  int nTrailers = 1;
  int dataSize = (nHeaders+nTrailers+dataWords.size()) * sizeof(Word64);
  FEDRawData * raw = new FEDRawData(dataSize);

  //
  // add header
  //
  unsigned char *pHeader  = raw->data();
  int evt_ty = 3;
  int source_ID = fedId;
  FEDHeader::set(pHeader, evt_ty, lvl1_ID, trigger_BX, source_ID);

  //
  // add datawords
  //
  for (unsigned int idata = 0; idata < dataWords.size(); idata ++) {
    Word64 * word = reinterpret_cast<Word64* >(pHeader+(idata+1)*sizeof(Word64));
    *word = dataWords[idata];
  }

  //
  // add trailer
  //
  unsigned char *pTrailer = pHeader + raw->size()-sizeof(Word64);
  int crc = 0;
  int evt_stat = 15;
  int tts = 0;
  int datasize =  raw->size()/sizeof(Word64);
  FEDTrailer::set(pTrailer, datasize, crc, evt_stat, tts);

  return raw;
}

vector<EventRecords> RPCPackingModule::eventRecords(
    int fedId, 
    int trigger_BX, 
    const RPCDigiCollection* digis , 
    const RPCRecordFormatter& formatter)
{
  typedef  DigiContainerIterator<RPCDetId, RPCDigi> DigiRangeIterator;
  vector<EventRecords> dataRecords;


  LogDebug("RPCRawDataPacker")<<"Packing Fed id="<<fedId;
  for (DigiRangeIterator it=digis->begin(); it != digis->end(); it++) {
    RPCDetId rpcDetId = (*it).first;
    uint32_t rawDetId = rpcDetId.rawId();
    RPCDigiCollection::Range range = digis->get(rpcDetId);
    for (vector<RPCDigi>::const_iterator  id = range.first; id != range.second; id++) {
      const RPCDigi & digi = (*id);
      vector<EventRecords>  rawFromDigi =  formatter.recordPack(rawDetId, digi, trigger_BX);
      dataRecords.insert(dataRecords.end(), rawFromDigi.begin(), rawFromDigi.end());
    }
  }

  //
  // merge data words
  //
  LogTrace("RPCRawDataPacker") <<" size of   data: " << dataRecords.size();
  vector<EventRecords> merged = EventRecords::mergeRecords(dataRecords);
  LogTrace("") <<" size of megred: " << merged.size();

  return merged;
}
