/**
   \file
   class impl

   \Original author Stefano ARGIRO
   \Current author Bill Tanenbaum
   \version $Id: ProductRegistry.cc,v 1.4 2007/05/25 23:18:33 wmtan Exp $
   \date 19 Jul 2005
*/

static const char CVSId[] = "$Id: ProductRegistry.cc,v 1.4 2007/05/25 23:18:33 wmtan Exp $";


#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "FWCore/Utilities/interface/ReflexTools.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include <algorithm>
#include <sstream>

namespace edm {
  void
  ProductRegistry::addProduct(BranchDescription const& productDesc,
			      bool fromListener) {
    throwIfFrozen();
    productDesc.init();
    checkDictionaries(productDesc.fullClassName(), productDesc.transient());
    productList_.insert(std::make_pair(BranchKey(productDesc), productDesc));
    addCalled(productDesc,fromListener);
  }
  
  void
  ProductRegistry::copyProduct(BranchDescription const& productDesc) {
    throwIfFrozen();
    productDesc.init();
    productList_.insert(std::make_pair(BranchKey(productDesc), productDesc));
    if (productDesc.productID().id_ >= nextID_) {
      nextID_ = productDesc.productID().id_ + 1;
    }
  }
  
  void
  ProductRegistry::setProductIDs() {
    checkAllDictionaries();
    throwIfFrozen();
    for (ProductList::iterator it = productList_.begin(), itEnd = productList_.end();
        it != itEnd; ++it) {
      if (it->second.productID().id_ == 0) {
        it->second.productID_.id_ = nextID_++;
      }
    }
    frozen_ = true;
    initializeTransients();
  }
  
  void
  ProductRegistry::setFrozen() const {
    if(frozen_) return;
    frozen_ = true;
    initializeTransients();
  }
  
  void
  ProductRegistry::throwIfFrozen() const {
    if (frozen_) {
      throw cms::Exception("ProductRegistry", "throwIfFrozen")
            << "cannot modify the ProductRegistry because it is frozen";
    }
  }
  
  void
  ProductRegistry::throwIfNotFrozen() const {
/*
    if (!frozen_) {
      throw cms::Exception("ProductRegistry", "throwIfNotFrozen")
            << "cannot read the ProductRegistry because it is not yet frozen";
    }
*/
  }
  
  void
  ProductRegistry::addCalled(BranchDescription const&, bool) {
  }

  std::string
  ProductRegistry::merge(ProductRegistry const& other,
	std::string const& fileName,
	BranchDescription::MatchMode m) {
    std::ostringstream differences;

    ProductRegistry::ProductList::iterator j = productList_.begin();
    ProductRegistry::ProductList::iterator s = productList_.end();
    ProductRegistry::ProductList::const_iterator i = other.productList().begin();
    ProductRegistry::ProductList::const_iterator e = other.productList().end();

    // Loop over entries in the main product registry.
    while(j != s || i != e) {
      if (j != s && j->second.produced()) {
	// Ignore branches just produced (i.e. not in input file).
	++j;
      } else if (j == s || i->first < j->first) {
	differences << "Branch '" << i->second.branchName() << "' is in file '" << fileName << "'\n";
	differences << "    but not in previous files.\n";
	++i;
      } else if (i == e || j->first < i->first) {
	differences << "Branch '" << j->second.branchName() << "' is in previous files\n";
	differences << "    but not in file '" << fileName << "'.\n";
	++j;
      } else {
	std::string difs = match(j->second, i->second, fileName, m);
	if (difs.empty()) {
	  if (m == BranchDescription::Permissive) j->second.merge(i->second);
	} else {
	  differences << difs;
	}
	++i;
	++j;
      }
    }
    initializeTransients();
    return differences.str();
  }

  void ProductRegistry::initializeTransients() const {
    constProductList_.clear();
    productLookup_.clear();
    elementLookup_.clear();
    for (ProductList::const_iterator i = productList_.begin(), e = productList_.end(); i != e; ++i) {
      constProductList_.insert(std::make_pair(i->first, ConstBranchDescription(i->second)));
      if (i->second.productID().id() > maxID_) {
        maxID_ = i->second.productID().id();    
      }
	

      ProcessLookup& processLookup = productLookup_[i->first.friendlyClassName_];
      std::vector<ProductID>& vint = processLookup[i->first.processName_];
      vint.push_back(i->second.productID());
      //[could use productID instead]
        
      ROOT::Reflex::Type type(ROOT::Reflex::Type::ByName(i->second.className()));
      if (bool(type)) {
        
        // Here we look in the object named "type" for a typedef
        // named "value_type" and get the Reflex::Type for it.
        // Then check to ensure the Reflex dictionary is defined
        // for this value_type.
        // I do not throw an exception here if the check fails
        // because there are known cases where the dictionary does
        // not exist and we do not need to support those cases.
        ROOT::Reflex::Type valueType;
        //        if (edm::value_type_of(type, valueType) && bool(valueType)) {
        if ((edm::is_RefVector(type, valueType) || edm::value_type_of(type, valueType)) 
            && bool(valueType)) {
          
          fillElementLookup(valueType, i->second.productID(), i->first);
          
          // Repeat this for all public base classes of the value_type
          std::vector<ROOT::Reflex::Type> baseTypes;
          edm::public_base_classes(valueType, baseTypes);
          
          for (std::vector<ROOT::Reflex::Type>::iterator iter = baseTypes.begin(),
	       iend = baseTypes.end();
               iter != iend;
               ++iter) {
            fillElementLookup(*iter, i->second.productID(), i->first);
          }
        }
      }
    }
  }

  void ProductRegistry::fillElementLookup(const ROOT::Reflex::Type & type,
                                          const edm::ProductID& id,
                                          const BranchKey& bk) const
  {
    TypeID typeID(type.TypeInfo());
    std::string friendlyClassName = typeID.friendlyClassName();
    
    ProcessLookup& processLookup = elementLookup_[friendlyClassName];
    std::vector<ProductID>& vint = processLookup[bk.processName_];
    vint.push_back(id);    
  }
  
  void ProductRegistry::print(std::ostream& os) const {
    for (ProductList::const_iterator i = productList_.begin(), e = productList_.end(); i != e; ++i) {
	os << i->second << "\n-----\n";
    }
  }

}
