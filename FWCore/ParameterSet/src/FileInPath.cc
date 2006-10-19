// ----------------------------------------------------------------------
// $Id: FileInPath.cc,v 1.15 2006/10/18 22:16:18 wmtan Exp $
//
// ----------------------------------------------------------------------

// TODO: This file needs some clean-up, especially regarding the
// handling of environment variables. We can do better with
// translating them only once --- after we have settled down on how
// long the search path is allowed to be, and whether are only choice
// for the "official" directory is CMSSW_DATA_PATH.

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <string>
#include <vector>
#include<iostream> // temporary
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/parse.h"

namespace bf = boost::filesystem;

namespace 
{
  /// These are the names of the environment variables which control
/// the behavior  of the FileInPath  class.  They are local to  this
/// class; other code should not even know about them!
    
  const std::string PathVariableName("CMSSW_SEARCH_PATH");
  // Environment variables for local and release areas: 
  const std::string LOCALTOP("CMSSW_BASE");
  const std::string RELEASETOP("CMSSW_RELEASE_BASE");

  // String to serve as placeholder for release top. 
  // Do not change this value.
  const std::string BASE("BASE");

  // Return false if the environment variable 'name is not found, and
  // true if it is found. If it is found, put the translation of the
  // environment variable into 'result'.
  bool envstring(std::string const& name,
		 std::string& result)
  {
    const char* val = getenv(name.c_str());
    if (val == 0) {
      return false;
    }
    result = val;
    return true;
  }


  // Check for existence of a file for the given relative path and
  // 'prefix'.
  // Return true if a file (not directory or symbolic link) is found
  // Return false is *nothing* is found
  // Throw an exception if either a directory or symbolic link is found.
  // If true is returned, then put the 
  bool locateFile(bf::path  p,
		  std::string const& relative)
  {
    p /= relative;

    if (!bf::exists(p)) return false;

    if (bf::is_directory(p))
      throw edm::Exception(edm::errors::FileInPathError)
	<< "Path " 
	<< p.native_directory_string()
	<< " is a directory, not a file\n";

    if (bf::symbolic_link_exists(p))
      throw edm::Exception(edm::errors::FileInPathError)
	<< "Path " 
	<< p.native_file_string()
	<< " is a symbolic link, not a file\n";

    return true;    
  }

}

namespace edm
{

  FileInPath::FileInPath() :
    relativePath_(),
    canonicalFilename_(),
    isLocal_(false)
  { }

  FileInPath::FileInPath(const std::string& r) :
    relativePath_(r),
    canonicalFilename_(),
    isLocal_(false)
  {
    initialize_();
  }

  FileInPath::FileInPath(const char* r) :
    relativePath_(r ?
		  r :
		  ((throw edm::Exception(edm::errors::FileInPathError)
		    << "Relative path may not be null\n"), r)),
    canonicalFilename_(),
    isLocal_(false)
  {
    initialize_();    
  }

  FileInPath::FileInPath(FileInPath const& other) :
    relativePath_(other.relativePath_),
    canonicalFilename_(other.canonicalFilename_),
    isLocal_(other.isLocal_)
  {}

  FileInPath&
  FileInPath::operator= (FileInPath const& other)
  {
    FileInPath temp(other);
    this->swap(temp);
    return *this;
  }

  void
  FileInPath::swap(FileInPath& other)
  {
    relativePath_.swap(other.relativePath_);
    canonicalFilename_.swap(other.canonicalFilename_);
    std::swap(isLocal_, other.isLocal_);
  }

  std::string
  FileInPath::relativePath() const
  {
    return relativePath_;
  }


  bool
  FileInPath::isLocal() const
  {
    return isLocal_;
  }

  std::string
  FileInPath::fullPath() const
  {
#if 1
    // This #if needed for backward compatibility
    // for files written before CMSSW_0_8_0_pre2.
    if (canonicalFilename_.empty()) {
      throw edm::Exception(edm::errors::FileInPathError)
        << "Unable to find file "
        << relativePath_
        << " anywhere in the search path."
        << "\nThe search path is defined by: "
        << PathVariableName
        << "\n${"
        << PathVariableName
        << "} is: "
        << getenv(PathVariableName.c_str())
        << "\nCurrent directory is: "
        << bf::initial_path().string()
        << "\n";    
    }
#endif
    return canonicalFilename_;
  }

  void
  FileInPath::write(std::ostream& os) const
  {
    if (isLocal_ || canonicalFilename_.empty()) {
      // If a local copy of the file is used, we don't care if the persistent value is site independent.
      os << relativePath_ << ' ' << isLocal_ << ' ' << canonicalFilename_;    
    } else {
      // Guarantee a site independent value by substituting the literal BASE for the release top.
      std::string releaseTop;
      if (!envstring(RELEASETOP, releaseTop)) {
	throw edm::Exception(edm::errors::FileInPathError)
	  << "Environment Variable " 
	  << RELEASETOP
	  << " is not set.\n";
      }
      std::string::size_type pos = canonicalFilename_.find(releaseTop);
      if (pos != 0) {
	throw edm::Exception(edm::errors::FileInPathError)
	  << "Path " 
	  << canonicalFilename_
	  << " is not in the base release area "
	  << releaseTop
	  << "\n";
      }

      os << relativePath_ << ' ' << isLocal_ << ' ' << BASE << canonicalFilename_.substr(releaseTop.size());
    }
  }


