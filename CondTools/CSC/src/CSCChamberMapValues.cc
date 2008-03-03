#include <memory>
#include "boost/shared_ptr.hpp"
#include <fstream>

#include "CondFormats/CSCObjects/interface/CSCChamberMap.h"
#include "CondFormats/DataRecord/interface/CSCChamberMapRcd.h"
#include "CondTools/CSC/interface/CSCChamberMapValues.h"
#include "CondTools/CSC/interface/CSCMap1.h"
  

CSCChamberMapValues::CSCChamberMapValues(const edm::ParameterSet& iConfig)
{
  //the following line is needed to tell the framework what
  // data is being produced
  mapObj = fillChamberMap();
  setWhatProduced(this,&CSCChamberMapValues::produceChamberMap);
  findingRecord<CSCChamberMapRcd>();
  //now do what ever other initialization is needed
}


CSCChamberMapValues::~CSCChamberMapValues()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  delete mapObj;
}


//
// member functions
//

// ------------ method called to produce the data  ------------
CSCChamberMapValues::ReturnType
CSCChamberMapValues::produceChamberMap(const CSCChamberMapRcd& iRecord)
{
  //need a new object so to not be deleted at exit
  CSCChamberMap* mydata=new CSCChamberMap( *mapObj );
  return mydata;
  
}

 void CSCChamberMapValues::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &, const edm::IOVSyncValue&,
 edm::ValidityInterval & oValidity)
 {
 oValidity = edm::ValidityInterval(edm::IOVSyncValue::beginOfTime(),edm::IOVSyncValue::endOfTime());
 
 }
