#ifndef DataFormats_SiStripDigi_Classes_H
#define DataFormats_SiStripDigi_Classes_H

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include <vector>
#include <string>

#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
namespace {
  namespace {
    edm::Wrapper< SiStripDigi > zs0;
    edm::Wrapper< std::vector<SiStripDigi>  > zs1;
    edm::Wrapper< edm::DetSet<SiStripDigi> > zs2;
    edm::Wrapper< std::vector<edm::DetSet<SiStripDigi> > > zs3;
    edm::Wrapper< edm::DetSetVector<SiStripDigi> > zs4;
  }
}

#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
namespace {
  namespace {
    edm::Wrapper< SiStripRawDigi > raw0;
    edm::Wrapper< std::vector<SiStripRawDigi>  > raw1;
    edm::Wrapper< edm::DetSet<SiStripRawDigi> > raw2;
    edm::Wrapper< std::vector<edm::DetSet<SiStripRawDigi> > > raw3;
    edm::Wrapper< edm::DetSetVector<SiStripRawDigi> > raw4;
  }
}

#include "DataFormats/SiStripDigi/interface/SiStripDigis.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
namespace {
  namespace {
    edm::Wrapper< SiStripDigis > digis;
    edm::Wrapper< FEDRawDataCollection > fed_buffers;
    edm::RefProd< FEDRawDataCollection > ref_to_fed_buffers;
    edm::Wrapper< std::vector< bool > > list_of_fed_id;
/*     edm::Wrapper< unsigned long > appended_bytes;  */
/*     edm::Wrapper< unsigned long > daq_hdr_pos;  */
/*     edm::Wrapper< unsigned long > trk_hdr_pos;  */
/*     edm::Wrapper< unsigned long > apv_error_hdr_pos;  */
/*     edm::Wrapper< unsigned long > full_debug_hdr_pos;  */
/*     edm::Wrapper< unsigned long > apv_error_hdr_size;  */
/*     edm::Wrapper< unsigned long > full_debug_hdr_size;  */
/*     edm::Wrapper< std::string >   error;  */
  }
}

#include "DataFormats/SiStripDigi/interface/SiStripEventSummary.h"
#include "DataFormats/SiStripCommon/interface/SiStripEnumeratedTypes.h"
namespace {
  namespace {
    edm::Wrapper< sistrip::Task > task;
    edm::Wrapper< sistrip::FedReadoutMode > fed_mode;
    edm::Wrapper< SiStripEventSummary > summary;

  }
}

#endif // DataFormats_SiStripDigi_Classes_H


 
