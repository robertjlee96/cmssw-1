// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEveLegoView
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Thu Feb 21 11:22:41 EST 2008
// $Id: FWEveLegoView.cc,v 1.13 2008/06/23 06:25:48 dmytro Exp $
//

// system include files
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "TRootEmbeddedCanvas.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TClass.h"
#include "TH2F.h"
#include "TView.h"
#include "TColor.h"
#include "TEveScene.h"
#include "TGLViewer.h"
#include "TGLEmbeddedViewer.h"
#include "TEveViewer.h"
#include "TEveManager.h"
#include "TEveElement.h"
#include "TEveCalo.h"
#include "TEveElement.h"
#include "TEveRGBAPalette.h"
#include "TGLPerspectiveCamera.h"
#include "TEveLegoEventHandler.h"
#include "TGLWidget.h"
#include "TEveTrans.h"

// user include files
#include "Fireworks/Core/interface/FWEveLegoView.h"
#include "Fireworks/Core/interface/FWConfiguration.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FWEveLegoView::FWEveLegoView(TGFrame* iParent, TEveElementList* list):
 m_minEcalEnergy(this,"ECAL energy threshold (GeV)",0.,0.,100.),
 m_minHcalEnergy(this,"HCAL energy threshold (GeV)",0.,0.,100.),
 m_ecalSlice(0),
 m_hcalSlice(0),
 m_cameraMatrix(0),
 m_cameraMatrixBase(0),
 m_cameraSet(false)
{
   m_pad = new TEvePad;
   TGLEmbeddedViewer* ev = new TGLEmbeddedViewer(iParent, m_pad);
   m_embeddedViewer=ev;
   TEveViewer* nv = new TEveViewer(staticTypeName().c_str());
   nv->SetGLViewer(ev);
   nv->IncDenyDestroy();
   // ev->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
   ev->SetCurrentCamera(TGLViewer::kCameraPerspXOY);
   ev->SetEventHandler(new TEveLegoEventHandler("Lego", ev->GetGLWidget(), ev));
   m_cameraMatrix = const_cast<TGLMatrix*>(&(ev->CurrentCamera().GetCamTrans()));
   m_cameraMatrixBase = const_cast<TGLMatrix*>(&(ev->CurrentCamera().GetCamBase()));

   TEveScene* ns = gEve->SpawnNewScene(staticTypeName().c_str());
   m_scene = ns;
   nv->AddScene(ns);
   m_viewer=nv;
   gEve->AddElement(nv, gEve->GetViewers());
   
   TEveRGBAPalette* pal = new TEveRGBAPalette(0, 100);
   // pal->SetLimits(0, data->GetMaxVal());
   pal->SetLimits(0, 100);
   pal->SetDefaultColor((Color_t)1000);
   
   m_lego = new TEveCaloLego();
   m_lego->InitMainTrans();
   m_lego->RefMainTrans().SetScale(2*M_PI, 2*M_PI, M_PI);
   
   m_lego->SetPalette(pal);
   // m_lego->SetMainColor(Color_t(TColor::GetColor("#0A0A0A")));
   m_lego->SetGridColor(Color_t(TColor::GetColor("#202020")));
   m_lego->Set2DMode(TEveCaloLego::kValSize);
   m_lego->SetBinWidth(6);
   // lego->SetEtaLimits(etaLimLow, etaLimHigh);
   // lego->SetTitle("caloTower Et distribution");
   gEve->AddElement(m_lego, ns);
   gEve->AddToListTree(m_lego, kTRUE);
   gEve->AddElement(list,ns);
   gEve->AddToListTree(list, kTRUE);
   m_minEcalEnergy.changed_.connect(boost::bind(&FWEveLegoView::setMinEcalEnergy,this,_1));
   m_minHcalEnergy.changed_.connect(boost::bind(&FWEveLegoView::setMinHcalEnergy,this,_1));
}

FWEveLegoView::~FWEveLegoView()
{
}

void
FWEveLegoView::draw(TEveCaloDataHist* data)
{
   // bool firstTime = (m_lego->GetData() == 0);
   m_lego->SetData(data);
   m_lego->ElementChanged();
   m_lego->DataChanged();
   if ( ! m_cameraSet ) {
      m_scene->Repaint();
      m_viewer->Redraw(kTRUE);
      // std::cout << "Viewer: " <<  m_viewer << std::endl;
      // m_viewer->GetGLViewer()->ResetCameras();
      m_cameraSet = true;
   }
   m_viewer->GetGLViewer()->RequestDraw();
}

