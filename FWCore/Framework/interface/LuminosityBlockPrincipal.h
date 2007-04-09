#ifndef Framework_LuminosityBlockPrincipal_h
#define Framework_LuminosityBlockPrincipal_h

/*----------------------------------------------------------------------
  
LuminosityBlockPrincipal: This is the class responsible for management of
per luminosity block EDProducts. It is not seen by reconstruction code;
such code sees the LuminosityBlock class, which is a proxy for LuminosityBlockPrincipal.

The major internal component of the LuminosityBlockPrincipal
is the DataBlock.

$Id: LuminosityBlockPrincipal.h,v 1.11 2007/03/27 23:06:57 wmtan Exp $

----------------------------------------------------------------------*/

#include "DataFormats/Provenance/interface/LuminosityBlockAuxiliary.h"
#include "DataFormats/Provenance/interface/RunID.h"
#include "FWCore/Framework/interface/Principal.h"

#include "boost/shared_ptr.hpp"

namespace edm {
  class RunPrincipal;
  class LuminosityBlockPrincipal : private Principal {
  typedef Principal Base;
  public:
    LuminosityBlockPrincipal(LuminosityBlockNumber_t const& id,
	ProductRegistry const& reg,
        boost::shared_ptr<RunPrincipal> rp,
        ProcessConfiguration const& pc,
	ProcessHistoryID const& hist = ProcessHistoryID(),
	boost::shared_ptr<DelayedReader> rtrv = boost::shared_ptr<DelayedReader>(new NoDelayedReader));

    LuminosityBlockPrincipal(LuminosityBlockNumber_t const& id,
	ProductRegistry const& reg,
        RunNumber_t run,
        ProcessConfiguration const& pc,
	ProcessHistoryID const& hist = ProcessHistoryID(),
	boost::shared_ptr<DelayedReader> rtrv = boost::shared_ptr<DelayedReader>(new NoDelayedReader));

    ~LuminosityBlockPrincipal() {}

    RunPrincipal const& runPrincipal() const {
      return *runPrincipal_;
    }

    RunPrincipal & runPrincipal() {
      return *runPrincipal_;
    }

    boost::shared_ptr<RunPrincipal>
    runPrincipalSharedPtr() {
      return runPrincipal_;
    }

    LuminosityBlockID id() const {
      return aux().id();
    }

    LuminosityBlockNumber_t luminosityBlock() const {
      return aux().luminosityBlock();
    }

    LuminosityBlockAuxiliary const& aux() const {
      return aux_;
    }

    RunNumber_t runNumber() const {
      return aux().run();
    }
    using Base::addGroup;
    using Base::addToProcessHistory;
    using Base::begin;
    using Base::end;
    using Base::getAllProvenance;
    using Base::getByLabel;
    using Base::get;
    using Base::getBySelector;
    using Base::getByType;
    using Base::getGroup;
    using Base::getIt;
    using Base::getMany;
    using Base::getManyByType;
    using Base::getProvenance;
    using Base::groupGetter;
    using Base::numEDProducts;
    using Base::processHistory;
    using Base::processHistoryID;
    using Base::prodGetter;
    using Base::productRegistry;
    using Base::put;
    using Base::size;
    using Base::store;

  private:
    virtual bool unscheduledFill(Group const&) const {return false;}
    virtual bool fillAndMatchSelector(Provenance &, SelectorBase const&) const {return false;}

    boost::shared_ptr<RunPrincipal> runPrincipal_;
    LuminosityBlockAuxiliary aux_;
  };
}
#endif

