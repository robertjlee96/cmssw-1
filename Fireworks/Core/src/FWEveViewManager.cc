// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEveViewManager
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Chris Jones, Alja Mrak-Tadel
//         Created:  Thu Mar 18 14:11:32 CET 2010
// $Id: FWEveViewManager.cc,v 1.25 2010/05/27 18:45:32 amraktad Exp $
//

// system include files

// user include files
#include "TEveSelection.h"
#include "TEveManager.h"
#include "TEveScene.h"
#include "TEveCompound.h"
#include "TEveCalo.h"
#include "TGLViewer.h"
#include "TSystem.h"

// common
#include "Fireworks/Core/interface/FWEveViewManager.h"
#include "Fireworks/Core/interface/FWSelectionManager.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/Core/interface/Context.h"
#include "Fireworks/Core/interface/FWInteractionList.h"

// PB
#include "Fireworks/Core/interface/FWEDProductRepresentationChecker.h"
#include "Fireworks/Core/interface/FWSimpleRepresentationChecker.h"
#include "Fireworks/Core/interface/FWTypeToRepresentations.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWProxyBuilderBase.h"
#include "Fireworks/Core/interface/FWProxyBuilderFactory.h"

// viewes
#include "Fireworks/Core/interface/FWGUIManager.h"
#include "Fireworks/Core/interface/FWISpyView.h"
#include "Fireworks/Core/interface/FW3DView.h"
#include "Fireworks/Core/interface/FWGlimpseView.h"
#include "Fireworks/Core/interface/FWEveLegoView.h"
#include "Fireworks/Core/interface/FWHFView.h"
#include "Fireworks/Core/interface/FWRPZView.h"


class DetIdToMatrix;
class FWViewContext;

// sentry class block TEveSelection signals when call TEveSelection::Remove/AddElement and
// when process its callbacks
class EveSelectionSentry {
public:
   EveSelectionSentry()
   {
      m_blocked = gEve->GetSelection()->BlockSignals(true);
   }
   ~EveSelectionSentry()
   {
     gEve->GetSelection()->BlockSignals(m_blocked);
   }
private:
   bool m_blocked;
};

//
//
// constants, enums and typedefs
//
//
// constructors and destructor
//
FWEveViewManager::FWEveViewManager(FWGUIManager* iGUIMgr) :
   FWViewManagerBase()
{
     
   // builders
   std::set<std::string> builders;
   
   std::vector<edmplugin::PluginInfo> available = FWProxyBuilderFactory::get()->available();
   std::transform(available.begin(),
                  available.end(),
                  std::inserter(builders,builders.begin()),
                  boost::bind(&edmplugin::PluginInfo::name_,_1));
   
   if(edmplugin::PluginManager::get()->categoryToInfos().end()!=edmplugin::PluginManager::get()->categoryToInfos().find(FWProxyBuilderFactory::get()->category())) {
      available = edmplugin::PluginManager::get()->categoryToInfos().find(FWProxyBuilderFactory::get()->category())->second;
      std::transform(available.begin(),
                     available.end(),
                     std::inserter(builders,builders.begin()),
                     boost::bind(&edmplugin::PluginInfo::name_,_1));
   }
   
   
   for(std::set<std::string>::iterator it = builders.begin(), itEnd=builders.end();
       it!=itEnd;
       ++it) {
      std::string::size_type first = it->find_first_of('@')+1;
      std::string purpose = it->substr(first,it->find_last_of('@')-first);
      
      first = it->find_last_of('@')+1;
      std::string view_str =  it->substr(first,it->find_last_of('#')-first);
      int viewTypes = atoi(view_str.c_str());
      std::string fullName = *it;
      m_typeToBuilder[purpose].push_back(BuilderInfo(*it, viewTypes));
   }
   
   
   m_views.resize(FWViewType::kSize); 
   
   
   // view construction called via GUI mng
   // note:: this could be simplifed if all view types would use FWViewType

   FWGUIManager::ViewBuildFunctor f[FWViewType::kSize];
   f[FWViewType::kRhoPhi   ] = boost::bind(&FWEveViewManager::createRhoPhiView   , this, _1);
   f[FWViewType::kRhoZ     ] = boost::bind(&FWEveViewManager::createRhoZView     , this, _1);
   f[FWViewType::kISpy     ] = boost::bind(&FWEveViewManager::createISpyView     , this, _1);
   f[FWViewType::k3D       ] = boost::bind(&FWEveViewManager::create3DView       , this, _1);
   f[FWViewType::kLego     ] = boost::bind(&FWEveViewManager::createLegoView     , this, _1);
   f[FWViewType::kLegoHF   ] = boost::bind(&FWEveViewManager::createLegoHFView   , this, _1);
   f[FWViewType::kGlimpse  ] = boost::bind(&FWEveViewManager::createGlimpseView  , this, _1);

   for (int i = 0; i < FWViewType::kSize; i++)
      iGUIMgr->registerViewBuilder(FWViewType::idToName(i), f[i]);


   // signal
   gEve->GetHighlight()->SetPickToSelect(TEveSelection::kPS_Master);
   TEveSelection* eveSelection = gEve->GetSelection();
   eveSelection->SetPickToSelect(TEveSelection::kPS_Master);
   eveSelection->Connect("SelectionAdded(TEveElement*)","FWEveViewManager",this,"selectionAdded(TEveElement*)");
   eveSelection->Connect("SelectionRepeated(TEveElement*)","FWEveViewManager",this,"selectionAdded(TEveElement*)");
   eveSelection->Connect("SelectionRemoved(TEveElement*)","FWEveViewManager",this,"selectionRemoved(TEveElement*)");
   eveSelection->Connect("SelectionCleared()","FWEveViewManager",this,"selectionCleared()");
}

