#ifndef CSC_FAKEDBNOISEMATRIX_SRC_IMPL_H
#define CSC_FAKEDBNOISEMATRIX_SRC_IMPL_H

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondFormats/CSCObjects/interface/CSCobject.h"
#include "CondFormats/DataRecord/interface/CSCDBNoiseMatrixRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CondTools/CSC/interface/CSCFakeDBNoiseMatrix.h"

namespace popcon
{
  class CSCFakeDBNoiseMatrixImpl : public popcon::PopConSourceHandler<CSCDBNoiseMatrix>
    {
      
    public:
      void getNewObjects();
      std::string id() const { return m_name;}
      ~CSCFakeDBNoiseMatrixImpl(); 
      
      CSCFakeDBNoiseMatrixImpl(const edm::ParameterSet& pset);
      
    private:
      std::string m_name;
    };
}
#endif
