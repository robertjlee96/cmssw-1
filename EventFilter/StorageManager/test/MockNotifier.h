// -*- c++ -*-
// $Id: MockNotifier.h,v 1.3 2009/07/01 13:08:18 dshpakov Exp $

#ifndef MOCKNOTIFIER_H
#define MOCKNOTIFIER_H

// Notifier implementation to be used by the state machine unit test

#include "EventFilter/StorageManager/interface/Notifier.h"

#include "xdaq/Application.h"


namespace stor
{

  class MockNotifier: public Notifier
  {

  public:

    MockNotifier( xdaq::Application* app ):
      _app( app )
    {}
    
    ~MockNotifier() {}

    void reportNewState( const std::string& stateName ) {}
    Logger& getLogger() { return _app->getApplicationLogger(); }
    void tellSentinel( const std::string& level, xcept::Exception& e ) {}

  private:

    xdaq::Application* _app;

    unsigned long instanceNumber() const { return 0; }

  };

}

#endif // MOCKNOTIFIER_H
