/*----------------------------------------------------------------------
$Id: RootInputFileSequence.cc,v 1.77 2008/01/08 06:57:39 wmtan Exp $
----------------------------------------------------------------------*/
#include "RootInputFileSequence.h"
#include "PoolSource.h"
#include "RootFile.h"
#include "RootTree.h"

#include "FWCore/Catalog/interface/FileCatalog.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "CLHEP/Random/RandFlat.h"
#include "TTree.h"
#include "TFile.h"

namespace edm {
  RootInputFileSequence::RootInputFileSequence(
		ParameterSet const& pset,
		PoolSource const& input,
		InputFileCatalog const& catalog) :
    input_(input),
    catalog_(catalog),
    firstFile_(true),
    fileIterBegin_(fileCatalogItems().begin()),
    fileIter_(fileCatalogItems().end()),
    rootFile_(),
    matchMode_(BranchDescription::Permissive),
    flatDistribution_(0),
    eventsRemainingInFile_(0),
    startAtRun_(pset.getUntrackedParameter<unsigned int>("firstRun", 1U)),
    startAtLumi_(pset.getUntrackedParameter<unsigned int>("firstLuminosityBlock", 1U)),
    startAtEvent_(pset.getUntrackedParameter<unsigned int>("firstEvent", 1U)),
    eventsToSkip_(pset.getUntrackedParameter<unsigned int>("skipEvents", 0U)),
    skipBadFiles_(pset.getUntrackedParameter<bool>("skipBadFiles", false)),
    forcedRunOffset_(0),
    setRun_(pset.getUntrackedParameter<unsigned int>("setRunNumber", 0)) {

    std::string matchMode = pset.getUntrackedParameter<std::string>("fileMatchMode", std::string("permissive"));
    if (matchMode == std::string("strict")) matchMode_ = BranchDescription::Strict;
    if (primary()) {
      for(fileIter_ = fileIterBegin_; fileIter_ != fileCatalogItems().end(); ++fileIter_) {
        initFile(skipBadFiles_);
        if (rootFile_) break;
      }
      if (rootFile_) {
        forcedRunOffset_ = rootFile_->setForcedRunOffset(setRun_);
        if (forcedRunOffset_ < 0) {
          throw cms::Exception("Configuration")
            << "The value of the 'setRunNumber' parameter must not be\n"
            << "less than the first run number in the first input file.\n"
            << "'setRunNumber' was " << setRun_ <<", while the first run was "
            << setRun_ - forcedRunOffset_ << ".\n";
        }
        updateProductRegistry();
      }
    } else {
      Service<RandomNumberGenerator> rng;
      if (!rng.isAvailable()) {
        throw cms::Exception("Configuration")
          << "A secondary input source requires the RandomNumberGeneratorService\n"
          << "which is not present in the configuration file.  You must add the service\n"
          << "in the configuration file or remove the modules that require it.";
      }
      CLHEP::HepRandomEngine& engine = rng->getEngine();
      flatDistribution_ = new CLHEP::RandFlat(engine);
    }
  }

  std::vector<FileCatalogItem> const&
  RootInputFileSequence::fileCatalogItems() const {
    return catalog_.fileCatalogItems();
  }

  void
  RootInputFileSequence::endJob() {
    closeFile_();
  }

  boost::shared_ptr<FileBlock>
  RootInputFileSequence::readFile_() {
    if (firstFile_) {
      // The first input file has already been opened.
      firstFile_ = false;
    } else {
      if (!nextFile()) {
      assert(0);
      }
    }
    if (!rootFile_) {
      return boost::shared_ptr<FileBlock>(new FileBlock);
    }
    return rootFile_->createFileBlock();
  }

  void RootInputFileSequence::closeFile_() {
    if (rootFile_) {
    // Account for events skipped in the file.
      eventsToSkip_ = rootFile_->eventsToSkip();
      rootFile_->close(true);
      rootFile_.reset();
    }
  }

  void RootInputFileSequence::initFile(bool skipBadFiles) {
    TTree::SetMaxTreeSize(kMaxLong64);
    boost::shared_ptr<TFile> filePtr;
    try {
      filePtr = boost::shared_ptr<TFile>(TFile::Open(fileIter_->fileName().c_str()));
    } catch (cms::Exception) {
      if (!skipBadFiles) throw;
    }
    if (filePtr && !filePtr->IsZombie()) {
      rootFile_ = RootFileSharedPtr(new RootFile(fileIter_->fileName(), catalog_.url(),
	  processConfiguration(), fileIter_->logicalFileName(), filePtr,
	  startAtRun_, startAtLumi_, startAtEvent_, eventsToSkip_, remainingEvents(),
	  forcedRunOffset_));
    } else {
      if (!skipBadFiles) {
	throw edm::Exception(edm::errors::FatalRootError) <<
	   "RootInputFileSequence::initFile(): Input file " << fileIter_->fileName() << " was not found or could not be opened.\n";
      }
      LogWarning("") << "Input file: " << fileIter_->fileName() << " was not found or could not be opened, and will be skipped.\n";
    }
  }

