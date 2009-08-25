
#include "IOPool/Output/src/RootOutputFile.h"

#include "FWCore/Utilities/interface/GlobalIdentifier.h"

#include "DataFormats/Provenance/interface/EventAuxiliary.h" 
#include "DataFormats/Provenance/interface/LuminosityBlockAuxiliary.h" 
#include "DataFormats/Provenance/interface/RunAuxiliary.h" 
#include "FWCore/Version/interface/GetFileFormatVersion.h"
#include "DataFormats/Provenance/interface/FileFormatVersion.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/Algorithms.h"
#include "FWCore/Utilities/interface/Digest.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "FWCore/Framework/interface/RunPrincipal.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Provenance/interface/BranchChildren.h"
#include "DataFormats/Provenance/interface/BranchID.h"
#include "DataFormats/Provenance/interface/BranchIDList.h"
#include "DataFormats/Provenance/interface/Parentage.h"
#include "DataFormats/Provenance/interface/ParentageRegistry.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/History.h"
#include "DataFormats/Provenance/interface/ParameterSetBlob.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "DataFormats/Provenance/interface/ProcessHistoryRegistry.h"
#include "DataFormats/Provenance/interface/ProcessHistoryID.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "DataFormats/Provenance/interface/ProductStatus.h"
#include "DataFormats/Common/interface/BasicHandle.h"
#include "DataFormats/Provenance/interface/BranchIDListRegistry.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TROOT.h"

#include "TTree.h"
#include "TFile.h"
#include "TClass.h"
#include "Rtypes.h"

#include <algorithm>
#include <iomanip>
#include <sstream>


namespace edm {

  namespace {
    bool
    sorterForJobReportHash(BranchDescription const* lh, BranchDescription const* rh) {
      return 
	lh->fullClassName() < rh->fullClassName() ? true :
	lh->fullClassName() > rh->fullClassName() ? false :
	lh->moduleLabel() < rh->moduleLabel() ? true :
	lh->moduleLabel() > rh->moduleLabel() ? false :
	lh->productInstanceName() < rh->productInstanceName() ? true :
	lh->productInstanceName() > rh->productInstanceName() ? false :
	lh->processName() < rh->processName() ? true :
	false;
    }
  }

