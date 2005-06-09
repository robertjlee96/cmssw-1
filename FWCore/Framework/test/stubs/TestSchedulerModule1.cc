/**
   \file
   Test Modules for ScheduleBuilder

   \author Stefano ARGIRO
   \version $Id: TestSchedulerModule1.cc,v 1.1 2005/05/29 02:29:54 wmtan Exp $
   \date 19 May 2005
*/


#include "FWCore/CoreFramework/interface/EDProducer.h"
#include "FWCore/CoreFramework/interface/Event.h"
#include "FWCore/CoreFramework/interface/Handle.h"
#include "FWCore/CoreFramework/interface/MakerMacros.h"

#include "FWCore/CoreFramework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/CoreFramework/test/stubs/DummyProduct.h"
#include <memory>
#include <string>

using namespace edm;

static const char CVSId[] = "$Id: TestSchedulerModule1.cc,v 1.1 2005/05/29 02:29:54 wmtan Exp $";

class TestSchedulerModule1 : public EDProducer
{
 public:
  explicit TestSchedulerModule1(ParameterSet const& p):pset_(p){}

  void produce(Event& e, EventSetup const&);

private:
  ParameterSet pset_;
};


void TestSchedulerModule1::produce(Event& e, EventSetup const&)
{

  std::string myname = pset_.getString("module_name");
  std::auto_ptr<edmtest::DummyProduct> product(new edmtest::DummyProduct);
  product->setName(myname);
  e.put(product);
}

DEFINE_FWK_MODULE(TestSchedulerModule1)






// Configure (x)emacs for this file ...
// Local Variables:
// mode:c++
// compile-command: "make -C .. -k"
// End:
