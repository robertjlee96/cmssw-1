#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/CopyPolicy.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/HLTGlobalStatus.h"
#include "DataFormats/Common/interface/HLTPathStatus.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Provenance/interface/ProductID.h"
#include "DataFormats/Common/interface/RangeMap.h"
#include "DataFormats/Common/interface/RefBase.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "DataFormats/Common/interface/VectorHolder.h"
#include "DataFormats/Common/interface/RefItem.h"
#include "DataFormats/Common/interface/RefVectorBase.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/FillView.h"
#include "DataFormats/Common/interface/DataFrame.h"
#include "DataFormats/Common/interface/DataFrameContainer.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Common/interface/ConstPtrCache.h"
#include "DataFormats/Common/interface/BoolCache.h"
#include "DataFormats/Common/interface/PtrVectorBase.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "FWCore/MessageLogger/interface/ErrorSummaryEntry.h"

#include <vector>

namespace {
  struct dictionary {
    edm::Wrapper<edm::DataFrameContainer> dummywdfc;
    edm::Wrapper<edm::HLTPathStatus> dummyx16;
    edm::Wrapper<std::vector<edm::HLTPathStatus> > dummyx17;
    edm::Wrapper<edm::HLTGlobalStatus> dummyx18;
    edm::Wrapper<edm::TriggerResults> dummyx19;
    
    edm::Wrapper<edm::RefVector<std::vector<int> > > dummyx20;
    edm::Wrapper<edm::RefToBaseVector<int> > dummyx21;
    edm::Wrapper<edm::PtrVector<int> > dummyx21_3;
    edm::Ptr<int> dummyx21_4;
    edm::reftobase::RefVectorHolderBase * dummyx21_0;
    edm::reftobase::IndirectVectorHolder<int> dummyx21_1;
    edm::reftobase::VectorHolder<int, edm::RefVector<std::vector<int> > > dummyx21_2;
    
    edm::RefItem<unsigned int> dummyRefItem1;
    edm::RefItem<unsigned long> dummyRefItem1_1;
    edm::RefItem<int> dummyRefItem3;
    edm::RefItem<std::pair<unsigned int, unsigned int> > dummyRefItem2;
    edm::RefItem<std::pair<unsigned int, unsigned long> > dummyRefItem2a;
    edm::RefBase<std::vector<unsigned int>::size_type> dummRefBase1;
    edm::RefBase<std::pair<unsigned int, unsigned int> > dummRefBase2;
    edm::RefBase<std::pair<unsigned int, unsigned long> > dummRefBase2a;
    edm::RefBase<int> dummyRefBase3;
    edm::RefBase<unsigned int> dummyRefBase3_1;
    edm::RefBase<unsigned long> dummyRefBase3_2;
    edm::RefVectorBase<std::vector<unsigned int>::size_type> dummyRefVectorBase;
    edm::RefVectorBase<int> dummyRefVectorBase2;
    edm::RefVectorBase<unsigned int> dummyRefVectorBase2_1;
    edm::RefVectorBase<unsigned long> dummyRefVectorBase2_2;
    edm::RefVectorBase<std::pair<unsigned int, unsigned int> > dummyRefVectorBase3;
    
    edm::RangeMap<int, std::vector<float>, edm::CopyPolicy<float> > dummyRangeMap1;
    
    std::vector<edmNew::dstvdetails::DetSetVectorTrans::Item>  dummyDSTVItemVector;

    std::pair<edm::ProductID, unsigned int> ppui1;
    edm::Wrapper<edm::ValueMap<int> > wvm1;
    edm::Wrapper<edm::ValueMap<unsigned int> > wvm2;
    edm::Wrapper<edm::ValueMap<bool> > wvm3;
    edm::Wrapper<edm::ValueMap<float> > wvm4;
    edm::Wrapper<edm::ValueMap<double> > wvm5;
    std::vector<edm::EventAuxiliary> dummyVectorEventAuxiliary;
    edm::Wrapper<std::vector<edm::EventAuxiliary> > wvea;
    edm::Wrapper<std::vector<edm::ErrorSummaryEntry> > wves;
  };
}
