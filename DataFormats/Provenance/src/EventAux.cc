#include "DataFormats/Provenance/interface/EventAux.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"

/*----------------------------------------------------------------------

$Id: EventAux.cc,v 1.1 2007/03/15 21:45:37 wmtan Exp $

----------------------------------------------------------------------*/

namespace edm {
  void conversion(EventAux const& from, EventAuxiliary & to) {
    to.processHistoryID_ = from.processHistoryID_;
    to.id_ = from.id_;
    to.time_ = from.time_;
    to.luminosityBlock_ = from.luminosityBlockID_;
  }
}
