// $Id: DTtTrigTestDisplayPlugins.cc,v 1.1 2007/03/29 09:10:24 gmasetti Exp $

/*!
  \file DTtTrigTestDisplayPlugins
  \brief Display Plugin for tTrig Client test Quality Histograms (2D)
  \author G. Masetti 
  \version $Revision: 1.1 $
  \date $Date: 2007/03/29 09:10:24 $
*/

#include "DQM/DTMonitorDisplayPlugins/src/DTtTrigTestDisplayPlugins.h"
//#include "DQM/DTMonitorDisplayPlugins/interface/ColorPalette.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include <iostream>
#include <TROOT.h>
#include <TGraph.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TVirtualPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TColor.h>



DTtTrigTestDisplayPlugins::DTtTrigTestDisplayPlugins () {

}

bool DTtTrigTestDisplayPlugins::istTrigTestME (std::string name) {

  if( name.find( "tTrigTest" ) == 0 ) {
    return true;
  }

  return false;

}


std::string DTtTrigTestDisplayPlugins::preDraw( VisDQMDisplayPlugin::DisplayData *data ) {

  if( dynamic_cast<TProfile2D*>( data->object ) ) {
    return preDrawTProfile2D( data );
  }

  if( dynamic_cast<TProfile*>( data->object ) ) {
    return preDrawTProfile( data );
  }

  if( dynamic_cast<TH2F*>( data->object ) ) {
    return preDrawTH2F( data );
  }
  
  if( dynamic_cast<TH1F*>( data->object ) ) {
    return preDrawTH1F( data );
  }
  
  return "";

}


std::string DTtTrigTestDisplayPlugins::preDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTtTrigTestDisplayPlugins::preDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";

}


std::string DTtTrigTestDisplayPlugins::preDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return "";    

}

std::string DTtTrigTestDisplayPlugins::preDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  TH1F* obj = dynamic_cast<TH1F*>( data->object );

  //name = (data->object)->GetName();

  if( obj ) {
    
    //     // This applies to all
    gStyle->SetCanvasBorderMode( 0 );
    gStyle->SetPadBorderMode( 0 );
    gStyle->SetPadBorderSize( 0 );
    //      (data->pad)->SetLogy( 1 );
    gStyle->SetOptStat( 0 );
    obj->SetStats( kFALSE );
    
    if ( name.find( "tTrigTest" ) == 0 ) {
      
      TAttLine *line = dynamic_cast<TAttLine *> (data->object);
      
      obj->GetXaxis()->SetBinLabel(1,"SL1");
      obj->GetXaxis()->SetBinLabel(2,"SL2");
      obj->GetXaxis()->SetBinLabel(3,"SL3");
      
      if (line) {
	
	MonitorElement* me = data->me;
	
	if (me->hasError()) {
	  line->SetLineColor(TColor::GetColor("#CC0000"));
	  //	  std::cout << name << " has error" << std::endl;
	}
	else if (me->hasWarning()) {
	  line->SetLineColor(TColor::GetColor("#993300"));
	  //	  std::cout << name << " has worning" << std::endl;
	}
	else if (me->hasOtherReport()) { 
	  line->SetLineColor(TColor::GetColor("#FFCC00"));
	  //	  std::cout << name << " has other report" << std::endl;
	}
	else {
	  line->SetLineColor(TColor::GetColor("#000000"));
	  //	  std::cout << name << " has nothing" << std::endl;
	  
	}
      }
      
    }
    
    
  }
  
  return "";    
  
}


void DTtTrigTestDisplayPlugins::postDraw( VisDQMDisplayPlugin::DisplayData *data ) {

  if( dynamic_cast<TProfile2D*>( data->object ) ) {
    return postDrawTProfile2D( data );
  }

  if( dynamic_cast<TProfile*>( data->object ) ) {
    return postDrawTProfile( data );
  }

  if( dynamic_cast<TH2F*>( data->object ) ) {
    return postDrawTH2F( data );
  }
  
  if( dynamic_cast<TH1F*>( data->object ) ) {
    return postDrawTH1F( data );
  }
  
  return ;

}

void DTtTrigTestDisplayPlugins::postDrawTProfile2D( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTtTrigTestDisplayPlugins::postDrawTProfile( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTtTrigTestDisplayPlugins::postDrawTH2F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

void DTtTrigTestDisplayPlugins::postDrawTH1F( VisDQMDisplayPlugin::DisplayData *data ) {

  return ;

}

