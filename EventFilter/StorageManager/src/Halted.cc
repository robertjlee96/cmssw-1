// $Id: Halted.cc,v 1.6 2009/07/10 11:41:03 dshpakov Exp $
/// @file: Halted.cc

#include "EventFilter/StorageManager/interface/Notifier.h"
#include "EventFilter/StorageManager/interface/StateMachine.h"
#include "EventFilter/StorageManager/interface/Notifier.h"

#include <iostream>

#include "xcept/tools.h"

using namespace std;
using namespace stor;

void Halted::do_entryActionWork()
{
  TransitionRecord tr( stateName(), true );
  outermost_context().updateHistory( tr );
  outermost_context().setExternallyVisibleState( "Halted" );
  outermost_context().getNotifier()->reportNewState( "Halted" );
}

Halted::Halted( my_context c ): my_base(c)
{
  safeEntryAction( outermost_context().getNotifier() );
}

void Halted::do_exitActionWork()
{
  TransitionRecord tr( stateName(), false );
  outermost_context().updateHistory( tr );
}

Halted::~Halted()
{
  safeExitAction( outermost_context().getNotifier() );
}

string Halted::do_stateName() const
{
  return string( "Halted" );
}

void Halted::do_moveToFailedState( const std::string& reason ) const
{
  outermost_context().getSharedResources()->moveToFailedState( reason );
}



/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