  void RootInputFileSequence::updateProductRegistry() const {
    if (rootFile_->productRegistry()->nextID() > productRegistry()->nextID()) {
      productRegistryUpdate().setNextID(rootFile_->productRegistry()->nextID());
    }
    ProductRegistry::ProductList const& prodList = rootFile_->productRegistry()->productList();
    for (ProductRegistry::ProductList::const_iterator it = prodList.begin(), itEnd = prodList.end();
	it != itEnd; ++it) {
      productRegistryUpdate().copyProduct(it->second);
    }
  }

  bool RootInputFileSequence::nextFile() {
    if(fileIter_ != fileCatalogItems().end()) ++fileIter_;
    if(fileIter_ == fileCatalogItems().end()) {
      if (primary()) {
	return false;
      } else {
	fileIter_ = fileIterBegin_;
      }
    }

    if (rootFile_) {
      rootFile_->close(primary());
      rootFile_.reset();
    }

    initFile(skipBadFiles_);

    if (primary() && rootFile_) {
      // make sure the new product registry is compatible with the main one
      std::string mergeInfo = productRegistryUpdate().merge(*rootFile_->productRegistry(),
							    fileIter_->fileName(),
							    matchMode_);
      if (!mergeInfo.empty()) {
        throw cms::Exception("MismatchedInput","RootInputFileSequence::nextFile()") << mergeInfo;
      }
    }
    return true;
  }

  bool RootInputFileSequence::previousFile() {
    if(fileIter_ == fileIterBegin_) {
      if (primary()) {
	return false;
      } else {
	fileIter_ = fileCatalogItems().end();
      }
    }
    --fileIter_;

    if (rootFile_) {
      rootFile_->close(primary());
      rootFile_.reset();
    }

    initFile(false);

    if (primary() && rootFile_) {
      // make sure the new product registry is compatible to the main one
      std::string mergeInfo = productRegistryUpdate().merge(*rootFile_->productRegistry(),
							    fileIter_->fileName(),
							    matchMode_);
      if (!mergeInfo.empty()) {
        throw cms::Exception("MismatchedInput","RootInputFileSequence::previousEvent()") << mergeInfo;
      }
    }
    if (rootFile_) rootFile_->setToLastEntry();
    return true;
  }

  RootInputFileSequence::~RootInputFileSequence() {
  }

  boost::shared_ptr<RunPrincipal>
  RootInputFileSequence::readRun_() {
    return rootFile_->readRun(primary() ? productRegistry() : rootFile_->productRegistry()); 
  }

  boost::shared_ptr<LuminosityBlockPrincipal>
  RootInputFileSequence::readLuminosityBlock_() {
    return rootFile_->readLumi(primary() ? productRegistry() : rootFile_->productRegistry(), runPrincipal()); 
  }

  // readEvent_() is responsible for creating, and setting up, the
  // EventPrincipal.
  //
  //   1. create an EventPrincipal with a unique EventID
  //   2. For each entry in the provenance, put in one Group,
  //      holding the Provenance for the corresponding EDProduct.
  //   3. set up the caches in the EventPrincipal to know about this
  //      Group.
  //
  // We do *not* create the EDProduct instance (the equivalent of reading
  // the branch containing this EDProduct. That will be done by the Delayed Reader,
  //  when it is asked to do so.
  //

  std::auto_ptr<EventPrincipal>
  RootInputFileSequence::readEvent_(boost::shared_ptr<LuminosityBlockPrincipal> lbp) {
    return rootFile_->readEvent(primary() ? productRegistry() : rootFile_->productRegistry(), lbp); 
  }

  std::auto_ptr<EventPrincipal>
  RootInputFileSequence::readCurrentEvent() {
    return rootFile_->readCurrentEvent(primary() ?
				       productRegistry() :
				       rootFile_->productRegistry(), boost::shared_ptr<LuminosityBlockPrincipal>()); 
  }

  std::auto_ptr<EventPrincipal>
  RootInputFileSequence::readIt(EventID const& id) {
    rootFile_->setEntryAtEvent(id);
    return readCurrentEvent();
  }