  RootOutputFile::RootOutputFile(PoolOutputModule* om, std::string const& fileName, std::string const& logicalFileName) :
      file_(fileName),
      logicalFile_(logicalFileName),
      reportToken_(0),
      om_(om),
      whyNotFastClonable_(om_->whyNotFastClonable()),
      filePtr_(TFile::Open(file_.c_str(), "recreate", "", om_->compressionLevel())),
      fid_(),
      fileIndex_(),
      eventEntryNumber_(0LL),
      lumiEntryNumber_(0LL),
      runEntryNumber_(0LL),
      metaDataTree_(0),
      parentageTree_(0),
      eventHistoryTree_(0),
      pEventAux_(0),
      pLumiAux_(0),
      pRunAux_(0),
      eventEntryInfoVector_(),
      lumiEntryInfoVector_(),
      runEntryInfoVector_(),
      pEventEntryInfoVector_(&eventEntryInfoVector_),
      pLumiEntryInfoVector_(&lumiEntryInfoVector_),
      pRunEntryInfoVector_(&runEntryInfoVector_),
      pHistory_(0),
      eventTree_(filePtr_, InEvent, pEventEntryInfoVector_,
                 om_->basketSize(), om_->splitLevel(), om_->treeMaxVirtualSize()),
      lumiTree_(filePtr_, InLumi, pLumiEntryInfoVector_,
                om_->basketSize(), om_->splitLevel(), om_->treeMaxVirtualSize()),
      runTree_(filePtr_, InRun, pRunEntryInfoVector_,
               om_->basketSize(), om_->splitLevel(), om_->treeMaxVirtualSize()),
      treePointers_(),
      dataTypeReported_(false),
      parentageIDs_(),
      branchesWithStoredHistory_() {

    eventTree_.addAuxiliary<EventAuxiliary>(InEvent, pEventAux_, om_->auxItems()[InEvent].basketSize_); 
    lumiTree_.addAuxiliary<LuminosityBlockAuxiliary>(InLumi, pLumiAux_, om_->auxItems()[InLumi].basketSize_);
    runTree_.addAuxiliary<RunAuxiliary>(InRun, pRunAux_, om_->auxItems()[InRun].basketSize_);

    treePointers_[InEvent] = &eventTree_;
    treePointers_[InLumi]  = &lumiTree_;
    treePointers_[InRun]   = &runTree_;

    for(int i = InEvent; i < NumBranchTypes; ++i) {
      BranchType branchType = static_cast<BranchType>(i);
      RootOutputTree *theTree = treePointers_[branchType];
      for(OutputItemList::const_iterator it = om_->selectedOutputItemList()[branchType].begin(),
	  itEnd = om_->selectedOutputItemList()[branchType].end();
	  it != itEnd; ++it) {
	BranchDescription const& desc = *it->branchDescription_;
	desc.init();
	theTree->addBranch(desc.branchName(),
			   desc.wrappedName(),
			   it->product_,
			   it->splitLevel_,
			   it->basketSize_,
			   it->branchDescription_->produced());
	//make sure we always store product registry info for all branches we create
	branchesWithStoredHistory_.insert(it->branchID());
      }
    }
    // Don't split metadata tree or event description tree
    metaDataTree_         = RootOutputTree::makeTTree(filePtr_.get(), poolNames::metaDataTreeName(), 0);
    parentageTree_ = RootOutputTree::makeTTree(filePtr_.get(), poolNames::parentageTreeName(), 0);

    // Create the tree that will carry (event) History objects.
    eventHistoryTree_     = RootOutputTree::makeTTree(filePtr_.get(), poolNames::eventHistoryTreeName(), om_->splitLevel());
    if(!eventHistoryTree_)
      throw edm::Exception(errors::FatalRootError) 
	<< "Failed to create the tree for History objects\n";

    if(! eventHistoryTree_->Branch(poolNames::eventHistoryBranchName().c_str(), &pHistory_, om_->basketSize(), 0))
      throw edm::Exception(errors::FatalRootError) 
	<< "Failed to create a branch for Histories in the output file\n";

    fid_ = FileID(createGlobalIdentifier());

    // For the Job Report, get a vector of branch names in the "Events" tree.
    // Also create a hash of all the branch names in the "Events" tree
    // in a deterministic order, except use the full class name instead of the friendly class name.
    // To avoid extra string copies, we create a vector of pointers into the product registry,
    // and use a custom comparison operator for sorting.
    std::vector<std::string> branchNames;
    std::vector<BranchDescription const*> branches;
    branchNames.reserve(om_->selectedOutputItemList()[InEvent].size());
    branches.reserve(om->selectedOutputItemList()[InEvent].size());
    for(OutputItemList::const_iterator it = om_->selectedOutputItemList()[InEvent].begin(),
	  itEnd = om_->selectedOutputItemList()[InEvent].end();
	  it != itEnd; ++it) {
      branchNames.push_back(it->branchDescription_->branchName());
      branches.push_back(it->branchDescription_);
    }
    // Now sort the branches for the hash.
    sort_all(branches, sorterForJobReportHash);
    // Now, make a concatenated string.
    std::ostringstream oss;
    char const underscore = '_';
    for(std::vector<BranchDescription const*>::const_iterator it = branches.begin(), itEnd = branches.end(); it != itEnd; ++it) {
      BranchDescription const& bd = **it;
      oss <<  bd.fullClassName() << underscore
	  << bd.moduleLabel() << underscore
	  << bd.productInstanceName() << underscore
	  << bd.processName() << underscore;
    }
    std::string stringrep = oss.str();
    cms::Digest md5alg(stringrep);

    // Register the output file with the JobReport service
    // and get back the token for it.
    std::string moduleName = "PoolOutputModule";
    Service<JobReport> reportSvc;
    reportToken_ = reportSvc->outputFileOpened(
		      file_, logicalFile_,  // PFN and LFN
		      om_->catalog(),  // catalog
		      moduleName,   // module class name
		      om_->moduleLabel(),  // module label
		      fid_.fid(), // file id (guid)
		      std::string(), // data type (not yet known, so string is empty).
		      md5alg.digest().toString(), // branch hash
		      branchNames); // branch names being written
  }

