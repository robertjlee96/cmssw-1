#include "DataFormats/Provenance/interface/Provenance.h"
#include <ostream>

/*----------------------------------------------------------------------

$Id: Provenance.cc,v 1.3 2007/05/10 12:27:02 wmtan Exp $

----------------------------------------------------------------------*/

namespace edm {
  Provenance::Provenance(BranchDescription const& p) :
    product_(ConstBranchDescription(p)),
    event_()
  { }

  Provenance::Provenance(ConstBranchDescription const& p) :
    product_(p),
    event_()
  { }

  Provenance::Provenance(BranchDescription const& p, BranchEntryDescription::CreatorStatus const& status) :
    product_(ConstBranchDescription(p)),
    event_(new BranchEntryDescription(p.productID(), status))
  { }

  Provenance::Provenance(ConstBranchDescription const& p, BranchEntryDescription::CreatorStatus const& status) :
    product_(p),
    event_(new BranchEntryDescription(p.productID(), status))
  { }

  Provenance::Provenance(BranchDescription const& p, boost::shared_ptr<BranchEntryDescription> e) :
    product_(ConstBranchDescription(p)),
    event_(e)
  { }

  Provenance::Provenance(ConstBranchDescription const& p, boost::shared_ptr<BranchEntryDescription> e) :
    product_(p),
    event_(e)
  { }

 Provenance::Provenance(BranchDescription const& p, BranchEntryDescription const& e) :
    product_(ConstBranchDescription(p)),
    event_(new BranchEntryDescription(e))
  { }

 Provenance::Provenance(ConstBranchDescription const& p, BranchEntryDescription const& e) :
    product_(p),
    event_(new BranchEntryDescription(e))
  { }

  void
  Provenance::setEvent(boost::shared_ptr<BranchEntryDescription> e) {
    assert(event_.get() == 0);
    event_ = e;
  }

  void
  Provenance::write(std::ostream& os) const {
    // This is grossly inadequate, but it is not critical for the
    // first pass.
    product().write(os);
    event().write(os);
  }

    
  bool operator==(Provenance const& a, Provenance const& b) {
    return
      a.product() == b.product()
      && a.event() == b.event();
  }

}

