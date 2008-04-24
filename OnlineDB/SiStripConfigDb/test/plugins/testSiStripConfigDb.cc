// Last commit: $Id: testSiStripConfigDb.cc,v 1.6 2008/04/23 12:17:49 bainbrid Exp $

#include "OnlineDB/SiStripConfigDb/test/plugins/testSiStripConfigDb.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "OnlineDB/SiStripConfigDb/interface/SiStripConfigDb.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------
// 
testSiStripConfigDb::testSiStripConfigDb( const edm::ParameterSet& pset ) 
  : db_(0),
    download_( pset.getUntrackedParameter<bool>("Download",false) ),
    upload_( pset.getUntrackedParameter<bool>("Upload",false) ),
    devices_( pset.getUntrackedParameter<bool>("DeviceDescriptions",false) ),
    feds_( pset.getUntrackedParameter<bool>("FedDescriptions",false) ),
    conns_( pset.getUntrackedParameter<bool>("FedConnections",false) ),
    dcus_( pset.getUntrackedParameter<bool>("DcuDetIdMap",false) )
{
  std::stringstream ss;
  ss << "[testSiStripConfigDb::" << __func__ << "]"
     << " Parameters:" << std::endl 
     << "  Download           : " << download_ << std::endl 
     << "  Upload             : " << upload_ << std::endl 
     << "  FedConnections     : " << conns_ << std::endl 
     << "  DeviceDescriptions : " << devices_ << std::endl 
     << "  FedDescriptions    : " << feds_ << std::endl 
     << "  DcuDetIdMap        : " << dcus_;
  LogTrace(mlCabling_) << ss.str();
}

// -----------------------------------------------------------------------------
// 
testSiStripConfigDb::~testSiStripConfigDb() {
  LogTrace(mlCabling_)
    << "[testSiStripConfigDb::" << __func__ << "]"
    << " Destructing object...";
}

