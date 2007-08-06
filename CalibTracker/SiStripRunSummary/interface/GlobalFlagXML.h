// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#ifndef GLOBAL_FLAG_XML_H
#define GLOBAL_FLAG_XML_H

#include <iosfwd>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/version.hpp>

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/ClassIDBase.h"

/**
* @brief
*   The same flag should also be implemented for Text/binary format. We need 
*   to declare partner that is used for conversion: GlobalFlagTxt <-> 
*   GlobalFlagXML
*/
class GlobalFlagTxt;

/** 
* @brief 
*   XML format flag. Tasks:
*     1. Define cloning
*     2. Implement identification (get class ID)
*     3. Specify allowed children
*     4. Serialization part:
*           a) (de)serialization through base class
*           b) register class in boost/serialization
*     5. flag output in ostream (print, display).
*/
class GlobalFlagXML: public FlagXML {
  public:
    GlobalFlagXML() {}
    
    /** 
    * @brief 
    *   Construct XML format flag from Txt formatted one
    * 
    * @param poGLOBAL_FLAGTXT  Txt format flag to be used as source
    */
    GlobalFlagXML( const GlobalFlagTxt *poGLOBAL_FLAGTXT);

  protected:
    /** 
    * @brief 
    *   Clone method is used in copying Tree. Should be implemented by each
    *   flag individually. Flag should be created on a heap store.
    * 
    * @return 
    *   Pointer to newly created flag.
    */
    inline virtual Clonable *clone() const {
      return new GlobalFlagXML( *this);
    }

    /** 
    * @brief 
    *   Convert XML format flag to Txt. Txt flag should be created on a heap
    *   store and constructed out of XML formatted one.
    * 
    * @return 
    *   Pointer to newly created flag.
    */
    virtual Clonable *cloneTxt() const;

    /** 
    * @brief 
    *   Used in (de)serialization process to get class ID.
    * 
    * @return 
    *   ID of current class.
    */
    virtual ClassIDBase::ID getID() const;

    /** 
    * @brief 
    *   Override current method in case flag may have any children.
    * 
    * @param poCHILD_CANDIDATE  Pointer to flag to be checked if it is allowed.
    * 
    * @return 
    *   Number of children allowed (refer to base class for details)
    */
    virtual int isChildValid( const FlagXML *poCHILD_CANDIDATE) const;

  private:
    /**
    * @brief
    *                                                   --[ BEGIN ]--
    *   Serialization section: XML format
    */
    friend class boost::serialization::access;

    template<class Archive>
      void serialize( Archive &roArchive, const unsigned int &rnVERSION) {
        roArchive & BOOST_SERIALIZATION_BASE_OBJECT_NVP( FlagXML);
      }
    /**
    * @brief
    *                                                   --[ END ]--
    */
};

/** 
* @brief 
*   Make flag printable. Flag output format is defined by:
*
*     std::ostream &operator <<( std::ostream &roOut, const Flag &roFLAG);
*
* @param roOut   Output stream where flag should be written
* @param roFLAG  Flag to be written
* 
* @return 
*   Reference to output stream.
*/
std::ostream &
  operator <<( std::ostream &roOut, const GlobalFlagXML &roFLAG);

/**
* @brief
*   Register class in boost/serialization otherwise flag can not be saved/read.
*   [IMPORTANT: Label (the second argument) should be unique (!). This is
*               pretty much the same as class ID]
*/
BOOST_CLASS_EXPORT_GUID( GlobalFlagXML, "GlobalFlagXML")

/**
* @brief
*   Register current class version. Given number will be used in serialization
*   process in should be changed once core changes in order to maintain any
*   saved tree before. For example, Flag class will be extended with additional
*   property, let's say: author. Any tree that was created before won't have 
*   given value. Thus attempting to read it would lead to runtime error.
*   Versioning was introduced to solve current issue.
*/
BOOST_CLASS_VERSION( GlobalFlagXML, 0)

#endif // GLOBAL_FLAG_XML_H