  namespace {
    void
    maybeIssueWarning(int whyNotFastClonable, std::string const& ifileName, std::string const& ofileName) {

      // No message if fast cloning was deliberately disabled, or if there are no events to copy anyway.
      if ((whyNotFastClonable &
	(FileBlock::DisabledInConfigFile | FileBlock::NoRootInputSource | FileBlock::NotProcessingEvents | FileBlock::NoEventsInFile)) != 0) {
	return;
      }

      // There will be a message stating every reason that fast cloning was not possible.
      // If at one or more of the reasons was because of something the user explicitly specified (e.g. event selection, skipping events),
      //   or if the input file was in an old format, the message will be informational.  Otherwise, the message will be a warning.
      bool isWarning = true;
      std::ostringstream message;
      message << "Fast copying of file " << ifileName << " to file " << ofileName << " is disabled because:\n";
      if((whyNotFastClonable & FileBlock::HasSecondaryFileSequence) != 0) {
	message << "a SecondaryFileSequence was specified.\n";
        whyNotFastClonable &= ~(FileBlock::HasSecondaryFileSequence);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::FileTooOld) != 0) {
	message << "the input file is in an old format.\n";
        whyNotFastClonable &= ~(FileBlock::FileTooOld);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::EventsToBeSorted) != 0) {
	message << "events need to be sorted.\n";
        whyNotFastClonable &= ~(FileBlock::EventsToBeSorted);
      }
      if((whyNotFastClonable & FileBlock::EventsOrLumisSelectedByID) != 0) {
	message << "events or lumis were selected or skipped by ID.\n";
        whyNotFastClonable &= ~(FileBlock::EventsOrLumisSelectedByID);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::InitialEventsSkipped) != 0) {
	message << "initial events, lumis or runs were skipped.\n";
        whyNotFastClonable &= ~(FileBlock::InitialEventsSkipped);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::DuplicateEventsRemoved) != 0) {
	message << "some events were skipped because of duplicate checking.\n";
        whyNotFastClonable &= ~(FileBlock::DuplicateEventsRemoved);
      }
      if((whyNotFastClonable & FileBlock::MaxEventsTooSmall) != 0) {
	message << "some events were not copied because of maxEvents limit.\n";
        whyNotFastClonable &= ~(FileBlock::MaxEventsTooSmall);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::MaxLumisTooSmall) != 0) {
	message << "some events were not copied because of maxLumis limit.\n";
        whyNotFastClonable &= ~(FileBlock::MaxLumisTooSmall);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::RunNumberModified) != 0) {
	message << "setRunNumber was specified.\n";
        whyNotFastClonable &= ~(FileBlock::RunNumberModified);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::EventSelectionUsed) != 0) {
	message << "an EventSelector was specified.\n";
        whyNotFastClonable &= ~(FileBlock::EventSelectionUsed);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::OutputMaxEventsTooSmall) != 0) {
	message << "some events were not copied because of maxEvents output limit.\n";
        whyNotFastClonable &= ~(FileBlock::OutputMaxEventsTooSmall);
        isWarning = false;
      }
      if((whyNotFastClonable & FileBlock::SplitLevelMismatch) != 0) {
	message << "the split level or basket size of a branch or branches was modified.\n";
        whyNotFastClonable &= ~(FileBlock::SplitLevelMismatch);
      }
      if((whyNotFastClonable & FileBlock::BranchMismatch) != 0) {
	message << "The format of a data product has changed.\n";
        whyNotFastClonable &= ~(FileBlock::BranchMismatch);
      }
      assert(whyNotFastClonable == FileBlock::CanFastClone);
      if (isWarning) {
        LogWarning("FastCloningDisabled") << message.str();
      } else {
        LogInfo("FastCloningDisabled") << message.str();
      }
    }
  }

  void RootOutputFile::beginInputFile(FileBlock const& fb, int remainingEvents) {

    if(fb.tree() != 0) {

      whyNotFastClonable_ |= fb.whyNotFastClonable();

      if(remainingEvents >= 0 && remainingEvents < fb.tree()->GetEntries()) {
        whyNotFastClonable_ |= FileBlock::OutputMaxEventsTooSmall;
      }

      bool match = eventTree_.checkSplitLevelsAndBasketSizes(fb.tree());
      if(!match) {
        if(om_->overrideInputFileSplitLevels()) {
	  // We may be fast copying.  We must disable fast copying if the split levels
	  // or basket sizes do not match.
	  whyNotFastClonable_ |= FileBlock::SplitLevelMismatch;
	} else {
	  // We are using the input split levels and basket sizes from the first input file
	  // for copied output branches.  In this case, we throw an exception if any branches
	  // have different split levels or basket sizes in a subsequent input file.
	  // If the mismatch is in the first file, there is a bug somewhere, so we assert.
	  assert(om_->inputFileCount() > 1);
          throw edm::Exception(errors::MismatchedInputFiles, "RootOutputFile::beginInputFile()") <<
	    "Merge failure because input file " << file_ << " has different ROOT split levels or basket sizes\n" <<
	    "than previous files.  To allow merging in splite of this, use the configuration parameter\n" <<
	    "overrideInputFileSplitLevels=cms.untracked.bool(True)\n" <<
	    "in every PoolOutputModule.\n";
	}
      }

      // Since this check can be time consuming, we do it only if we would otherwise fast clone.
      if(whyNotFastClonable_ == FileBlock::CanFastClone) {
	if(!eventTree_.checkIfFastClonable(fb.tree())) {
	  whyNotFastClonable_ |= FileBlock::BranchMismatch;
	}
      }
    } else {
      whyNotFastClonable_ |= FileBlock::NoRootInputSource;
    }

    eventTree_.maybeFastCloneTree(whyNotFastClonable_ == FileBlock::CanFastClone, fb.tree(), om_->basketOrder());

    // Possibly issue warning or informational message if we haven't fast cloned.
    if(fb.tree() != 0 && whyNotFastClonable_ != FileBlock::CanFastClone) {
      maybeIssueWarning(whyNotFastClonable_, fb.fileName(), file_);
    }
  }

  void RootOutputFile::respondToCloseInputFile(FileBlock const&) {
    eventTree_.setEntries();
    lumiTree_.setEntries();
    runTree_.setEntries();
  }

  bool RootOutputFile::shouldWeCloseFile() const {
    unsigned int const oneK = 1024;
    Long64_t size = filePtr_->GetSize()/oneK;
    return(size >= om_->maxFileSize());
  }

  void RootOutputFile::writeOne(EventPrincipal const& e) {
    // Auxiliary branch
    pEventAux_ = &e.aux();
   
    // Store an invailid process history ID in EventAuxiliary for obsolete field.
    pEventAux_->processHistoryID_ = ProcessHistoryID(); // backward compatibility
    
    // Because getting the data may cause an exception to be thrown we want to do that
    // first before writing anything to the file about this event
    // NOTE: pEventAux_ must be set before calling fillBranches since it gets written out
    // in that routine.
    fillBranches(InEvent, e, pEventEntryInfoVector_);
     
    // History branch
    History historyForOutput(e.history());
    historyForOutput.addEventSelectionEntry(om_->selectorConfig());
    pHistory_ = &historyForOutput;
    int sz = eventHistoryTree_->Fill();
    if(sz <= 0)
      throw edm::Exception(errors::FatalRootError) 
	<< "Failed to fill the History tree for event: " << e.id()
	<< "\nTTree::Fill() returned " << sz << " bytes written." << std::endl;

    // Add the dataType to the job report if it hasn't already been done
    if(!dataTypeReported_) {
      Service<JobReport> reportSvc;
      std::string dataType("MC");
      if(pEventAux_->isRealData())  dataType = "Data";
      reportSvc->reportDataType(reportToken_, dataType);
      dataTypeReported_ = true;
    }

    pHistory_ = &e.history();

    // Add event to index
    fileIndex_.addEntry(pEventAux_->run(), pEventAux_->luminosityBlock(), pEventAux_->event(), eventEntryNumber_);
    ++eventEntryNumber_;

    // Report event written 
    Service<JobReport> reportSvc;
    reportSvc->eventWrittenToFile(reportToken_, e.id().run(), e.id().event());
  }

  void RootOutputFile::writeLuminosityBlock(LuminosityBlockPrincipal const& lb) {
    // Auxiliary branch
    // NOTE: pLumiAux_ must be set before calling fillBranches since it gets written out
    // in that routine.
    pLumiAux_ = &lb.aux();
    // Add lumi to index.
    fileIndex_.addEntry(pLumiAux_->run(), pLumiAux_->luminosityBlock(), 0U, lumiEntryNumber_);
    ++lumiEntryNumber_;
    fillBranches(InLumi, lb, pLumiEntryInfoVector_);
  }

  void RootOutputFile::writeRun(RunPrincipal const& r) {
    // Auxiliary branch
    // NOTE: pRunAux_ must be set before calling fillBranches since it gets written out
    // in that routine.
    pRunAux_ = &r.aux();
    // Add run to index.
    fileIndex_.addEntry(pRunAux_->run(), 0U, 0U, runEntryNumber_);
    ++runEntryNumber_;
    fillBranches(InRun, r, pRunEntryInfoVector_);
  }

  void RootOutputFile::writeParentageRegistry() {
    Parentage const*   desc(0);
    
    if(!parentageTree_->Branch(poolNames::parentageBranchName().c_str(), 
					&desc, om_->basketSize(), 0))
      throw edm::Exception(errors::FatalRootError) 
	<< "Failed to create a branch for Parentages in the output file";

    ParentageRegistry& ptReg = *ParentageRegistry::instance();
    std::set<ParentageID>::const_iterator pidend = parentageIDs_.end();
    for(ParentageRegistry::const_iterator i = ptReg.begin(), e = ptReg.end(); i != e; ++i) {
      if(parentageIDs_.find(i->first) != pidend) {
        desc = &(i->second);
        parentageTree_->Fill();
      }
    }
  }

  void RootOutputFile::writeFileFormatVersion() {
    FileFormatVersion fileFormatVersion(getFileFormatVersion());
    FileFormatVersion* pFileFmtVsn = &fileFormatVersion;
    TBranch* b = metaDataTree_->Branch(poolNames::fileFormatVersionBranchName().c_str(), &pFileFmtVsn, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeFileIdentifier() {
    FileID* fidPtr = &fid_;
    TBranch* b = metaDataTree_->Branch(poolNames::fileIdentifierBranchName().c_str(), &fidPtr, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeFileIndex() {
    fileIndex_.sortBy_Run_Lumi_Event();
    FileIndex* findexPtr = &fileIndex_;
    TBranch* b = metaDataTree_->Branch(poolNames::fileIndexBranchName().c_str(), &findexPtr, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeEventHistory() {
    RootOutputTree::writeTTree(eventHistoryTree_);
  }

  void RootOutputFile::writeProcessConfigurationRegistry() {
    typedef ProcessConfigurationRegistry::collection_type Map;
    Map const& procConfigMap = ProcessConfigurationRegistry::instance()->data();
    ProcessConfigurationVector procConfigVector;
    for(Map::const_iterator i = procConfigMap.begin(), e = procConfigMap.end(); i != e; ++i) {
      procConfigVector.push_back(i->second);
    }
    sort_all(procConfigVector);
    ProcessConfigurationVector* p = &procConfigVector;
    TBranch* b = metaDataTree_->Branch(poolNames::processConfigurationBranchName().c_str(), &p, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeProcessHistoryRegistry() { 
    typedef ProcessHistoryRegistry::collection_type Map;
    Map const& procHistoryMap = ProcessHistoryRegistry::instance()->data();
    ProcessHistoryVector procHistoryVector;
    for(Map::const_iterator i = procHistoryMap.begin(), e = procHistoryMap.end(); i != e; ++i) {
      procHistoryVector.push_back(i->second);
    }
    ProcessHistoryVector* p = &procHistoryVector;
    TBranch* b = metaDataTree_->Branch(poolNames::processHistoryBranchName().c_str(), &p, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeBranchIDListRegistry() { 
    BranchIDListRegistry::collection_type* p = &BranchIDListRegistry::instance()->data();
    TBranch* b = metaDataTree_->Branch(poolNames::branchIDListBranchName().c_str(), &p, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeParameterSetRegistry() { 
    typedef std::map<ParameterSetID, ParameterSetBlob> ParameterSetMap;
    ParameterSetMap psetMap;
    pset::fillMap(pset::Registry::instance(), psetMap);
    ParameterSetMap* pPsetMap = &psetMap;
    TBranch* b = metaDataTree_->Branch(poolNames::parameterSetMapBranchName().c_str(), &pPsetMap, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::writeProductDescriptionRegistry() { 
    // Make a local copy of the ProductRegistry, removing any transient or pruned products.
    typedef ProductRegistry::ProductList ProductList;
    Service<ConstProductRegistry> reg;
    ProductRegistry pReg(reg->productList());
    ProductList& pList  = const_cast<ProductList &>(pReg.productList());
    std::set<BranchID>::iterator end = branchesWithStoredHistory_.end();
    for(ProductList::iterator it = pList.begin(); it != pList.end();) {
      if(branchesWithStoredHistory_.find(it->second.branchID()) == end) {
	// avoid invalidating iterator on deletion
	ProductList::iterator itCopy = it;
	++it;
	pList.erase(itCopy);
	
      } else {
	++it;
      }
    }

    ProductRegistry* ppReg = &pReg;
    TBranch* b = metaDataTree_->Branch(poolNames::productDescriptionBranchName().c_str(), &ppReg, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  } 
  void RootOutputFile::writeProductDependencies() { 
    BranchChildren& pDeps = const_cast<BranchChildren&>(om_->branchChildren());
    BranchChildren* ppDeps = &pDeps;
    TBranch* b = metaDataTree_->Branch(poolNames::productDependenciesBranchName().c_str(), &ppDeps, om_->basketSize(), 0);
    assert(b);
    b->Fill();
  }

  void RootOutputFile::finishEndFile() { 
    metaDataTree_->SetEntries(-1);
    RootOutputTree::writeTTree(metaDataTree_);

    RootOutputTree::writeTTree(parentageTree_);

    // Create branch aliases for all the branches in the
    // events/lumis/runs trees. The loop is over all types of data
    // products.
    for(int i = InEvent; i < NumBranchTypes; ++i) {
      BranchType branchType = static_cast<BranchType>(i);
      setBranchAliases(treePointers_[branchType]->tree(), om_->keptProducts()[branchType]);
      treePointers_[branchType]->writeTree();
    }

    // close the file -- mfp
    // Just to play it safe, zero all pointers to objects in the TFile to be closed.
    metaDataTree_ = parentageTree_ = eventHistoryTree_ = 0;
    for(RootOutputTreePtrArray::iterator it = treePointers_.begin(), itEnd = treePointers_.end(); it != itEnd; ++it) {
      (*it)->close();
      (*it) = 0;
    }
    filePtr_->Close();
    filePtr_.reset();

    // report that file has been closed
    Service<JobReport> reportSvc;
    reportSvc->outputFileClosed(reportToken_);

  }

  void
  RootOutputFile::setBranchAliases(TTree* tree, Selections const& branches) const {
    if(tree && tree->GetNbranches() != 0) {
      for(Selections::const_iterator i = branches.begin(), iEnd = branches.end();
	  i != iEnd; ++i) {
	BranchDescription const& pd = **i;
	std::string const& full = pd.branchName() + "obj";
	if(pd.branchAliases().empty()) {
	  std::string const& alias =
	      (pd.productInstanceName().empty() ? pd.moduleLabel() : pd.productInstanceName());
	  tree->SetAlias(alias.c_str(), full.c_str());
	} else {
	  std::set<std::string>::const_iterator it = pd.branchAliases().begin(), itEnd = pd.branchAliases().end();
	  for(; it != itEnd; ++it) {
	    tree->SetAlias((*it).c_str(), full.c_str());
	  }
	}
      }
    }
  }
   
  void
  RootOutputFile::insertAncestors(ProductProvenance const& iGetParents,
                                  Principal const& principal,
                                  bool produced,
                                  std::set<ProductProvenance>& oToFill) {
    assert(om_->dropMetaData() != PoolOutputModule::DropAll);
    assert(produced || om_->dropMetaData() != PoolOutputModule::DropPrior);
    if(om_->dropMetaData() == PoolOutputModule::DropDroppedPrior && !produced) return;
    BranchMapper const& iMapper = *principal.branchMapperPtr();
    std::vector<BranchID> const& parentIDs = iGetParents.parentage().parents();
    for(std::vector<BranchID>::const_iterator it = parentIDs.begin(), itEnd = parentIDs.end();
          it != itEnd; ++it) {
      branchesWithStoredHistory_.insert(*it);
      boost::shared_ptr<ProductProvenance> info = iMapper.branchIDToProvenance(*it);
      if(info) {
        if(om_->dropMetaData() == PoolOutputModule::DropNone ||
		 principal.getProvenance(info->branchID()).product().produced()) {
	  if(oToFill.insert(*info).second) {
            //haven't seen this one yet
            insertAncestors(*info, principal, produced, oToFill);
	  }
	}
      }
    }
  }
   
  void RootOutputFile::fillBranches(
		BranchType const& branchType,
		Principal const& principal,
		std::vector<ProductProvenance>* productProvenanceVecPtr) {

    std::vector<boost::shared_ptr<EDProduct> > dummies;

    bool const fastCloning = (branchType == InEvent) && (whyNotFastClonable_ == FileBlock::CanFastClone);
    
    OutputItemList const& items = om_->selectedOutputItemList()[branchType];

    std::set<ProductProvenance> provenanceToKeep;

    // Loop over EDProduct branches, fill the provenance, and write the branch.
    for(OutputItemList::const_iterator i = items.begin(), iEnd = items.end(); i != iEnd; ++i) {

      BranchID const& id = i->branchDescription_->branchID();
      branchesWithStoredHistory_.insert(id);
       
      bool produced = i->branchDescription_->produced();
      bool keepProvenance = om_->dropMetaData() == PoolOutputModule::DropNone ||
			    om_->dropMetaData() == PoolOutputModule::DropDroppedPrior ||
			   (om_->dropMetaData() == PoolOutputModule::DropPrior && produced);
      bool getProd = (produced || !fastCloning ||
	 treePointers_[branchType]->uncloned(i->branchDescription_->branchName()));

      EDProduct const* product = 0;
      OutputHandle const oh = principal.getForOutput(id, getProd);
      if(keepProvenance && oh.productProvenance()) {
	provenanceToKeep.insert(*oh.productProvenance());
	assert(principal.branchMapperPtr());
	insertAncestors(*oh.productProvenance(), principal, produced, provenanceToKeep);
	parentageIDs_.insert(oh.productProvenance()->parentageID());
      }
      product = oh.wrapper();
      if(getProd) {
	if(product == 0) {
	  // No product with this ID is in the event.
	  // Add a null product.
	  TClass* cp = gROOT->GetClass(i->branchDescription_->wrappedName().c_str());
	  boost::shared_ptr<EDProduct> dummy(static_cast<EDProduct*>(cp->New()));
	  dummies.push_back(dummy);
	  product = dummy.get();
	}
	i->product_ = product;
      }
    }
     
    productProvenanceVecPtr->assign(provenanceToKeep.begin(), provenanceToKeep.end());
    treePointers_[branchType]->fillTree();
    productProvenanceVecPtr->clear();
  }

}
