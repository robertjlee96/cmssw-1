#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <cstdlib>

#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/mem_fn.hpp"

#include "PluginManager/PluginManager.h"

#include "FWCore/Utilities/interface/DebugMacros.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "FWCore/Framework/interface/Actions.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventProcessor.h"
#include "FWCore/Framework/interface/ScheduleBuilder.h"
#include "FWCore/Framework/interface/ScheduleExecutor.h"
#include "FWCore/Framework/interface/IOVSyncValue.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EventSetupProvider.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"

#include "FWCore/Framework/src/Worker.h"
#include "FWCore/Framework/src/WorkerRegistry.h"
#include "FWCore/Framework/src/InputSourceFactory.h"
#include "FWCore/Framework/src/SignallingProductRegistry.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ProcessPSetBuilder.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"

#include "FWCore/EDProduct/interface/EDProductGetter.h"

#include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"


using namespace std;
using boost::shared_ptr;
using edm::serviceregistry::ServiceLegacy; 
using edm::serviceregistry::kOverlapIsError;

namespace edm {

  typedef vector<string>   StrVec;
  typedef list<string>     StrList;
  typedef Worker*          WorkerPtr;
  typedef list<WorkerPtr>  WorkerList;
  typedef list<WorkerList> PathList;

  struct CommonParams
  {
    CommonParams():version_(),pass_() { }
    CommonParams(const string& name,unsigned long ver,unsigned long pass):
      processName_(name),version_(ver),pass_(pass) { }

    string                  processName_;
    unsigned long           version_;
    unsigned long           pass_;
  }; // struct CommonParams


  // temporary function because we do not know how to do this
  unsigned long getVersion() { return 0; }


  shared_ptr<InputSource> makeInput(ParameterSet const& params_,
				    const CommonParams& common,
				    ProductRegistry& preg)
  {
    // find single source
    bool sourceSpecified = false;
    try {
      ParameterSet main_input = params_.getParameter<ParameterSet>("@main_input");
      sourceSpecified = true;
      InputSourceDescription isdesc(common.processName_,common.pass_,preg);

      shared_ptr<InputSource> input_
	(InputSourceFactory::get()->makeInputSource(main_input, isdesc).release());
    
      return input_;
    } catch(const edm::Exception& iException) {
      if(sourceSpecified == false && errors::Configuration == iException.categoryCode()) {
        throw edm::Exception(errors::Configuration, "NoInputSource")
	  <<"No main input source found in configuration.  Please add an input source via 'source = ...' in the configuration file.\n";
      } else {
        throw;
      }
    }
    return shared_ptr<InputSource>();
  }
  
  void fillEventSetupProvider(eventsetup::EventSetupProvider& cp,
			      ParameterSet const& params_,
			      const CommonParams& common)
  {
    using namespace std;
    using namespace edm::eventsetup;
    vector<string> providers = params_.getParameter<vector<string> >("@all_esmodules");
    for(vector<string>::iterator itName = providers.begin();
	itName != providers.end();
	++itName) {
      ParameterSet providerPSet = params_.getParameter<ParameterSet>(*itName);
      ModuleFactory::get()->addTo(cp, 
				  providerPSet, 
				  common.processName_, 
				  common.version_, 
				  common.pass_);
    }

    vector<string> sources = params_.getParameter<vector<string> >("@all_essources");
    for(vector<string>::iterator itName = sources.begin();
	itName != sources.end();
	++itName) {
      ParameterSet providerPSet = params_.getParameter<ParameterSet>(*itName);
      SourceFactory::get()->addTo(cp, 
				  providerPSet, 
				  common.processName_, 
				  common.version_, 
				  common.pass_);
    }
  }


  //need a wrapper to let me 'copy' references to EventSetup
  namespace eventprocessor 
  {
    struct ESRefWrapper 
    {
      EventSetup const & es_;
      ESRefWrapper(EventSetup const &iES) : es_(iES) {}
      operator const EventSetup&() { return es_; }
    };
  }

  using eventprocessor::ESRefWrapper;

  //----------------------------------------------------------------------
  // Implementation of FwkImpl, the 'pimpl' for EventProcessor
  //----------------------------------------------------------------------
  //
  // right now we only support a pset string from constructor or
  // pset read from file

  class FwkImpl
  {
  public:
    explicit FwkImpl(const string& config,
                     const ServiceToken& = ServiceToken(),
                     ServiceLegacy=kOverlapIsError);

