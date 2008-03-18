#ifndef PixelConfigBase_h
#define PixelConfigBase_h
/*! \file CalibFormats/SiPixelObjects/interface/PixelConfigBase.h
*   \brief This file contains the base class for "pixel configuration data" 
*          management
*
*   A longer explanation will be placed here later
*/
//
// Base class for pixel configuration data
// provide a place to implement common interfaces
// for these objects. Any configuration data
// object that is to be accessed from the database
// should derive from this class.
//

#include <string>


namespace pos{
/*!  \ingroup ConfigurationObjects "Configuration Objects"
*    
*  @{
*
*   \class PixelConfigBase PixelConfigBase.h "interface/PixelConfigBase.h"
*   \brief This file contains the base class for "pixel configuration data" 
*          management
*
*   A longer explanation will be placed here later
*/
  class PixelConfigBase {

  public:

    //A few things that you should provide
    //description : purpose of this object
    //creator : who created this configuration object
    //date : time/date of creation (should probably not be
    //       a string, but I have no idea what CMS uses.
    PixelConfigBase(std::string description,
		    std::string creator,
                    std::string date);

    virtual ~PixelConfigBase(){}

    std::string description();
    std::string creator();
    std::string date();

    //Interface to write out data to ascii file
    virtual void writeASCII(std::string dir="") const = 0;
    
  private:

    std::string description_;
    std::string creator_;
    std::string date_;
     

  };

/* @} */
}

#endif