FWEveViewManager::~FWEveViewManager()
{
}

//
// member functions
//

//______________________________________________________________________________

void
FWEveViewManager::newItem(const FWEventItem* iItem)
{
   TypeToBuilder::iterator itFind = m_typeToBuilder.find(iItem->purpose());
   if (itFind != m_typeToBuilder.end())
   {  
      std::vector<BuilderInfo>& blist = itFind->second;
      for ( std::vector<BuilderInfo>::iterator bIt= blist.begin(); bIt != blist.end(); ++bIt)
      {
         // create builder
         std::string builderName = (*bIt).m_name;
         int builderViewBit =  (*bIt).m_viewBit;
         
         FWProxyBuilderBase* builder = FWProxyBuilderFactory::get()->create(builderName);
         if (builder)
         {
            // printf("FWEveViewManager::makeProxyBuilderFor NEW builder %s \n", builderName.c_str());
            
            boost::shared_ptr<FWProxyBuilderBase> pB( builder );
            builder->setItem(iItem);
            iItem->changed_.connect(boost::bind(&FWEveViewManager::modelChanges,this,_1));
            iItem->goingToBeDestroyed_.connect(boost::bind(&FWEveViewManager::removeItem,this,_1));
            iItem->itemChanged_.connect(boost::bind(&FWEveViewManager::itemChanged,this,_1));

            if (builder->willHandleInteraction() == false)
            {
               FWInteractionList* il = 0;
               std::map<const FWEventItem*, FWInteractionList*>::iterator inter_it = m_interactionLists.find(iItem);
               if (inter_it == m_interactionLists.end())
               {
                  il = new FWInteractionList(iItem);
                  m_interactionLists[iItem] = il;
               }
               else
               {
                  il = inter_it->second;
               }
               //  printf(">>> builder %s add list %p \n", iItem->name().c_str(), il); fflush(stdout);
               builder->setInteractionList(il, iItem->purpose());
            }

            builder->setHaveWindow(haveViewForBit(builderViewBit));
            
            // supported views
            for (int viewType = 0;  viewType < FWViewType::kSize; ++viewType)
            {
               int viewBit = 1 << viewType;

               if (viewBit & builderViewBit)
               {   
                  // printf("%s builder %s supportsd view %s \n",  iItem->name().c_str(), builderName.c_str(), FWViewType::idToName(viewType).c_str());
                  if (builder->havePerViewProduct((FWViewType::EType)viewType))
                  { 
                     for (EveViewVec_it i = m_views[viewType].begin(); i!= m_views[viewType].end(); ++i)
                     {
                        TEveElementList* product = builder->createProduct((FWViewType::EType)viewType, i->get()->viewContext());

                        if (FWViewType::isProjected(viewType))
                        {
                           FWRPZView* rpzView = dynamic_cast<FWRPZView*> (i->get());
                           rpzView->importElements(product, iItem->layer(), rpzView->ownedProducts());
                        }
                        else
                        {
                           i->get()->ownedProducts()->AddElement(product);
                        }
                     }
                  }
                  else 
                  {
                     TEveElementList* product = builder->createProduct((FWViewType::EType)viewType, 0);

                     for (EveViewVec_it i = m_views[viewType].begin(); i!= m_views[viewType].end(); ++i)
                     { 
                        if (FWViewType::isProjected(viewType))
                        {
                           FWRPZView* rpzView = dynamic_cast<FWRPZView*> (i->get());
                           rpzView->importElements(product, iItem->layer(), rpzView->eventScene());
                        }
                        else
                        {
                           i->get()->eventScene()->AddElement(product);
                        }
                     }
                  }
               }
            }

            m_builders[builderViewBit].push_back(pB);
            
            
         } // if builder
      } // loop views      
   } // loop purposes
}