  InputSource::ItemType
  RootInputFileSequence::getNextItemType() {
    if (fileIter_ == fileCatalogItems().end()) {
      return InputSource::IsStop;
    }
    if (firstFile_) {
      return InputSource::IsFile;
    }
    if (rootFile_) {
      FileIndex::EntryType entryType = rootFile_->getEntryTypeSkippingDups();
      if (entryType == FileIndex::kEvent) {
        return InputSource::IsEvent;
      } else if (entryType == FileIndex::kLumi) {
        return InputSource::IsLumi;
      } else if (entryType == FileIndex::kRun) {
        return InputSource::IsRun;
      }
      assert(entryType == FileIndex::kEnd);
    }
    if (fileIter_ + 1 == fileCatalogItems().end()) {
      return InputSource::IsStop;
    }
    return InputSource::IsFile;
  }

  // Rewind to before the first event that was read.
  void
  RootInputFileSequence::rewind_() {
    if (rootFile_) {
	rootFile_->close(primary());
	rootFile_.reset();
    }
    fileIter_ = fileIterBegin_;
    initFile(skipBadFiles_);
  }

  // Rewind to the beginning of the current file
  void
  RootInputFileSequence::rewindFile() {
    rootFile_->rewind();
  }

  // Advance "offset" events.  Offset can be positive or negative (or zero).
  void
  RootInputFileSequence::skip(int offset) {
    while (offset != 0) {
      offset = rootFile_->skipEvents(offset);
      if (offset > 0 && !nextFile()) return;
      if (offset < 0 && !previousFile()) return;
    }
  }

  bool const
  RootInputFileSequence::primary() const {
    return input_.primary();
  }

  boost::shared_ptr<RunPrincipal>
  RootInputFileSequence::runPrincipal() const {
    return input_.runPrincipal();
  }
   
  ProcessConfiguration const&
  RootInputFileSequence::processConfiguration() const {
    return input_.processConfiguration();
  }
  
  int
  RootInputFileSequence::remainingEvents() const {
    return input_.remainingEvents();
  }

  ProductRegistry &
  RootInputFileSequence::productRegistryUpdate() const{
    return input_.productRegistryUpdate();
  }

  boost::shared_ptr<ProductRegistry const>
  RootInputFileSequence::productRegistry() const{
    return input_.productRegistry();
  }

  void
  RootInputFileSequence::readMany_(int number, EventPrincipalVector& result) {
    for (int i = 0; i < number; ++i) {
      std::auto_ptr<EventPrincipal> ev = readCurrentEvent();
      if (ev.get() == 0) {
	return;
      }
      VectorInputSource::EventPrincipalVectorElement e(ev.release());
      result.push_back(e);
      rootFile_->nextEventEntry();
    }
  }

  void
  RootInputFileSequence::readMany_(int number, EventPrincipalVector& result, EventID const& id, unsigned int fileSeqNumber) {
    unsigned int currentSeqNumber = fileIter_ - fileIterBegin_;
    if (currentSeqNumber != fileSeqNumber) {
      if (rootFile_) {
	rootFile_->close(primary());
	rootFile_.reset();
      }
      fileIter_ = fileIterBegin_ + fileSeqNumber;
      initFile(false);
    }
    rootFile_->setEntryAtEvent(id);
    for (int i = 0; i < number; ++i) {
      std::auto_ptr<EventPrincipal> ev = readCurrentEvent();
      if (ev.get() == 0) {
        rewindFile();
	ev = readCurrentEvent();
	assert(ev.get() != 0);
      }
      VectorInputSource::EventPrincipalVectorElement e(ev.release());
      result.push_back(e);
      rootFile_->nextEventEntry();
    }
  }

  void
  RootInputFileSequence::readManyRandom_(int number, EventPrincipalVector& result, unsigned int& fileSeqNumber) {
    skipBadFiles_ = false;
    while (eventsRemainingInFile_ < number) {
      if (rootFile_) {
	rootFile_->close(primary());
	rootFile_.reset();
      }
      fileIter_ = fileIterBegin_ + flatDistribution_->fireInt(fileCatalogItems().size());
      initFile(false);
      eventsRemainingInFile_ = rootFile_->eventTree().entries();
      rootFile_->setAtEventEntry(flatDistribution_->fireInt(eventsRemainingInFile_));
    }
    fileSeqNumber = fileIter_ - fileIterBegin_;
    for (int i = 0; i < number; ++i) {
      std::auto_ptr<EventPrincipal> ev = readCurrentEvent();
      if (ev.get() == 0) {
        rewindFile();
	ev = readCurrentEvent();
	assert(ev.get() != 0);
      }
       VectorInputSource::EventPrincipalVectorElement e(ev.release());
      result.push_back(e);
      --eventsRemainingInFile_;
      rootFile_->nextEventEntry();
    }
  }
}

