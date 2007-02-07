/*
 *  servicesmanager_t.cppunit.cc
 *  CMSSW
 *
 *  Created by Chris Jones on 9/5/05.
 *
 */
//NOTE: I need to open a 'back door' so I can test ServiceManager 'inheritance'
#define private public
#include "FWCore/ServiceRegistry/interface/ServiceToken.h"
#undef private

#include "FWCore/ServiceRegistry/interface/ServicesManager.h"
#include "FWCore/ServiceRegistry/interface/ServicePluginFactory.h"
#include "FWCore/ServiceRegistry/interface/ServiceMakerBase.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include <cppunit/extensions/HelperMacros.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/test/stubs/DummyService.h"
#include "FWCore/ServiceRegistry/test/stubs/DependsOnDummyService.h"
#include "FWCore/PluginManager/interface/ProblemTracker.h"
#include <iostream>

class testServicesManager: public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(testServicesManager);
   
   CPPUNIT_TEST(putGetTest);
   CPPUNIT_TEST(loadTest);
   CPPUNIT_TEST(legacyTest);
   CPPUNIT_TEST(dependencyTest);
   
   CPPUNIT_TEST_SUITE_END();
public:
      void setUp(){}
   void tearDown(){}
   
   void putGetTest();
   void loadTest();
   void legacyTest();
   void dependencyTest();
};

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testServicesManager);

namespace {
   struct DummyService {
      //DummyService(const edm::ParameterSet&,
      //             edm::ActivityRegistry&) {}
   };
}

//namespace edm {
//   struct ActivityRegistry {};
//}

void
testServicesManager::putGetTest()
{

   using namespace edm::serviceregistry;

   std::vector<edm::ParameterSet> ps;
   ServicesManager sm(ps);
   

   CPPUNIT_ASSERT(!sm.isAvailable<DummyService>());
   bool exceptionThrown = true;
   try {
      sm.get<DummyService>();
      exceptionThrown = false;
   } catch(const edm::Exception&) {
   }
   CPPUNIT_ASSERT(exceptionThrown);
   
   std::auto_ptr< DummyService > pService(new DummyService);
   boost::shared_ptr< ServiceWrapper<DummyService> > 
      ptrWrapper (new ServiceWrapper<DummyService>(pService));

   CPPUNIT_ASSERT(sm.put(ptrWrapper));

   CPPUNIT_ASSERT(sm.isAvailable<DummyService>());

   sm.get<DummyService>();

   CPPUNIT_ASSERT(!sm.put(ptrWrapper));
}


void
testServicesManager::loadTest()
{
   typedef testserviceregistry::DummyService TestService;

   using namespace edm::serviceregistry;

   edm::AssertHandler ah;

   {
      std::vector<edm::ParameterSet> pss;
      
      edm::ParameterSet ps;
      std::string typeName("DummyService");
      ps.addParameter("@service_type", typeName);
      int value = 1;
      ps.addParameter("value", value);
      pss.push_back(ps);
      
      ServicesManager sm(pss);
      
      CPPUNIT_ASSERT(1 == sm.get<TestService>().value());
   }
   {
      std::vector<edm::ParameterSet> pss;
      
      edm::ParameterSet ps;
      std::string typeName("DoesntExistService");
      ps.addParameter("@service_type", typeName);
      pss.push_back(ps);
      
      bool threwConfigurationException = false;
      try {
         ServicesManager sm(pss);
      } catch(const edm::Exception&) {
         threwConfigurationException = true;
      }
      
      CPPUNIT_ASSERT(threwConfigurationException);
   }
   {
      std::vector<edm::ParameterSet> pss;
      
      edm::ParameterSet ps;
      std::string typeName("DummyService");
      ps.addParameter("@service_type", typeName);
      int value = 1;
      ps.addParameter("value", value);
      pss.push_back(ps);
      pss.push_back(ps);
      
      bool caughtMultipleServiceError = false;
      try {
         ServicesManager sm(pss);
      } catch(const edm::Exception&) {
         caughtMultipleServiceError = true;
      }
      
      CPPUNIT_ASSERT(caughtMultipleServiceError);
   }
   //NEED A TEST FOR SERVICES THAT DEPEND ON OTHER SERVICES
}   