//______________________________________________________________________________

FWViewBase*
FWEveViewManager::createISpyView(TEveWindowSlot* iParent)
{
   FWViewType::EType t = FWViewType::kISpy;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FWISpyView(iParent, t)));
   return finishViewCreate(m_views[t].back());   
}

FWViewBase*
FWEveViewManager::create3DView(TEveWindowSlot* iParent)
{
   FWViewType::EType t = FWViewType::k3D;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FW3DView(iParent, t)));
   return finishViewCreate(m_views[t].back());   
}

FWViewBase*
FWEveViewManager::createRhoPhiView(TEveWindowSlot* iParent)
{
   FWViewType::EType t = FWViewType::kRhoPhi;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FWRPZView(iParent, t)));
   return finishViewCreate(m_views[t].back());   
}

FWViewBase*
FWEveViewManager::createRhoZView(TEveWindowSlot* iParent)
{ 
   FWViewType::EType t = FWViewType::kRhoZ;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FWRPZView(iParent, t)));
   return finishViewCreate(m_views[t].back());   
}

FWViewBase*
FWEveViewManager::createLegoView(TEveWindowSlot* iParent)
{
   FWViewType::EType t = FWViewType::kLego;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FWEveLegoView(iParent, t)));
   return finishViewCreate(m_views[t].back());
}

FWViewBase*
FWEveViewManager::createLegoHFView(TEveWindowSlot* iParent)
{
   FWViewType::EType t = FWViewType::kLegoHF;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FWHFView(iParent, t)));
   return finishViewCreate(m_views[t].back());
}

FWViewBase*
FWEveViewManager::createGlimpseView(TEveWindowSlot* iParent)
{      
   FWViewType::EType t = FWViewType::kGlimpse;
   m_views[t].push_back(boost::shared_ptr<FWEveView> (new FWGlimpseView(iParent, t)));
   return finishViewCreate(m_views[t].back());
}

FWEveView*
FWEveViewManager::finishViewCreate(boost::shared_ptr<FWEveView> view)
{
   // printf("new view %s added \n", view->typeName().c_str());
   gEve->DisableRedraw();

   // set geometry and calo data
   view->setContext(context()); 

   // set proxies have a window falg
   int viewerBit = 1 << view->typeId();   
   if (m_views[view->typeId()].size() == 1)
   {
      for ( std::map<int, BuilderVec>::iterator i = m_builders.begin(); i!=  m_builders.end(); ++i)
      {
         int builderViewBit = i->first;
         BuilderVec& bv =  i->second;
         if (viewerBit == (builderViewBit & viewerBit))
         {
            for(BuilderVec_it bIt = bv.begin(); bIt != bv.end(); ++bIt)
            {
               (*bIt)->setHaveWindow(true);
            }
         }
      }
   }
    
   FWRPZView* rpzView = dynamic_cast<FWRPZView*>(view.get());
   for ( std::map<int, BuilderVec>::iterator i = m_builders.begin(); i!=  m_builders.end(); ++i)
   {
      int builderViewBit = i->first;
      BuilderVec& bv =  i->second;
      if (viewerBit == (builderViewBit & viewerBit))
      { 
         for(BuilderVec_it bIt = bv.begin(); bIt != bv.end(); ++bIt)
         {
            // it is ok to call create even for shared productsm since
            // builder map key garanties that
            TEveElementList* product = (*bIt)->createProduct(view->typeId(), view->viewContext());

            if ((*bIt)->havePerViewProduct((FWViewType::EType)view->typeId()))
            {
               // view owned
               (*bIt)->build();
               if (rpzView)
               {
                  rpzView->importElements(product, (*bIt)->item()->layer(), rpzView->ownedProducts());
               }
               else
               {
                  view->ownedProducts()->AddElement(product);
               }
            }
            else
            {
               // shared
               if (rpzView)
               {
                  rpzView->importElements(product, (*bIt)->item()->layer(), rpzView->eventScene());
               }
               else
               {
                  view->eventScene()->AddElement(product);
               }
                 
            }
         }
      }
   }

   view->beingDestroyed_.connect(boost::bind(&FWEveViewManager::beingDestroyed,this,_1));

  

   gEve->EnableRedraw();
   view->viewerGL()->UpdateScene();
   gEve->Redraw3D();   

   return view.get();
}