    EventProcessor::StatusCode run(unsigned long numberToProcess);
    void                       beginJob();
    bool                       endJob();


    ServiceToken   getToken();
    void           connectSigs(EventProcessor* ep);
    InputSource&   getInputSource();

  private:
    
    shared_ptr<ParameterSet>        params_;
    CommonParams                    common_;
    WorkerRegistry                  wreg_;
    SignallingProductRegistry       preg_;
    PathList                        workers_;

    ActivityRegistry                actReg_;
    ServiceToken                    serviceToken_;
    shared_ptr<InputSource>         input_;
    std::auto_ptr<ScheduleExecutor> runner_;
    eventsetup::EventSetupProvider  esp_;    

    bool                            emittedBeginJob_;
    ActionTable                     act_table_;
  }; // class FwkImpl

  // ---------------------------------------------------------------

  FwkImpl::FwkImpl(const string& config,
                   const ServiceToken& iToken, 
		   ServiceLegacy iLegacy):
    //configstring_(config),
    emittedBeginJob_(false) 
  {
    // TODO: Fix const-correctness. The ParameterSets that are
    // returned here should be const, so that we can be sure they are
    // not modified.

    shared_ptr<vector<ParameterSet> > pServiceSets;
    makeParameterSets(config, params_, pServiceSets);

    //create the services
    serviceToken_ = ServiceRegistry::createSet(*pServiceSets,
					       iToken,iLegacy);
    serviceToken_.connectTo(actReg_);
     
    //add the ProductRegistry as a service ONLY for the construction phase
    typedef serviceregistry::ServiceWrapper<ConstProductRegistry> w_CPR;
    shared_ptr<w_CPR>
      reg(new w_CPR( std::auto_ptr<ConstProductRegistry>(new ConstProductRegistry(preg_))));
    ServiceToken tempToken( ServiceRegistry::createContaining(reg, 
							      serviceToken_, 
							      kOverlapIsError));
    //make the services available
    ServiceRegistry::Operate operate(tempToken);
     
    //params_ = builder.getProcessPSet();
    act_table_ = ActionTable(*params_);
    common_ = 
      CommonParams((*params_).getParameter<string>("@process_name"),
		   getVersion(), // this is not written for real yet
		   0); // how is this specifified? Where does it come from?
     
    input_= makeInput(*params_, common_, preg_);
    ScheduleBuilder sbuilder(*params_, wreg_, preg_, act_table_);
     
    workers_= (sbuilder.getPathList());
    runner_ = std::auto_ptr<ScheduleExecutor>(new ScheduleExecutor(workers_,act_table_));
    runner_->preModuleSignal.connect(actReg_.preModuleSignal_);
    runner_->postModuleSignal.connect(actReg_.postModuleSignal_);
     
     
    fillEventSetupProvider(esp_, *params_, common_);
    //   initialize(iToken,iLegacy);
    FDEBUG(2) << params_->toString() << std::endl;
  }

  EventProcessor::StatusCode
  FwkImpl::run(unsigned long numberToProcess)
  {
    //make the services available
    ServiceRegistry::Operate operate(serviceToken_);

    bool runforever = numberToProcess==0;
    unsigned int eventcount=0;

    //make sure this was called
    beginJob();

    while(runforever || eventcount<numberToProcess)
      {
	++eventcount;
	FDEBUG(1) << eventcount << std::endl;
	auto_ptr<EventPrincipal> pep = input_->readEvent();
        
	if(pep.get()==0) break;
	IOVSyncValue ts(pep->id(), pep->time());
	EventSetup const& es = esp_.eventSetupForInstance(ts);

	try
	  {
            ModuleDescription dummy;
            {
              actReg_.preProcessEventSignal_(pep->id(),pep->time());
            }
	    runner_->runOneEvent(*pep.get(),es);
            {
              actReg_.postProcessEventSignal_(Event(*pep.get(),dummy) , es);
            }
	  }
	catch(cms::Exception& e)
	  {
	    actions::ActionCodes code = act_table_.find(e.rootCause());
	    if(code==actions::IgnoreCompletely)
	      {
		// change to error logger!
		cerr << "Ignoring exception from Event ID=" << pep->id()
		     << ", message:\n" << e.what()
		     << endl;
		continue;
	      }
	    else if(code==actions::SkipEvent)
	      {
		cerr << "Skipping Event ID=" << pep->id()
		     << ", message:\n" << e.what()
		     << endl;
		continue;
	      }
	    else
	      throw edm::Exception(errors::EventProcessorFailure,
				   "EventProcessingStopped",e);
	  }
      }

    return 0;
  }

