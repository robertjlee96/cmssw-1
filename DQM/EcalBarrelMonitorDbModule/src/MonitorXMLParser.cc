// $Id: $

/*!
  \file MonitorXMLParser.cc
  \brief monitor db xml elements parsing tool
  \author B. Gobbo 
  \version $Revision: $
  \date $Date: $
*/

#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>

#include "DQM/EcalBarrelMonitorDbModule/interface/MonitorXMLParser.h"

MonitorXMLParser::MonitorXMLParser( const std::string& fromFile ) {

  try{

    xercesc::XMLPlatformUtils::Initialize();

  }catch( xercesc::XMLException& e ){

    char* message = xercesc::XMLString::transcode( e.getMessage() ) ;

    std::cerr << "XML toolkit initialization error: " << message << std::endl;

    xercesc::XMLString::release( &message );

    exit( ERROR_XERCES_INIT );

  }

  xmlFile_ = fromFile; 
  parser_  = new xercesc::XercesDOMParser();
  tags_    = new TagNames();

}

// - - - - - - - - - - - - - - - - - 

MonitorXMLParser::~MonitorXMLParser() throw() {

  try{
    xercesc::XMLPlatformUtils::Terminate();
  } catch ( xercesc::XMLException& e ){
    char* message = xercesc::XMLString::transcode( e.getMessage() );
    std::cerr << "XML toolkit teardown error: " << message << std::endl;
    xercesc::XMLString::release( &message ) ;
  }

}



// - - - - - - - - - - - - - - - - - 