void 
FWEveLegoView::setMinEcalEnergy(double value)
{
   const std::string name = "ecalLego";
   if ( ! m_lego->GetData() ) return;
   if ( ! m_ecalSlice )
     for ( int i = 0; i < m_lego->GetData()->GetNSlices(); ++i )
       if ( name == m_lego->GetData()->RefSliceInfo(i).fHist->GetName() )
	 {
	    m_ecalSlice = &(m_lego->GetData()->RefSliceInfo(i));
	    break;
	 }
   if ( ! m_ecalSlice ) return;
   m_ecalSlice->fThreshold = value;
   m_lego->ElementChanged();
   m_lego->DataChanged();
   m_viewer->GetGLViewer()->RequestDraw();
}

void 
FWEveLegoView::setMinHcalEnergy(double value)
{
   const std::string name = "hcalLego";
   if ( ! m_lego->GetData() ) return;
   if ( ! m_hcalSlice )
     for ( int i = 0; i < m_lego->GetData()->GetNSlices(); ++i )
       if ( name == m_lego->GetData()->RefSliceInfo(i).fHist->GetName() )
	 {
	    m_hcalSlice = &(m_lego->GetData()->RefSliceInfo(i));
	    break;
	 }
   if ( ! m_hcalSlice ) return;
   m_hcalSlice->fThreshold = value;
   m_lego->ElementChanged();
   m_lego->DataChanged();
   m_viewer->GetGLViewer()->RequestDraw();
}

void 
FWEveLegoView::setMinEnergy()
{
   setMinEcalEnergy( m_minEcalEnergy.value() );
   setMinHcalEnergy( m_minHcalEnergy.value() );
}

void 
FWEveLegoView::setFrom(const FWConfiguration& iFrom)
{
   // take care of parameters
   FWConfigurableParameterizable::setFrom(iFrom);
   
   // retrieve camera parameters
   
   // transformation matrix
   assert(m_cameraMatrix);
   std::string matrixName("cameraMatrix");
   for ( unsigned int i = 0; i < 16; ++i ){
      std::ostringstream os;
      os << i;
      const FWConfiguration* value = iFrom.valueForKey( matrixName + os.str() + "Lego" );
      assert( value );
      std::istringstream s(value->value());
      s>>((*m_cameraMatrix)[i]);
   }
   
   // transformation matrix base
   assert(m_cameraMatrixBase);
   matrixName = "cameraMatrixBase";
   for ( unsigned int i = 0; i < 16; ++i ){
      std::ostringstream os;
      os << i;
      const FWConfiguration* value = iFrom.valueForKey( matrixName + os.str() + "Lego" );
      assert( value );
      std::istringstream s(value->value());
      s>>((*m_cameraMatrixBase)[i]);
   }

   m_viewer->GetGLViewer()->RequestDraw();
   m_cameraSet = true;
}

//
// const member functions
//
TGFrame* 
FWEveLegoView::frame() const
{
   return m_embeddedViewer->GetFrame();
}

const std::string& 
FWEveLegoView::typeName() const
{
   return staticTypeName();
}

void 
FWEveLegoView::addTo(FWConfiguration& iTo) const
{
   // take care of parameters
   FWConfigurableParameterizable::addTo(iTo);
   
   // store camera parameters
   
   // transformation matrix
   assert(m_cameraMatrix);
   std::string matrixName("cameraMatrix");
   for ( unsigned int i = 0; i < 16; ++i ){
      std::ostringstream osIndex;
      osIndex << i;
      std::ostringstream osValue;
      osValue << (*m_cameraMatrix)[i];
      iTo.addKeyValue(matrixName+osIndex.str()+"Lego",FWConfiguration(osValue.str()));
   }
   
   // transformation matrix base
   assert(m_cameraMatrixBase);
   matrixName = "cameraMatrixBase";
   for ( unsigned int i = 0; i < 16; ++i ){
      std::ostringstream osIndex;
      osIndex << i;
      std::ostringstream osValue;
      osValue << (*m_cameraMatrixBase)[i];
      iTo.addKeyValue(matrixName+osIndex.str()+"Lego",FWConfiguration(osValue.str()));
   }
}

void 
FWEveLegoView::saveImageTo(const std::string& iName) const
{
   bool succeeded = m_viewer->GetGLViewer()->SavePicture(iName.c_str());
   if(!succeeded) {
      throw std::runtime_error("Unable to save picture");
   }
}


//
// static member functions
//
const std::string& 
FWEveLegoView::staticTypeName()
{
   static std::string s_name("3D Lego");
   return s_name;
}

