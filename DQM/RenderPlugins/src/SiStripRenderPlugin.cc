/*!
  \file SiStripRenderPlugin
  \brief Display Plugin for SiStrip DQM Histograms
  \author S. Dutta
  \version $Revision: 1.17 $
  \date $Date: 2009/10/31 23:18:54 $
*/

#include "VisMonitoring/DQMServer/interface/DQMRenderPlugin.h"
#include "utils.h"

#include "TProfile2D.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TText.h"
#include "TLine.h"
#include <cassert>

class SiStripRenderPlugin : public DQMRenderPlugin
{
public:
  virtual bool applies( const VisDQMObject &o, const VisDQMImgInfo & )
    {
      if ((o.name.find( "SiStrip/" ) == std::string::npos) &&
	  (o.name.find( "Tracking/" ) == std::string::npos))
         return false;

      if( o.name.find( "/EventInfo/" ) != std::string::npos )
        return true;

      if( o.name.find( "/MechanicalView/" ) != std::string::npos )
        return true;

      if( o.name.find( "/ReadoutView/" ) != std::string::npos )
        return true;

      if( o.name.find( "/TrackParameters/" ) != std::string::npos )
        return true;

      return false;
    }

  virtual void preDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo &, VisDQMRenderInfo & )
    {
      c->cd();

      if( dynamic_cast<TH2F*>( o.object ) )
      {
        preDrawTH2F( c, o );
      }
      else if( dynamic_cast<TH1F*>( o.object ) )
      {
        preDrawTH1F( c, o );
      }
      else if( dynamic_cast<TProfile2D*>( o.object ) )
      {
        preDrawTProfile2D( c, o );
      }
      else if( dynamic_cast<TProfile*>( o.object ) )
      {
        preDrawTProfile( c, o );
      }
    }

  virtual void postDraw( TCanvas *c, const VisDQMObject &o, const VisDQMImgInfo & )
    {
      c->cd();

      if( dynamic_cast<TH1F*>( o.object ) )
      {
        postDrawTH1F( c, o );
      }
      if( dynamic_cast<TH2F*>( o.object ) )
      {
        postDrawTH2F( c, o );
      }
      if( dynamic_cast<TProfile*>( o.object ) )
      {
        postDrawTProfile( c, o );
      }
    }

