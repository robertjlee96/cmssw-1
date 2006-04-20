#ifndef Framework_EDProducer_h
#define Framework_EDProducer_h

/*----------------------------------------------------------------------
  
EDProducer: The base class of "modules" whose main purpose is to insert new
EDProducts into an Event.

$Id$

----------------------------------------------------------------------*/

#include "FWCore/Framework/interface/ProducerBase.h"

namespace edm {
  class Event;
  class EventSetup;
  class ParameterSet;
  class EDProducer : public ProducerBase {
  public:
    typedef EDProducer ModuleType;

    EDProducer ();
    virtual ~EDProducer();
    virtual void produce(Event& e, EventSetup const& c) = 0;
    virtual void beginJob(EventSetup const&);
    virtual void endJob();
 
  };
}

#endif