void
testServicesManager::legacyTest()
{
   typedef testserviceregistry::DummyService TestService;
   
   using namespace edm::serviceregistry;
   
   edm::AssertHandler ah;

   std::vector<edm::ParameterSet> pss;
   
   edm::ParameterSet ps;
   std::string typeName("DummyService");
   ps.addParameter("@service_type", typeName);
   int value = 1;
   ps.addParameter("value", value);
   pss.push_back(ps);
   
   boost::shared_ptr<ServicesManager>  legacy(new ServicesManager(pss));
   CPPUNIT_ASSERT(1 == legacy->get<TestService>().value());
   
   edm::ServiceToken legacyToken(legacy);
   {
      std::vector<edm::ParameterSet> pss;
      
      edm::ParameterSet ps;
      std::string typeName("DummyService");
      ps.addParameter("@service_type", typeName);
      int value = 2;
      ps.addParameter("value", value);
      pss.push_back(ps);
      
      bool threwConfigurationException = false;
      try {
         ServicesManager sm(legacyToken, kOverlapIsError, pss);
      } catch(const edm::Exception&) {
         threwConfigurationException = true;
      }
      
      CPPUNIT_ASSERT(threwConfigurationException);
   }
   {
      std::vector<edm::ParameterSet> pss;
      
      edm::ParameterSet ps;
      std::string typeName("DummyService");
      ps.addParameter("@service_type", typeName);
      int value = 2;
      ps.addParameter("value", value);
      pss.push_back(ps);
      
      ServicesManager sm(legacyToken, kTokenOverrides, pss);
      
      CPPUNIT_ASSERT(1 == sm.get<TestService>().value());
   }
   {
      std::vector<edm::ParameterSet> pss;
      
      edm::ParameterSet ps;
      std::string typeName("DummyService");
      ps.addParameter("@service_type", typeName);
      int value = 2;
      ps.addParameter("value", value);
      pss.push_back(ps);
      
      ServicesManager sm(legacyToken, kConfigurationOverrides, pss);
      
      CPPUNIT_ASSERT(2 == sm.get<TestService>().value());
   }
   {
      try {
         std::vector<edm::ParameterSet> pss;
         
         ServicesManager sm(legacyToken, kOverlapIsError, pss);
         
         CPPUNIT_ASSERT(!sm.get<TestService>().beginJobCalled());
         edm::ActivityRegistry ar;
         sm.connectTo(ar);
         ar.postBeginJobSignal_();
         
         CPPUNIT_ASSERT(sm.get<TestService>().beginJobCalled());
      }catch(const edm::Exception& iException) {
         std::cout<<iException.what()<<std::endl;
         throw;
      }catch(const std::exception& iException) {
         std::cout<<iException.what()<<std::endl;
         throw;
      }
   }
}   

void
testServicesManager::dependencyTest()
{
   //Try both order of creating services
   typedef testserviceregistry::DummyService TestService;
   
   using namespace edm::serviceregistry;
   
   edm::AssertHandler ah;
   
   {
      std::vector<edm::ParameterSet> pss;
      {
         edm::ParameterSet ps;
         std::string typeName("DummyService");
         ps.addParameter("@service_type", typeName);
         int value = 1;
         ps.addParameter("value", value);
         pss.push_back(ps);
      }
      {
         edm::ParameterSet ps;
         std::string typeName("DependsOnDummyService");
         ps.addParameter("@service_type", typeName);
         pss.push_back(ps);
      }
      
      ServicesManager sm(pss);
      
      CPPUNIT_ASSERT(1 == sm.get<testserviceregistry::DependsOnDummyService>().value());
   }
   {
      std::vector<edm::ParameterSet> pss;
      {
         edm::ParameterSet ps;
         std::string typeName("DependsOnDummyService");
         ps.addParameter("@service_type", typeName);
         pss.push_back(ps);
      }
      {
         edm::ParameterSet ps;
         std::string typeName("DummyService");
         ps.addParameter("@service_type", typeName);
         int value = 1;
         ps.addParameter("value", value);
         pss.push_back(ps);
      }
      
      ServicesManager sm(pss);
      
      CPPUNIT_ASSERT(1 == sm.get<testserviceregistry::DependsOnDummyService>().value());
      
   }
}   