  void
  FileInPath::read(std::istream& is)
  {
    std::string relname;
    bool        local;
    std::string canFilename;
#if 1
    // This #if needed for backward compatibility
    // for files written before CMSSW_0_8_0_pre2.
    is >> relname >> local;
    if (!is) return;
    is >> canFilename;
    if (!is) {
      canFilename = "";
      is.clear();
    }
#else
    is >> relname >> local >> canFilename;
    if (!is) return;
#endif
    relativePath_ = relname;
    isLocal_ = local;
    if (isLocal_ || canFilename.empty()) {
      canonicalFilename_ = canFilename;
    } else {
      std::string::size_type pos = canFilename.find(BASE);
      if (pos == 0) {
        // Replace the placehoder with the path to the base release (site dependent).
        std::string releaseTop;
        if (!envstring(RELEASETOP, releaseTop)) {
	  throw edm::Exception(edm::errors::FileInPathError)
	    << "Environment Variable " 
	    << RELEASETOP
	    << " is not set.\n";
        }
        canonicalFilename_ = releaseTop + canFilename.substr(BASE.size());
      } else {
#if 1
    // This #if needed for backward compatibility for files written before CMSSW_1_2_0_pre2.
      canonicalFilename_ = canFilename;
#else
      throw edm::Exception(edm::errors::FileInPathError)
	<< "Site independent 'path' " 
	<< canFilename
	<< " does not begin with the placeholder "
	<< BASE
	<< "\n";
#endif
      }
    }
  }

  //------------------------------------------------------------


  void 
  FileInPath::initialize_()
  {
    if (relativePath_.empty())
      throw edm::Exception(edm::errors::FileInPathError)
	<< "Relative path may not be empty\n";

    // Find the file, based on the value of path variable.
    std::string searchPath;
    if (!envstring(PathVariableName, searchPath))
      throw edm::Exception(edm::errors::FileInPathError)
	<< PathVariableName
	<< " must be defined\n";

    typedef std::vector<std::string> stringvec_t;
    stringvec_t  pathElements = edm::pset::tokenize(searchPath, ":");
    stringvec_t::const_iterator it =  pathElements.begin();
    stringvec_t::const_iterator end = pathElements.end();
    while (it != end) {
      bf::path pathPrefix("", bf::no_check);

      // Set the boost::fs path to the current element of
      // CMSSW_SEARCH_PATH:
      
      pathPrefix = bf::path(*it, bf::no_check);

      // Does the a file exist? locateFile throws is it finds
      // something goofy.
      if (locateFile(pathPrefix, relativePath_)) {
	// Convert relative path to canonical form, and save it.
	relativePath_ = bf::path(relativePath_, bf::no_check).normalize().string();
	  
	// Save the absolute path.
	canonicalFilename_ = bf::complete(relativePath_, 
					  pathPrefix).string();
	if (canonicalFilename_.empty())
	  throw edm::Exception(edm::errors::FileInPathError)
	    << "fullPath is empty"
	    << "\nrelativePath() is: " << relativePath_
	    << "\npath prefix is: " << pathPrefix.string()
	    << '\n';

	// From the current path element, find the branch path (basically the path minus the
	// last directory, e.g. /src or /share):
	bf::path br = pathPrefix.branch_path();	   	    

	std::string localTop;
	isLocal_ = false;

	// Check that LOCALTOP really has a value and store it:
	if (!envstring(LOCALTOP, localTop))
	  throw edm::Exception(edm::errors::FileInPathError)
	    << LOCALTOP
	    << " must be defined - is runtime environment set correctly?\n";

	// Create a path object for our local path LOCALTOP:
	bf::path local_(localTop, bf::no_check);
	    
	// If the branch path matches the local path, the file was found locally:
	if (br == local_) {
	  isLocal_ = true;
	}
	    
	// We're done...indeed.
	    
	// This is really gross --- this organization of if/else
	// inside the while-loop should be changed so that
	// this break isn't needed.
	return;
      }
      // Keep trying
      ++it;
    }
    
    // If we got here, we ran out of path elements without finding
    // what we're looking found.
    throw edm::Exception(edm::errors::FileInPathError)
      << "Unable to find file "
      << relativePath_
      << " anywhere in the search path."
      << "\nThe search path is defined by: "
      << PathVariableName
      << "\n${"
      << PathVariableName
      << "} is: "
      << getenv(PathVariableName.c_str())
      << "\nCurrent directory is: "
      << bf::initial_path().string()
      << "\n";    
  }
    
  
}



