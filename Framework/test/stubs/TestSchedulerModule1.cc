/**
   \file
   Test Modules for ScheduleBuilder

   \author Stefano ARGIRO
   \version $Id$
   \date 19 May 2005
*/


#include "FWCore/CoreFramework/interface/EDProducer.h"
#include "FWCore/CoreFramework/interface/Event.h"
#include "FWCore/CoreFramework/interface/Handle.h"
#include "FWCore/CoreFramework/interface/MakerMacros.h"

#include "FWCore/CoreFramework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/CoreFramework/src/ToyProducts.h"
#include <memory>
#include <string>

using namespace edm;

static const char CVSId[] = "$Id$";

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

  std::string myname = getP<std::string>(pset_, "module_name");
  std::auto_ptr<edmtest::StringProduct> product(new edmtest::StringProduct(myname)); 
  e.put(product);
}

DEFINE_FWK_MODULE(TestSchedulerModule1)






// Configure (x)emacs for this file ...
// Local Variables:
// mode:c++
// compile-command: "make -C .. -k"
// End:
