#include "IOPool/Streamer/interface/MsgTools.h"
#include "IOPool/Streamer/interface/StreamerInputFile.h"
#include "IOPool/Streamer/src/StreamerFileReader.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Catalog/interface/InputFileCatalog.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Sources/interface/EventSkipperByID.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"

namespace edm {
  StreamerFileReader::StreamerFileReader(ParameterSet const& pset, InputSourceDescription const& desc) :
      StreamerInputSource(pset, desc),
      streamerNames_(pset.getUntrackedParameter<std::vector<std::string> >("fileNames")),
      streamReader_(),
      eventSkipperByID_(EventSkipperByID::create(pset).release()),
      numberOfEventsToSkip_(pset.getUntrackedParameter<unsigned int>("skipEvents", 0U)) {
    InputFileCatalog catalog(pset);
    streamerNames_ = catalog.fileNames();

    if (streamerNames_.size() > 1) {
      streamReader_ = std::auto_ptr<StreamerInputFile>(new StreamerInputFile(streamerNames_, &numberOfEventsToSkip_, eventSkipperByID_));
    } else if (streamerNames_.size() == 1) {
      streamReader_ = std::auto_ptr<StreamerInputFile>(new StreamerInputFile(streamerNames_.at(0), &numberOfEventsToSkip_, eventSkipperByID_));
    } else {
      throw Exception(errors::FileReadError, "StreamerFileReader::StreamerFileReader")
         << "No fileNames were specified\n";
    }
    InitMsgView const* header = getHeader();
    deserializeAndMergeWithRegistry(*header, false);
  }

  StreamerFileReader::~StreamerFileReader() {
  }

  EventPrincipal*
  StreamerFileReader::read() {

    EventMsgView const* eview = getNextEvent();

    if (newHeader()) {
        // FDEBUG(6) << "A new file has been opened and we must compare Headers here !!" << std::endl;
        // A new file has been opened and we must compare Heraders here !!
        //Get header/init from reader
        InitMsgView const* header = getHeader();
        deserializeAndMergeWithRegistry(*header, true);
    }
    if (eview == 0) {
        return  0;
    }
    return(deserializeEvent(*eview));
  }


  bool const
  StreamerFileReader::newHeader() {
    return streamReader_->newHeader();
  }

  InitMsgView const*
  StreamerFileReader::getHeader() {

    InitMsgView const* header = streamReader_->startMessage();

    if(header->code() != Header::INIT) { //INIT Msg
      throw Exception(errors::FileReadError, "StreamerFileReader::readHeader")
        << "received wrong message type: expected INIT, got "
        << header->code() << "\n";
    }

    return header;
  }

  EventMsgView const*
  StreamerFileReader::getNextEvent() {
    if (!streamReader_->next()) {
      return 0;
    }
    return streamReader_->currentRecord();
  }

} //end-of-namespace


