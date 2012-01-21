/** \class HLTSmartSinglet
 *
 * See header file for documentation
 *
 *  $Date: 2011/05/01 14:41:36 $
 *  $Revision: 1.9 $
 *
 *  \author Martin Grunewald
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTfilters/interface/HLTSmartSinglet.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//
// constructors and destructor
//
template<typename T, int Tid>
HLTSmartSinglet<T,Tid>::HLTSmartSinglet(const edm::ParameterSet& iConfig) : HLTFilter(iConfig), 
  inputTag_ (iConfig.template getParameter<edm::InputTag>("inputTag")),
  cut_      (iConfig.template getParameter<std::string>  ("cut"     )),
  min_N_    (iConfig.template getParameter<int>          ("MinN"    )),
  select_   (cut_                                                    )
{
   LogDebug("") << "Input/cut/ncut : " << inputTag_.encode() << " " << cut_<< " " << min_N_ ;
}

template<typename T, int Tid>
HLTSmartSinglet<T,Tid>::~HLTSmartSinglet()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
template<typename T, int Tid> 
bool
HLTSmartSinglet<T,Tid>::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs & filterproduct)
{
   using namespace std;
   using namespace edm;
   using namespace reco;
   using namespace trigger;

   typedef vector<T> TCollection;
   typedef Ref<TCollection> TRef;

   // All HLT filters must create and fill an HLT filter object,
   // recording any reconstructed physics objects satisfying (or not)
   // this HLT filter, and place it in the Event.

   // The filter object
   if (saveTags()) filterproduct.addCollectionTag(inputTag_);

   // Ref to Candidate object to be recorded in filter object
   TRef ref;

   // get hold of collection of objects
   Handle<TCollection> objects;
   iEvent.getByLabel (inputTag_,objects);

   // look at all objects, check cuts and add to filter object
   int n(0);
   typename TCollection::const_iterator i ( objects->begin() );
   for (; i!=objects->end(); i++) {
     if (select_(*i)) {
       n++;
       ref=TRef(objects,distance(objects->begin(),i));
       filterproduct.addObject(Tid,ref);
     }
   }

   // filter decision
   bool accept(n>=min_N_);

   return accept;
}