void
FWEveViewManager::beingDestroyed(const FWViewBase* vb)
{
   FWEveView* view = (FWEveView*) vb;
   int typeId = view->typeId();
   for(EveViewVec_it i= m_views[typeId].begin(); i != m_views[typeId].end(); ++i) {
      if(i->get() == vb) {
         m_views[typeId].erase(i);
         return;
      }
   }

   if (m_views[typeId].empty())
   {
      int viewerBit = 1 << typeId;
      for ( std::map<int, BuilderVec>::iterator i = m_builders.begin(); i!= m_builders.end(); ++i)
      {
         int builderBit = i->first;
         if (viewerBit == (builderBit & viewerBit)) // check only in case if connected
         {
            BuilderVec& bv =  i->second;

            // remove view-owned product
            if (viewerBit == (builderBit & viewerBit))
            {
               for(BuilderVec_it bIt = bv.begin(); bIt != bv.end(); ++bIt)
                  (*bIt)->removePerViewProduct(view->typeId(), view->viewContext());
            }

            // and setup proxy builders have-a-window flag
            if (!haveViewForBit(builderBit))
            {
               if (viewerBit == (builderBit & viewerBit))
               {
                  for(BuilderVec_it bIt = bv.begin(); bIt != bv.end(); ++bIt)
                     (*bIt)->setHaveWindow(false);
               }
            }
         }
      }
   }
}

//______________________________________________________________________________

void
FWEveViewManager::modelChangesComing()
{
   gEve->DisableRedraw();
}

void
FWEveViewManager::modelChangesDone()
{
   gEve->EnableRedraw();
}


void
FWEveViewManager::modelChanges(const FWModelIds& iIds)
{
   FWModelId id = *(iIds.begin());
   const FWEventItem* item = id.item();

   // in standard case new elements can be build in case of change of visibility
   // and in non-standard case (e.g. calo towers) PB's modelChages handles all changes
   bool itemHaveWindow = false;
   for ( std::map<int, BuilderVec>::iterator i = m_builders.begin(); i!=  m_builders.end(); ++i)
   {
      for(BuilderVec_it bIt = i->second.begin(); bIt != i->second.end(); ++bIt)
      {
         if ((*bIt)->getHaveWindow() && (*bIt)->item() == item)
         {
            (*bIt)->modelChanges(iIds);
            if ((*bIt)->getHaveWindow()) itemHaveWindow = true;
         }
      }
   }

   if (itemHaveWindow)
   {
      EveSelectionSentry();

      std::map<const FWEventItem*, FWInteractionList*>::iterator it =  m_interactionLists.find(item);
      if (it != m_interactionLists.end())
      {
         it->second->modelChanges(iIds);
      }
   }
}

void
FWEveViewManager::itemChanged(const FWEventItem* item)
{ 
   if (item)
   {
      bool itemHaveWindow = false;
      for ( std::map<int, BuilderVec>::iterator i = m_builders.begin(); i!=  m_builders.end(); ++i)
      {
         for(BuilderVec_it bIt = i->second.begin(); bIt != i->second.end(); ++bIt)
         {
            if ((*bIt)->item() == item)
            {
          
               (*bIt)->itemChanged(item);
               if ((*bIt)->getHaveWindow()) itemHaveWindow = true;
            }
         }
      }

      if (itemHaveWindow)
      {
         std::map<const FWEventItem*, FWInteractionList*>::iterator it =  m_interactionLists.find(item);
         if (it != m_interactionLists.end())
         {
            it->second->itemChanged();
         }
      }
   }
}