private:
  void preDrawTH2F( TCanvas *, const VisDQMObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //    (data->pad)->SetLogy( 0 );;
      //  gStyle->SetOptStat( 0 );

      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.05);
      ya->SetLabelSize(0.04);

      if( o.name.find( "PedsEvolution" ) != std::string::npos)
      {
        gStyle->SetOptStat( 1111 );
        obj->SetStats( kTRUE );
        obj->SetOption( "lego2" );
        return;
      }
      if( o.name.find( "CMDistribution " )  != std::string::npos)
      {
        obj->GetXaxis()->LabelsOption("d");
        obj->SetOption( "lego2" );
        return;
      }
      if( o.name.find( "CMSlopeDistribution " )  != std::string::npos)
      {
        obj->GetXaxis()->LabelsOption("d");
        obj->SetOption( "lego2" );
        return;
      }
      if( o.name.find( "PedestalDistribution " )  != std::string::npos)
      {
        obj->GetXaxis()->LabelsOption("d");
        obj->SetOption( "lego" );
        return;
      }
      if( o.name.find( "reportSummaryMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colztext");
        return;
      }
      if( o.name.find( "detFractionReportMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colztext");
        return;
      }
      if( o.name.find( "sToNReportMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        dqm::utils::reportSummaryMapPalette(obj);
        obj->SetOption("colztext");
        return;
      }
      if( o.name.find( "SummaryOfCabling" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetOption("text");
        return;
      }
      if( o.name.find( "TkHMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetOption("colz");
        return;
      }
      return;
    }

  void preDrawTH1F( TCanvas *, const VisDQMObject &o )
    {
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetOptStat(1110);
      //  if ( obj->GetMaximum(1.e5) > 0. ) {
      //    gPad->SetLogy(1);
      //  } else {
      //   gPad->SetLogy(0);
      //  }
      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.04);
      ya->SetLabelSize(0.04);

      if( o.name.find( "Summary_MeanNumberOfDigis" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetMaximum(2.0);
        obj->SetMinimum(-0.1);
        return;
      }
      if( o.name.find( "Summary_MeanNumberOfClusters" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetMaximum(0.05);
        obj->SetMinimum(-0.001);
        return;
      }
      if( o.name.find( "Summary_MeanClusterWidth" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetMaximum(20.0);
        obj->SetMinimum(-1.0);
        return;
      }
    }

  void preDrawTProfile2D( TCanvas *, const VisDQMObject &o )
    {
      TProfile2D* obj = dynamic_cast<TProfile2D*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //    (data->pad)->SetLogy( 0 );;
      //  gStyle->SetOptStat( 0 );

      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.05);
      ya->SetLabelSize(0.04);

      if( o.name.find( "TkHMap" )  != std::string::npos)
      {
        obj->SetStats( kFALSE );
        obj->SetOption("colz");
        return;
      }
      return;
    }
  void preDrawTProfile( TCanvas *, const VisDQMObject &o )
    {
      TProfile* obj = dynamic_cast<TProfile*>( o.object );
      assert( obj );

      // This applies to all
      gStyle->SetCanvasBorderMode( 0 );
      gStyle->SetPadBorderMode( 0 );
      gStyle->SetPadBorderSize( 0 );
      //    (data->pad)->SetLogy( 0 );;
      //  gStyle->SetOptStat( 0 );

      TAxis* xa = obj->GetXaxis();
      TAxis* ya = obj->GetYaxis();

      xa->SetTitleOffset(0.7);
      xa->SetTitleSize(0.05);
      xa->SetLabelSize(0.04);

      ya->SetTitleOffset(0.7);
      ya->SetTitleSize(0.05);
      ya->SetLabelSize(0.04);

      obj->SetStats( kFALSE );
      obj->SetOption("e");

      return;
    }
  void postDrawTH1F( TCanvas *c, const VisDQMObject &o )
    {
     
      TH1F* obj = dynamic_cast<TH1F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "NumberOfTracks_" ) != std::string::npos ||
          name.find( "Chi2overDoF_" ) != std::string::npos )
	{
	  c->SetLogy(1);
	  c->SetGridy();
        }

      TText tt;
      tt.SetTextSize(0.12);
      if (o.flags == 0) return;
      else
      {
        if (o.flags & DQMNet::DQM_PROP_REPORT_ERROR)
        {
          tt.SetTextColor(2);
          tt.DrawTextNDC(0.5, 0.5, "Error");
        }
        else if (o.flags & DQMNet::DQM_PROP_REPORT_WARN)
        {
          tt.SetTextColor(5);
          tt.DrawTextNDC(0.5, 0.5, "Warning");
        }
        else if (o.flags & DQMNet::DQM_PROP_REPORT_OTHER)
        {
          tt.SetTextColor(1);
          tt.DrawTextNDC(0.5, 0.5, "Other ");
        }
      }
    }

  void postDrawTH2F( TCanvas *c, const VisDQMObject &o )
    {
      TH2F* obj = dynamic_cast<TH2F*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      if( name.find( "reportSummaryMap" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
      if( name.find( "detFractionReportMap" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
      if( name.find( "sToNReportMap" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
      if( name.find( "SummaryOfCabling" ) != std::string::npos )
      {
        c->SetGridx();
        c->SetGridy();
        return;
      }
    }

  void postDrawTProfile( TCanvas *c, const VisDQMObject &o )
    {
      TProfile* obj = dynamic_cast<TProfile*>( o.object );
      assert( obj );

      std::string name = o.name.substr(o.name.rfind("/")+1);

      TLine tl1;
      tl1.SetLineColor(3);
      tl1.SetLineWidth(3);

      TLine tl2;
      tl2.SetLineColor(4);
      tl2.SetLineWidth(3);

      float xmin = 0.0;
      float xmax = obj->GetXaxis()->GetXmax();
      float ymax = obj->GetMaximum()*1.2;

      float TIBLimit1 = 900.0;
      float TOBLimit1 = 2000.0;
      float TECLimit1 = 2250.0;
      float TIDLimit1 = 320.0;

      float TIBLimit2 = 1787904.0;
      float TOBLimit2 = 3303936.0;
      float TECLimit2 = 3866624.0;
      float TIDLimit2 = 565248.0;

      if( name.find( "TotalNumberOfDigiProfile__" ) != std::string::npos )
      {
        c->SetLogy(1);
        c->SetGridy();
        if (name.find( "TotalNumberOfDigiProfile__TIB" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, TIBLimit1,  xmax, TIBLimit1);
          tl2.DrawLine(xmin, TIBLimit2*0.0005, xmax, TIBLimit2*0.0005);
          tl2.DrawLine(xmin, TIBLimit2*0.01,  xmax, TIBLimit2*0.01);
          obj->SetMinimum(TIBLimit1*0.1);
          obj->SetMaximum(TMath::Max(ymax, TIBLimit1*50));
        } else if (name.find( "TotalNumberOfDigiProfile__TOB" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, TOBLimit1,  xmax, TOBLimit1);
          tl2.DrawLine(xmin, TOBLimit2*0.0005, xmax, TOBLimit2*0.0005);
          tl2.DrawLine(xmin, TOBLimit2*0.01,  xmax, TOBLimit2*0.01);
          obj->SetMinimum(TOBLimit1*0.1);
          obj->SetMaximum(TMath::Max(ymax, TOBLimit1*50));
        } else if (name.find( "TotalNumberOfDigiProfile__TEC" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, TECLimit1,  xmax, TECLimit1);
          tl2.DrawLine(xmin, TECLimit2*0.0005, xmax, TECLimit2*0.0005);
          tl2.DrawLine(xmin, TECLimit2*0.01,  xmax, TECLimit2*0.01);
          obj->SetMinimum(TECLimit1*0.1);
          obj->SetMaximum(TMath::Max(ymax, TECLimit1*50));
        } else if (name.find( "TotalNumberOfDigiProfile__TID" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, TIDLimit1,  xmax, TIDLimit1);
          tl2.DrawLine(xmin, TIDLimit2*0.0005, xmax, TIDLimit2*0.0005);
          tl2.DrawLine(xmin, TIDLimit2*0.01,  xmax, TIDLimit2*0.01);
          obj->SetMinimum(TIDLimit1*0.1);
          obj->SetMaximum(TMath::Max(ymax, TIDLimit1*50));
       }
        return;
      }
      if( name.find( "TotalNumberOfClusterProfile__" ) != std::string::npos )
      {
        c->SetLogy(1);
        c->SetGridy();
        if (name.find( "TotalNumberOfClusterProfile__TIB" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, 5.0,  xmax, 5.0);
          tl2.DrawLine(xmin, TIBLimit2*0.0001, xmax, TIBLimit2*0.0001);
          tl2.DrawLine(xmin, TIBLimit2*5*0.00001,  xmax, TIBLimit2*5*0.00001);
        } else if (name.find( "TotalNumberOfClusterProfile__TOB" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, 15.0, xmax, 15.0);
          tl2.DrawLine(xmin, TOBLimit2*0.0001, xmax, TOBLimit2*0.0001);
          tl2.DrawLine(xmin, TOBLimit2*5*0.00001,  xmax, TOBLimit2*5*0.00001);
        } else if (name.find( "TotalNumberOfClusterProfile__TEC" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, 15.0, xmax,15.0);
          tl2.DrawLine(xmin, TECLimit2*0.0001, xmax, TECLimit2*0.0001);
          tl2.DrawLine(xmin, TECLimit2*5*0.00001,  xmax, TECLimit2*5*0.00001);
        } else if (name.find( "TotalNumberOfClusterProfile__TID" ) != std::string::npos) {
	  //          tl1.DrawLine(xmin, 3.0,  xmax, 3.0);
          tl2.DrawLine(xmin, TIDLimit2*0.0001, xmax, TIDLimit2*0.0001);
          tl2.DrawLine(xmin, TIDLimit2*5*0.00001,  xmax, TIDLimit2*5*0.00001);
        }
        return;
      }
    }
};

static SiStripRenderPlugin instance;