// -----------------------------------------------------------------------------
// 
void testSiStripConfigDb::beginJob( const edm::EventSetup& setup ) {

  
  // -------------------- INITIALISATION --------------------


  // Access service
  db_ = edm::Service<SiStripConfigDb>().operator->();

  // Check pointer
  if ( db_ ) {
    edm::LogVerbatim(mlCabling_)
      << "[testSiStripConfigDb::" << __func__ << "]"
      << " Pointer to SiStripConfigDb: 0x" 
      << std::setw(8) << std::setfill('0')
      << std::hex << db_ << std::dec;
  } else {
    edm::LogError(mlCabling_)
      << "[testSiStripConfigDb::" << __func__ << "]"
      << " NULL pointer to SiStripConfigDb!"
      << " Aborting!";
    return;
  }

  // Check pointer
  if ( db_->deviceFactory() || db_->databaseCache() ) {
    std::stringstream ss;
    ss << "[testSiStripConfigDb::" << __func__ << "]" << std::endl
       << " Pointer to DeviceFactory: 0x" 
       << std::setw(8) << std::setfill('0') 
       << std::hex << db_->deviceFactory() << std::dec
       << std::endl
       << " Pointer to DatabaseCache: 0x" 
       << std::setw(8) << std::setfill('0') 
       << std::hex << db_->databaseCache() << std::dec
       << std::endl;
    edm::LogVerbatim(mlCabling_) << ss.str();
  } else {
    edm::LogError(mlCabling_)
      << "[testSiStripConfigDb::" << __func__ << "]"
      << " NULL pointer to DeviceFactory AND DatabaseCache!"
      << " Aborting!";
    return;
  }

  // Local caches
  std::stringstream ss;
  SiStripConfigDb::DeviceDescriptions devices;
  SiStripConfigDb::FedDescriptions feds;
  SiStripConfigDb::FedConnections conns;
  SiStripConfigDb::DcuDetIdMap dcus;


  // -------------------- UPLOADS (download, then upload) --------------------
  

  if ( upload_ ) {

    // Connections
    if ( conns_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearFedConnections();
      SiStripConfigDb::FedConnections connections;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions_.end();
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::FedConnections::range conns = db_->getFedConnections( ii->second.partitionName_ );
	if ( conns != connections.emptyRange() ) {
	  std::vector<SiStripConfigDb::FedConnection*> tmp1( conns.begin(), conns.end() );
	  std::vector<SiStripConfigDb::FedConnection*> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  ConnectionFactory::vectorCopyI( tmp2, tmp1, true );
#else
	  tmp2 = tmp1;
#endif
	  connections.loadNext( ii->second.partitionName_, tmp2 );
	}
      }
      db_->printFedConnections();
      db_->clearFedConnections();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedConnections::range conns = connections.find( iter->second.partitionName_ );
	std::vector<SiStripConfigDb::FedConnection*> temp( conns.begin(), conns.end() );
	db_->addFedConnections( iter->second.partitionName_, temp );
	db_->printFedConnections( iter->second.partitionName_ );
	db_->uploadFedConnections( iter->second.partitionName_ );
      }

      // print all partitions and then upload, clear, print
      db_->printFedConnections();
      db_->uploadFedConnections();
      db_->clearFedConnections();
      db_->printFedConnections();
      
    }

    // Devices
    if ( devices_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearDeviceDescriptions();
      SiStripConfigDb::DeviceDescriptions devices;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions_.end();
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::DeviceDescriptions::range devs = db_->getDeviceDescriptions( ii->second.partitionName_ );
	if ( devs != devices.emptyRange() ) {
	  std::vector<SiStripConfigDb::DeviceDescription*> tmp1( devs.begin(), devs.end() );
	  std::vector<SiStripConfigDb::DeviceDescription*> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  FecFactory::vectorCopyI( tmp2, tmp1, true );
#else
	  tmp2 = tmp1;
#endif
	  devices.loadNext( ii->second.partitionName_, tmp2 );
	}
      }
      db_->printDeviceDescriptions();
      db_->clearDeviceDescriptions();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DeviceDescriptions::range devs = devices.find( iter->second.partitionName_ );
	std::vector<SiStripConfigDb::DeviceDescription*> temp( devs.begin(), devs.end() );
	db_->addDeviceDescriptions( iter->second.partitionName_, temp );
	db_->printDeviceDescriptions( iter->second.partitionName_ );
	db_->uploadDeviceDescriptions( iter->second.partitionName_ );
      }

      // print all partitions and then upload, clear, print
      db_->printDeviceDescriptions();
      db_->uploadDeviceDescriptions();
      db_->clearDeviceDescriptions();
      db_->printDeviceDescriptions();
      
    }

    // FEDs
    if ( feds_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearFedDescriptions();
      SiStripConfigDb::FedDescriptions feds;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions_.end();
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::FedDescriptions::range range = db_->getFedDescriptions( ii->second.partitionName_ );
	if ( range != feds.emptyRange() ) {
	  std::vector<SiStripConfigDb::FedDescription*> tmp1( range.begin(), range.end() );
	  std::vector<SiStripConfigDb::FedDescription*> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  Fed9U::Fed9UDeviceFactory::vectorCopy( tmp2, tmp1 );
#else
	  tmp2 = tmp1;
#endif
	  feds.loadNext( ii->second.partitionName_, tmp2 );
	}
      }
      db_->printFedDescriptions();
      db_->clearFedDescriptions();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedDescriptions::range range = feds.find( iter->second.partitionName_ );
	std::vector<SiStripConfigDb::FedDescription*> temp( range.begin(), range.end() );
	db_->addFedDescriptions( iter->second.partitionName_, temp );
	db_->printFedDescriptions( iter->second.partitionName_ );
	db_->uploadFedDescriptions( iter->second.partitionName_ );
      }
      
      // print all partitions and then upload, clear, print
      db_->printFedDescriptions();
      db_->uploadFedDescriptions();
      db_->clearFedDescriptions();
      db_->printFedDescriptions();
      
    }

    // DCU-DetId
    if ( dcus_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearDcuDetIdMap();
      SiStripConfigDb::DcuDetIdMap feds;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions_.end();
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::DcuDetIdMap::range range = db_->getDcuDetIdMap( ii->second.partitionName_ );
	if ( range != feds.emptyRange() ) {
	  std::vector<SiStripConfigDb::DcuDetId> tmp1( range.begin(), range.end() );
	  std::vector<SiStripConfigDb::DcuDetId> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  db_->clone( tmp1, tmp2 );
#else
	  tmp2 = tmp1;
#endif
	  feds.loadNext( ii->second.partitionName_, tmp2 );
	}
      }
      db_->printDcuDetIdMap();
      db_->clearDcuDetIdMap();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DcuDetIdMap::range range = feds.find( iter->second.partitionName_ );
	std::vector<SiStripConfigDb::DcuDetId> temp( range.begin(), range.end() );
	db_->addDcuDetIdMap( iter->second.partitionName_, temp );
	db_->printDcuDetIdMap( iter->second.partitionName_ );
	db_->uploadDcuDetIdMap( iter->second.partitionName_ );
      }
      
      // print all partitions and then upload, clear, print
      db_->printDcuDetIdMap();
      db_->uploadDcuDetIdMap();
      db_->clearDcuDetIdMap();
      db_->printDcuDetIdMap();
      
    }

  }
  
  
  // -------------------- DOWNLOADS (just download) --------------------
  

  if ( download_ ) {
    
    // Connections
    if ( conns_ ) {

      // iterate through partitions and get, print, clear, print
      db_->clearFedConnections();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedConnections::range conns = db_->getFedConnections( iter->second.partitionName_ );
	db_->printFedConnections( iter->second.partitionName_ );
	db_->clearFedConnections( iter->second.partitionName_ );
	db_->printFedConnections( iter->second.partitionName_ );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << conns.size()
	   << " FED connections!";
	if ( !conns.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      }

      // get all partitions and print, clear, print
      SiStripConfigDb::FedConnections::range conns = db_->getFedConnections();
      db_->printFedConnections();
      db_->clearFedConnections();
      db_->printFedConnections();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << conns.size()
	 << " FED connections!";
      if ( !conns.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }

    }

    // Devices
    if ( devices_ ) {

      // iterate through partitions and get, print, clear, print
      db_->clearDeviceDescriptions();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DeviceDescriptions::range devs = db_->getDeviceDescriptions( iter->second.partitionName_ );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << devs.size()
	   << " device descriptions!";
	SiStripConfigDb::DeviceDescriptionsRange apv = db_->getDeviceDescriptions( APV25, iter->second.partitionName_ );
	SiStripConfigDb::DeviceDescriptionsRange mux = db_->getDeviceDescriptions( APVMUX, iter->second.partitionName_ );
	SiStripConfigDb::DeviceDescriptionsRange dcu = db_->getDeviceDescriptions( DCU, iter->second.partitionName_ );
	SiStripConfigDb::DeviceDescriptionsRange lld = db_->getDeviceDescriptions( LASERDRIVER, iter->second.partitionName_ );
	SiStripConfigDb::DeviceDescriptionsRange doh = db_->getDeviceDescriptions( DOH, iter->second.partitionName_ );
	SiStripConfigDb::DeviceDescriptionsRange pll = db_->getDeviceDescriptions( PLL, iter->second.partitionName_ );
	if ( !devs.empty() ) { 
	  ss << std::endl
	     << " Number of APV descriptions : " << ( apv.second - apv.first ) << std::endl
	     << " Number of MUX descriptions : " << ( mux.second - mux.first ) << std::endl
	     << " Number of DCU descriptions : " << ( dcu.second - dcu.first ) << std::endl
	     << " Number of LLD descriptions : " << ( lld.second - lld.first ) << std::endl
	     << " Number of DOH descriptions : " << ( doh.second - doh.first ) << std::endl
	     << " Number of PLL descriptions : " << ( pll.second - pll.first );
	  edm::LogVerbatim("testSiStripConfigDb") << ss.str(); 
	}
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printDeviceDescriptions( iter->second.partitionName_ );
	db_->clearDeviceDescriptions( iter->second.partitionName_ );
	db_->printDeviceDescriptions( iter->second.partitionName_ );
      }

      // get all partitions and print, clear, print
      SiStripConfigDb::DeviceDescriptions::range devs = db_->getDeviceDescriptions();
      db_->printDeviceDescriptions();
      db_->clearDeviceDescriptions();
      db_->printDeviceDescriptions();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << devs.size()
	 << " device descriptions!";
      if ( !devs.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }

    }

    // FEDs
    if ( feds_ ) {
      
      // iterate through partitions and get, print, clear, print
      db_->clearFedDescriptions();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedDescriptions::range feds = db_->getFedDescriptions( iter->second.partitionName_ );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << feds.size()
	   << " FED descriptions!";
	SiStripConfigDb::FedIdsRange ids = db_->getFedIds( iter->second.partitionName_ );
	if ( !feds.empty() ) { 
	  ss << std::endl << " Number of FED ids : " << ( ids.second - ids.first );
	  edm::LogVerbatim("testSiStripConfigDb") << ss.str(); 
	}
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printFedDescriptions( iter->second.partitionName_ );
	db_->clearFedDescriptions( iter->second.partitionName_ );
	db_->printFedDescriptions( iter->second.partitionName_ );
      }
      
      // get all partitions and print, clear, print
      SiStripConfigDb::FedDescriptions::range feds = db_->getFedDescriptions();
      db_->printFedDescriptions();
      db_->clearFedDescriptions();
      db_->printFedDescriptions();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << feds.size()
	 << " FED descriptions!";
      if ( !feds.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      
    }

    // DCU-DetId 
    if ( dcus_ ) {
      
      // iterate through partitions and get, print, clear, print
      db_->clearDcuDetIdMap();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions_.begin();
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions_.end();
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DcuDetIdMap::range range = db_->getDcuDetIdMap( iter->second.partitionName_ );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << range.size()
	   << " DCU-DetId map!";
	if ( !range.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printDcuDetIdMap( iter->second.partitionName_ );
	db_->clearDcuDetIdMap( iter->second.partitionName_ );
	db_->printDcuDetIdMap( iter->second.partitionName_ );
      }
      
      // get all partitions and print, clear, print
      SiStripConfigDb::DcuDetIdMap::range range = db_->getDcuDetIdMap();
      db_->printDcuDetIdMap();
      db_->clearDcuDetIdMap();
      db_->printDcuDetIdMap();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << range.size()
	 << " DCU-DetId map!";
      if ( !range.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      
    }
    
  }
  
}