  void
  FwkImpl::beginJob() 
  {
    //make the services available
    ServiceRegistry::Operate operate(serviceToken_);

    if(! emittedBeginJob_) {
      //NOTE:  This implementation assumes 'Job' means one call the EventProcessor::run
      // If it really means once per 'application' then this code will have to be changed.
      // Also have to deal with case where have 'run' then new Module added and do 'run'
      // again.  In that case the newly added Module needs its 'beginJob' to be called.
      EventSetup const& es = esp_.eventSetupForInstance(IOVSyncValue::beginOfTime());
      PathList::iterator itWorkerList = workers_.begin();
      PathList::iterator itEnd = workers_.end();
      ESRefWrapper wrapper(es);
        
      for(; itWorkerList != itEnd; ++itEnd) {
	std::for_each(itWorkerList->begin(), itWorkerList->end(), 
		      boost::bind(boost::mem_fn(&Worker::beginJob), _1, wrapper));
      }
      emittedBeginJob_ = true;
      actReg_.postBeginJobSignal_();
    }
  }

  bool
  FwkImpl::endJob() 
  {
    //make the services available
    ServiceRegistry::Operate operate(serviceToken_);
     
    bool returnValue = true;
    PathList::const_iterator itWorkerList = workers_.begin();
    PathList::const_iterator itEnd = workers_.end();
    for(; itWorkerList != itEnd; ++itEnd) {
      for(WorkerList::const_iterator itWorker = itWorkerList->begin();
	  itWorker != itWorkerList->end();
	  ++itWorker) {
	try {
	  (*itWorker)->endJob();
	} catch(cms::Exception& iException) {
	  cerr<<"Caught cms::Exception in endJob: "<< iException.what()<<endl;
	  returnValue = false;
	} catch(std::exception& iException) {
	  cerr<<"Caught std::exception in endJob: "<< iException.what()<<endl;
	  cerr<<endl;
	  returnValue = false;
	} catch(...) {
	  cerr<<"Caught unknown exception in endJob."<<endl;
	  returnValue = false;
	}
      }
    }     
     
    actReg_.postEndJobSignal_();
    return returnValue;
  }

  ServiceToken
  FwkImpl::getToken()
  {
    return serviceToken_;
  }
  

  void
  FwkImpl::connectSigs(EventProcessor* ep)
  {
    // When the FwkImpl signals are given, pass them to the
    // appropriate EventProcessor signals so that the outside world
    // can see the signal.
    actReg_.preProcessEventSignal_.connect(ep->preProcessEventSignal);
    actReg_.postProcessEventSignal_.connect(ep->postProcessEventSignal);
  }

  InputSource&
  FwkImpl::getInputSource()
  {
    return *input_;
  }

  //----------------------------------------------------------------------
  // Implementation of EventProcessor
  //----------------------------------------------------------------------
  EventProcessor::EventProcessor(const string& config) :
    impl_(new FwkImpl(config, 
		      ServiceToken(), //  no pre-made services
		      kOverlapIsError))
  {
    impl_->connectSigs(this);
    //connectSigs(this, impl_);
  } 
  
  EventProcessor::EventProcessor(const string& config,
				 const ServiceToken& iToken,
				 ServiceLegacy iLegacy):
    impl_(new FwkImpl(config,iToken,iLegacy))
  {
    impl_->connectSigs(this);
    //connectSigs(this, impl_);
  } 
  
  EventProcessor::~EventProcessor()
  {
    //make the service's available while everything is being deleted
    ServiceToken token = impl_->getToken();
    ServiceRegistry::Operate op(token); 
    delete impl_;
  }

  EventProcessor::StatusCode
  EventProcessor::run(unsigned long numberToProcess)
  {
    return impl_->run(numberToProcess);
  }
  
  void
  EventProcessor::beginJob() 
  {
    impl_->beginJob();
  }

  bool
  EventProcessor::endJob() 
  {
    return impl_->endJob();
  }

  InputSource&
  EventProcessor::getInputSource()
  {
    //return *impl_->input_;
    return impl_->getInputSource();
  }



}
