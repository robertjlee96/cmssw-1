#ifndef FWLite_stdNamespaceAdder_h
#define FWLite_stdNamespaceAdder_h
// -*- C++ -*-
//
// Package:     FWLite
// Class  :     stdNamespaceAdder
// 
/**\class stdNamespaceAdder stdNamespaceAdder.h FWCore/RootAutoLibraryLoader/interface/stdNamespaceAdder.h

 Description: Adds back the 'std::' namespace prefix to standard classes

 Usage:
    <usage>

*/
//
// Original Author:  
//         Created:  Tue Dec  6 09:18:09 EST 2005
// $Id$
//

// system include files
#include <string>

// user include files

// forward declarations
namespace edm {
  namespace root {
    std::string stdNamespaceAdder(const std::string&);
  }
}
#endif