void MonitorXMLParser::handleElement( xercesc::DOMElement* element ){
    
  if( xercesc::XMLString::equals( tags_->TAG_ME, element->getTagName() ) ) {
    
    char* c;
    std::stringstream s;
    DB_ME me;
    bool meok;
    
    meok = false;
    
    xercesc::DOMNodeList* d1Nodes = element->getElementsByTagName( tags_->TAG_1D );
    const XMLSize_t d1Count = d1Nodes->getLength();
    
    for( XMLSize_t d1Index = 0; d1Index < d1Count; ++d1Index ){
      
      xercesc::DOMNode* d1Node = d1Nodes->item( d1Index ) ;
      
      xercesc::DOMElement* d1Element = dynamic_cast< xercesc::DOMElement* >( d1Node ) ;
      
      const XMLCh* d1titleXMLCh = d1Element->getAttribute( tags_->ATTR_TITLE ) ;
      c = xercesc::XMLString::transcode( d1titleXMLCh ); 
      me.type = "th1d";
      me.title = c;
      meok = true;
      xercesc::XMLString::release( &c );
      
      const XMLCh* d1xbinsXMLCh = d1Element->getAttribute( tags_->ATTR_XBINS ) ;
      c = xercesc::XMLString::transcode( d1xbinsXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xbins;
      xercesc::XMLString::release( &c );
      
      const XMLCh* d1xfromXMLCh = d1Element->getAttribute( tags_->ATTR_XFROM ) ;
      c = xercesc::XMLString::transcode( d1xfromXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xfrom;
      xercesc::XMLString::release( &c );

      const XMLCh* d1xtoXMLCh = d1Element->getAttribute( tags_->ATTR_XTO ) ;
      c = xercesc::XMLString::transcode( d1xtoXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xto;
      xercesc::XMLString::release( &c );
      
      me.ybins = 0;
      me.yfrom = 0.0;
      me.yto = 0.0;
      
    }
    
    xercesc::DOMNodeList* d2Nodes = element->getElementsByTagName( tags_->TAG_2D );
    const XMLSize_t d2Count = d2Nodes->getLength();
    
    for( XMLSize_t d2Index = 0; d2Index < d2Count; ++d2Index ){
	
      xercesc::DOMNode* d2Node = d2Nodes->item( d2Index ) ;
      
      xercesc::DOMElement* d2Element = dynamic_cast< xercesc::DOMElement* >( d2Node ) ;
      
      const XMLCh* d2titleXMLCh = d2Element->getAttribute( tags_->ATTR_TITLE ) ;
      c = xercesc::XMLString::transcode( d2titleXMLCh ); 
      me.type = "th2d";
      me.title = c;
      meok = true;
      xercesc::XMLString::release( &c );
      
      const XMLCh* d2xbinsXMLCh = d2Element->getAttribute( tags_->ATTR_XBINS ) ;
      c = xercesc::XMLString::transcode( d2xbinsXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xbins;
      xercesc::XMLString::release( &c );

      const XMLCh* d2xfromXMLCh = d2Element->getAttribute( tags_->ATTR_XFROM ) ;
      c = xercesc::XMLString::transcode( d2xfromXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xfrom;
      xercesc::XMLString::release( &c );
      
      const XMLCh* d2xtoXMLCh = d2Element->getAttribute( tags_->ATTR_XTO ) ;
      c = xercesc::XMLString::transcode( d2xtoXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xto;
      xercesc::XMLString::release( &c );

      const XMLCh* d2ybinsXMLCh = d2Element->getAttribute( tags_->ATTR_YBINS ) ;
      c = xercesc::XMLString::transcode( d2ybinsXMLCh ); 
      s.clear(); s.str( c );
      s >> me.ybins;
      xercesc::XMLString::release( &c );

      const XMLCh* d2yfromXMLCh = d2Element->getAttribute( tags_->ATTR_YFROM ) ;
      c = xercesc::XMLString::transcode( d2yfromXMLCh ); 
      s.clear(); s.str( c );
      s >> me.yfrom;
      xercesc::XMLString::release( &c );

      const XMLCh* d2ytoXMLCh = d2Element->getAttribute( tags_->ATTR_YTO ) ;
      c = xercesc::XMLString::transcode( d2ytoXMLCh ); 
      s.clear(); s.str( c );
      s >> me.yto;
      xercesc::XMLString::release( &c );
      
    }
    
    xercesc::DOMNodeList* tpNodes = element->getElementsByTagName( tags_->TAG_TPROFILE );
    const XMLSize_t tpCount = tpNodes->getLength();
	
    for( XMLSize_t tpIndex = 0; tpIndex < tpCount; ++tpIndex ){
      
      xercesc::DOMNode* tpNode = tpNodes->item( tpIndex ) ;
      
      xercesc::DOMElement* tpElement = dynamic_cast< xercesc::DOMElement* >( tpNode ) ;
      
      const XMLCh* tptitleXMLCh = tpElement->getAttribute( tags_->ATTR_TITLE ) ;
      c = xercesc::XMLString::transcode( tptitleXMLCh ); 
      me.type = "tprofile";
      me.title = c;
      meok = true;
      xercesc::XMLString::release( &c );
      
      const XMLCh* tpxbinsXMLCh = tpElement->getAttribute( tags_->ATTR_XBINS ) ;
      c = xercesc::XMLString::transcode( tpxbinsXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xbins;
      xercesc::XMLString::release( &c );
      
      const XMLCh* tpxfromXMLCh = tpElement->getAttribute( tags_->ATTR_XFROM ) ;
      c = xercesc::XMLString::transcode( tpxfromXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xfrom;
      xercesc::XMLString::release( &c );

      const XMLCh* tpxtoXMLCh = tpElement->getAttribute( tags_->ATTR_XTO ) ;
      c = xercesc::XMLString::transcode( tpxtoXMLCh ); 
      s.clear(); s.str( c );
      s >> me.xto;
      xercesc::XMLString::release( &c );
      
      const XMLCh* tpybinsXMLCh = tpElement->getAttribute( tags_->ATTR_YBINS ) ;
      c = xercesc::XMLString::transcode( tpybinsXMLCh ); 
      s.clear(); s.str( c );
      s >> me.ybins;
      xercesc::XMLString::release( &c );
      
      const XMLCh* tpyfromXMLCh = tpElement->getAttribute( tags_->ATTR_YFROM ) ;
      c = xercesc::XMLString::transcode( tpyfromXMLCh ); 
      s.clear(); s.str( c );
      s >> me.yfrom;
      xercesc::XMLString::release( &c );
      
      const XMLCh* tpytoXMLCh = tpElement->getAttribute( tags_->ATTR_YTO ) ;
      c = xercesc::XMLString::transcode( tpytoXMLCh ); 
      s.clear(); s.str( c );
      s >> me.yto;
      xercesc::XMLString::release( &c );
      
    }
      
      
    xercesc::DOMNodeList* qNodes = element->getElementsByTagName( tags_->TAG_QUERY );
    const XMLSize_t qCount = qNodes->getLength();
    
    for( XMLSize_t qIndex = 0; qIndex < qCount; ++qIndex ){
      
      xercesc::DOMNode* qNode = qNodes->item( qIndex ) ;
      
      xercesc::DOMElement* tpElement = dynamic_cast< xercesc::DOMElement* >( qNode ) ;
      
      const XMLCh* nameXMLCh = tpElement->getAttribute( tags_->ATTR_NAME ) ;
      c = xercesc::XMLString::transcode( nameXMLCh ); 
      
      const XMLCh* argXMLCh = tpElement->getAttribute( tags_->ATTR_ARG ) ;
      char* d = xercesc::XMLString::transcode( argXMLCh ); 
      
      me.queries.insert(std::pair<std::string, std::string>( c, d ));
      
      xercesc::XMLString::release( &c );
      xercesc::XMLString::release( &d );
      
      
    }
    
    if( meok ) DBMonitoringElements_.push_back( me );
    
  }   
} // handleElement()


// - - - - - - - - - - - - - - - - - - - 

void MonitorXMLParser::load() throw( std::runtime_error ) {
    
  parser_->setValidationScheme( xercesc::XercesDOMParser::Val_Never );
  parser_->setDoNamespaces( false );
  parser_->setDoSchema( false );
  parser_->setLoadExternalDTD( false );
    
  try{
      
    parser_->parse( xmlFile_.c_str() );
    
    xercesc::DOMDocument* xmlDoc = parser_->getDocument();
    
    xercesc::DOMElement* ebme = xmlDoc->getDocumentElement();
    
    if( NULL == ebme ){
      throw( std::runtime_error( "empty XML document" ) ) ;
    }
    
    xercesc::DOMNodeList* children = ebme->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
    
    for( XMLSize_t ix = 0 ; ix < nodeCount ; ++ix ){
      xercesc::DOMNode* currentNode = children->item( ix );
      if( NULL == currentNode ){
	// null node...
	continue;
      }
      
      if( xercesc::DOMNode::ELEMENT_NODE != currentNode->getNodeType() ){
	continue;
      }
      
      xercesc::DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );
      
      handleElement( currentElement );
      
    }
    
  }catch( xercesc::XMLException& e ){
    
    char* message = xercesc::XMLString::transcode( e.getMessage() );
    
    std::ostringstream buf ;
    buf << "Error parsing file: " << message << std::flush;
    
    xercesc::XMLString::release( &message );
    
    throw( std::runtime_error( buf.str() ) );
    
  }catch( const xercesc::DOMException& e ){
    
    char* message = xercesc::XMLString::transcode( e.getMessage() );
    
    std::ostringstream buf;
    buf << "Encountered DOM Exception: " << message << std::flush;
    
    xercesc::XMLString::release( &message );
    
    throw( std::runtime_error( buf.str() ) );
    
  }
  
  return;
  
} // load()

