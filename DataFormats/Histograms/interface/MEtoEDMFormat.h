#ifndef MEtoEDMFormat_h
#define MEtoEDMFormat_h

/** \class MEtoEDM
 *  
 *  DataFormat class to hold the information from a ME tranformed into
 *  ROOT objects as appropriate
 *
 *  $Date: 2008/09/22 21:01:04 $
 *  $Revision: 1.8 $
 *  \author M. Strang SUNY-Buffalo
 */

#include <TObject.h>
#include <TH1F.h>
#include <TH1S.h>
#include <TH2F.h>
#include <TH2S.h>
#include <TH3F.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TObjString.h>
#include <TString.h>

#include <string>
#include <vector>
#include <memory>
#include <map>

template <class T>
class MEtoEDM
{
 public:
  MEtoEDM() {}
  virtual ~MEtoEDM() {}

  typedef std::vector<uint32_t> TagList;

  struct MEtoEDMObject
  {
    std::string	name;
    TagList 	tags;
    T	        object;
    std::string release;
    int run;
    std::string datatier;
  };

  typedef std::vector<MEtoEDMObject> MEtoEdmObjectVector;

  void putMEtoEdmObject(const std::vector<std::string> &name,
			const std::vector<TagList> &tags,
			const std::vector<T> &object,
			const std::vector<std::string> &release,
			const std::vector<int> &run,
			const std::vector<std::string> &datatier)
    {
      MEtoEdmObject.resize(name.size());
      for (unsigned int i = 0; i < name.size(); ++i) {
	MEtoEdmObject[i].name = name[i];
	MEtoEdmObject[i].tags = tags[i];
	MEtoEdmObject[i].object = object[i];
	MEtoEdmObject[i].release = release[i];
	MEtoEdmObject[i].run = run[i];
	MEtoEdmObject[i].datatier = datatier[i];
      }
    }

  const MEtoEdmObjectVector & getMEtoEdmObject() const
    { return MEtoEdmObject; }

  bool mergeProduct(const MEtoEDM<T> &newMEtoEDM) {
    const MEtoEdmObjectVector &newMEtoEDMObject = 
      newMEtoEDM.getMEtoEdmObject();
    bool warn = false;
    std::vector<bool> tmp(newMEtoEDMObject.size(), false);
    for (unsigned int i = 0; i < MEtoEdmObject.size(); ++i) {
      unsigned int j = 0;
      while (j < newMEtoEDMObject.size() &&
             (strcmp(MEtoEdmObject[i].name.c_str(),
                     newMEtoEDMObject[j].name.c_str()) != 0)) ++j;
      if (j < newMEtoEDMObject.size()) {
        MEtoEdmObject[i].object.Add(&newMEtoEDMObject[j].object);
        tmp[j] = true;
      } else {
        warn = true;
      }
    }
    for (unsigned int j = 0; j < newMEtoEDMObject.size(); ++j) {
      if (!tmp[j]) {
        warn = true;
        MEtoEdmObject.push_back(newMEtoEDMObject[j]);
      }
    }
    if (warn) {
      std::cout << "WARNING: problem found in MEtoEDM::mergeProducts()" << std::endl;
    }
    return true;
  }

 private:

  MEtoEdmObjectVector MEtoEdmObject;

}; // end class declaration

template <>
inline bool
MEtoEDM<double>::mergeProduct(const MEtoEDM<double> &newMEtoEDM)
{ return true; }

template <>
inline bool
MEtoEDM<int>::mergeProduct(const MEtoEDM<int> &newMEtoEDM)
{
 const MEtoEdmObjectVector &newMEtoEDMObject =
   newMEtoEDM.getMEtoEdmObject();
 for (unsigned int i = 0; i < MEtoEdmObject.size(); ++i) {
   if ( MEtoEdmObject[i].name.find("EventInfo/processedEvents") != std::string::npos ) {
     MEtoEdmObject[i].object += (newMEtoEDMObject[i].object);
   }
   if ( MEtoEdmObject[i].name.find("EventInfo/iEvent") != std::string::npos ||
        MEtoEdmObject[i].name.find("EventInfo/iLumiSection") != std::string::npos) {
        if (MEtoEdmObject[i].object < newMEtoEDMObject[i].object) 
                           MEtoEdmObject[i].object = (newMEtoEDMObject[i].object);
   }
   
 }
 return true;
}

template <>
inline bool
MEtoEDM<TString>::mergeProduct(const MEtoEDM<TString> &newMEtoEDM)
{ return true; }

#endif