void
FWEveViewManager::removeItem(const FWEventItem* item)
{
   EveSelectionSentry();

   for ( std::map<int, BuilderVec>::iterator i = m_builders.begin(); i!=  m_builders.end(); ++i)
   {
      BuilderVec_it bIt = i->second.begin();
      while( bIt != i->second.end() )
      {
         if ((*bIt)->item() == item)
         { 
            // TODO caching of proxy builders
            (*bIt)->itemBeingDestroyed(item);
            bIt = i->second.erase(bIt);
         }
         else
         {
            ++bIt;
         }
      }
   }

   std::map<const FWEventItem*, FWInteractionList*>::iterator it =  m_interactionLists.find(item);
   if (it != m_interactionLists.end())
   {
      delete it->second;
      m_interactionLists.erase(it);
   }
}

void
FWEveViewManager::colorsChanged()
{
   for (int t = 0 ; t < FWViewType::kSize; ++t)
   {
      for(EveViewVec_it i = m_views[t].begin(); i != m_views[t].end(); ++i) 
         (*i)->setBackgroundColor(colorManager().background());
   }
}

//______________________________________________________________________________

void
FWEveViewManager::eventBegin()
{
   gEve->DisableRedraw();
}

void
FWEveViewManager::eventEnd()
{
   for (int t = 0 ; t < FWViewType::kSize; ++t)
   {
      for(EveViewVec_it i = m_views[t].begin(); i != m_views[t].end(); ++i)   
         (*i)->eventEnd();
   }
   gEve->EnableRedraw();
}

//______________________________________________________________________________

void
FWEveViewManager::selectionAdded(TEveElement* iElement)
{
   // std::cout <<"FWEveViewManager selection added "<<iElement<< std::endl;

   if(0!=iElement) {
      //std::cout <<"  non null"<<std::endl;
      void* userData=iElement->GetUserData();
      //std::cout <<"  user data "<<userData<<std::endl;
      if(0 != userData) {
         //std::cout <<"    have userData"<<std::endl;
         //std::cout <<"      calo"<<std::endl;
         EveSelectionSentry();
         FWFromEveSelectorBase* base = reinterpret_cast<FWFromEveSelectorBase*> (userData);
         base->doSelect();
      }
   }
}

void
FWEveViewManager::selectionRemoved(TEveElement* iElement)
{
   //  std::cout <<"FWEveViewManager selection removed"<<std::endl;

   if(0!=iElement) {
      void* userData=iElement->GetUserData();
      if(0 != userData) {
         EveSelectionSentry();
         FWFromEveSelectorBase* base = static_cast<FWFromEveSelectorBase*>(userData);
         //std::cout <<"   removing"<<std::endl;
         base->doUnselect();
      }
   }
}

void
FWEveViewManager::selectionCleared()
{
   context().selectionManager()->clearSelection();
}


//
// const member functions
//

FWTypeToRepresentations
FWEveViewManager::supportedTypesAndRepresentations() const
{
   // needed for add collection GUI
   FWTypeToRepresentations returnValue;
   const std::string kSimple("simple#");
   for(TypeToBuilder::const_iterator it = m_typeToBuilder.begin(), itEnd = m_typeToBuilder.end();
       it != itEnd;
       ++it) {

      std::vector<BuilderInfo> blist = it->second;
      for ( std::vector<BuilderInfo>::iterator bIt= blist.begin(); bIt != blist.end(); ++bIt)
      {
         std::string name = (*bIt).m_name;
     
         if(name.substr(0,kSimple.size()) == kSimple)
         {
            name = name.substr(kSimple.size(), name.find_first_of('@')-kSimple.size());
            returnValue.add(boost::shared_ptr<FWRepresentationCheckerBase>( new FWSimpleRepresentationChecker(name, it->first)) );
         }
         else
         {
            name = name.substr(0, name.find_first_of('@'));
            returnValue.add(boost::shared_ptr<FWRepresentationCheckerBase>( new FWEDProductRepresentationChecker(name, it->first)) );
         }
      }
   }
   return returnValue;
}


bool
FWEveViewManager::haveViewForBit(int bit) const
{
   bool haveView = false;
   
   for (int t = 0; t < FWViewType::kSize; ++t)
   {
      int testBit = 1 << t;
      if (testBit == (bit & testBit))
      {
         if( m_views[t].size())
         {
            haveView = true;
            break;
         }
      }
   }
   
   // printf("have %d view for bit %d \n", haveView, bit);
   return haveView;
}

