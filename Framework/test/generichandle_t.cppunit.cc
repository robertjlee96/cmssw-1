/*----------------------------------------------------------------------

Test of the EventPrincipal class.

$Id$

----------------------------------------------------------------------*/  
#include <string>
#include <iostream>

#include "FWCore/Utilities/interface/GetPassID.h"
#include "FWCore/Utilities/interface/GetReleaseVersion.h"
#include "FWCore/Utilities/interface/GlobalIdentifier.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/Provenance/interface/LuminosityBlockAuxiliary.h"
#include "DataFormats/Provenance/interface/RunAuxiliary.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/TestObjects/interface/ToyProducts.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "FWCore/Framework/interface/RunPrincipal.h"

#include "FWCore/Framework/interface/GenericHandle.h"
#include <cppunit/extensions/HelperMacros.h>

class testGenericHandle: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(testGenericHandle);
CPPUNIT_TEST(failgetbyLabelTest);
CPPUNIT_TEST(getbyLabelTest);
CPPUNIT_TEST(failWrongType);
CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown(){}
  void failgetbyLabelTest();
  void failWrongType();
  void getbyLabelTest();
};

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testGenericHandle);

void testGenericHandle::failWrongType() {
   try {
      //intentionally misspelled type
      edm::GenericHandle h("edmtest::DmmyProduct");
      CPPUNIT_ASSERT("Failed to thow"==0);
   }
   catch (cms::Exception& x) {
      // nothing to do
   }
   catch (...) {
      CPPUNIT_ASSERT("Threw wrong kind of exception" == 0);
   }
}
void testGenericHandle::failgetbyLabelTest() {

  edm::EventID id;
  edm::Timestamp time;
  std::string uuid = edm::createGlobalIdentifier();
  edm::ProcessConfiguration pc("PROD", edm::ParameterSetID(), edm::getReleaseVersion(), edm::getPassID());
  boost::shared_ptr<edm::ProductRegistry const> preg(new edm::ProductRegistry);
  edm::RunAuxiliary runAux(id.run(), time, time);
  boost::shared_ptr<edm::RunPrincipal> rp(new edm::RunPrincipal(runAux, preg, pc));
  edm::LuminosityBlockAuxiliary lumiAux(rp->run(), 1, time, time);
  boost::shared_ptr<edm::LuminosityBlockPrincipal>lbp(new edm::LuminosityBlockPrincipal(lumiAux, preg, rp, pc));
  edm::EventAuxiliary eventAux(id, uuid, time, lbp->luminosityBlock(), true);
  edm::EventPrincipal ep(eventAux, preg, lbp, pc);
  edm::GenericHandle h("edmtest::DummyProduct");
  bool didThrow=true;
  try {
     edm::ModuleDescription modDesc;
     modDesc.moduleName_="Blah";
     modDesc.moduleLabel_="blahs"; 
     edm::Event event(ep, modDesc);
     
     std::string label("this does not exist");
     event.getByLabel(label,h);
     *h;
     didThrow=false;
  }
  catch (cms::Exception& x) {
    // nothing to do
  }
  catch (std::exception& x) {
    std::cout <<"caught std exception "<<x.what()<<std::endl;
    CPPUNIT_ASSERT("Threw std::exception!"==0);
  }
  catch (...) {
    CPPUNIT_ASSERT("Threw wrong kind of exception" == 0);
  }
  if( !didThrow) {
    CPPUNIT_ASSERT("Failed to throw required exception" == 0);      
  }
  
}

void testGenericHandle::getbyLabelTest() {
  std::string processName = "PROD";

  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::string label("fred");
  std::string productInstanceName("Rick");

  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();

  edm::BranchDescription product;

  product.fullClassName_ = dummytype.userClassName();
  product.friendlyClassName_ = className;

  edm::ModuleDescription modDesc;
  modDesc.moduleName_ = "Blah";

  product.moduleLabel_ = label;
  product.productInstanceName_ = productInstanceName;
  product.processName_ = processName;
  product.moduleDescriptionID_ = modDesc.id();
  product.init();

  edm::ProductRegistry *preg = new edm::ProductRegistry;
  preg->addProduct(product);
  preg->setProductIDs();

  edm::ProductRegistry::ProductList const& pl = preg->productList();
  edm::BranchKey const bk(product);
  edm::ProductRegistry::ProductList::const_iterator it = pl.find(bk);
  product.productID_ = it->second.productID_;

  edm::EventID col(1L, 1L);
  edm::Timestamp fakeTime;
  std::string uuid = edm::createGlobalIdentifier();
  edm::ProcessConfiguration pc("PROD", edm::ParameterSetID(), edm::getReleaseVersion(), edm::getPassID());
  boost::shared_ptr<edm::ProductRegistry const> pregc(preg);
  edm::RunAuxiliary runAux(col.run(), fakeTime, fakeTime);
  boost::shared_ptr<edm::RunPrincipal> rp(new edm::RunPrincipal(runAux, pregc, pc));
  edm::LuminosityBlockAuxiliary lumiAux(rp->run(), 1, fakeTime, fakeTime);
  boost::shared_ptr<edm::LuminosityBlockPrincipal>lbp(new edm::LuminosityBlockPrincipal(lumiAux, pregc, rp, pc));
  edm::EventAuxiliary eventAux(col, uuid, fakeTime, lbp->luminosityBlock(), true);
  edm::EventPrincipal ep(eventAux, pregc, lbp, pc);

  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance(product));
  ep.put(pprod, pprov);
  
  edm::GenericHandle h("edmtest::DummyProduct");
  try {
    edm::ModuleDescription modDesc;
    modDesc.moduleName_="Blah";
    modDesc.moduleLabel_="blahs"; 
    edm::Event event(ep, modDesc);

    event.getByLabel(label, productInstanceName,h);
  }
  catch (cms::Exception& x) {
    std::cerr << x.explainSelf()<< std::endl;
    CPPUNIT_ASSERT("Threw cms::Exception unexpectedly" == 0);
  }
  catch(std::exception& x){
     std::cerr <<x.what()<<std::endl;
     CPPUNIT_ASSERT("threw std::exception"==0);
  }
  catch (...) {
    std::cerr << "Unknown exception type\n";
    CPPUNIT_ASSERT("Threw exception unexpectedly" == 0);
  }
  CPPUNIT_ASSERT(h.isValid());
  CPPUNIT_ASSERT(h.provenance()->moduleLabel() == label);
}

