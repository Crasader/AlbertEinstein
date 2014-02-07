//
//  MapMenu.cpp
//  EinsteinPrototype
//
//  Created by Camargo on 06/01/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "MapMenu.h"
#import "CCEGLView.h"

#include "HowToGoScene.h"
#include "MapaViewScene.h"
#include "HomeScene.h"
#include "LayerWebView.h"
#include "MinhasRotasScenes.h"
#include "GLES.h"
CCLayerColor* layer;
CCMenu* menu;

MapMenu::MapMenu()
{
    
    InitHome();
}
MapMenu::~MapMenu()
{
    
}

//void MapViewScene::CreateButtons()
//{
//    if(this->getChildByTag(11)!=NULL)
//        this->removeChildByTag(11, true);

    //    CCMenu* menuEntradaSaida = CCMenu::create();
    //    if(type == MAP_NORMAL )
    //    {
    //        createMenuItem(menuEntradaSaida, EST_SAIDA, "Saida_btn.png", "Saida_press.png", 0,0, menu_selector( ParkingScene::btnEntradaSaida),this);
    //        createMenuItem(menuEntradaSaida, EST_ENTRADA, "Entrada_Press.png", "Entrada_Press.png", 83,0, menu_selector( ParkingScene::btnEntradaSaida),this);
    //    }
    //    else
    //    {
    //        createMenuItem(menuEntradaSaida, EST_SAIDA, "Saida_press.png", "Saida_press.png", 0,0, menu_selector( ParkingScene::btnEntradaSaida),this);
    //        createMenuItem(menuEntradaSaida, EST_ENTRADA, "Entrada_btn.png", "Entrada_Press.png", 83,0, menu_selector( ParkingScene::btnEntradaSaida),this);
    //    }
    //
    //    if(this->getChildByTag(12)!=NULL)
    //        removeChildByTag(12, true);
    //    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    //    if (winsize.height > 480) {
    //        menuEntradaSaida->setPosition(ccp(73,445));
    //    }
    //    else
    //    {
    //        menuEntradaSaida->setPosition(ccp(73,365));
    //    }
    //    
    //    this->addChild(menuEntradaSaida,0,2);
//}
CCSprite* spriteBackButton;
void MapMenu::addMenu()
{
    if (spriteBackButton != NULL) {
        spriteBackButton->removeFromParentAndCleanup(false);
    }
    spriteBackButton = CCSprite::create("Background_Estacionamento_botoes.png");
    spriteBackButton->setAnchorPoint(ccp(0, 0));
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    if (winsize.height > 480) {
        spriteBackButton->setPosition(ccp(0, 430));
    }
    else
    {
        spriteBackButton->setPosition(ccp(0, 350));
    }
    
    layer->addChild(spriteBackButton, 100) ;
    
    
    
    CCMenuItem *pOpenMapItem;
    CCMenuItem *pOpen3DItem;
    if(type == MAP_NORMAL )
    {
        pOpenMapItem = CCMenuItemImage::create("button_comochegar_pressed.png","button_comochegar_pressed.png",this,menu_selector(MapMenu::openMap));
        pOpen3DItem = CCMenuItemImage::create("button_3d_on.png","button_3d_on.png",this,menu_selector(MapMenu::open3D));
        
    }
    else
    {
        pOpenMapItem = CCMenuItemImage::create("button_comochegar_on.png","button_comochegar_on.png",this,menu_selector(MapMenu::openMap));
        pOpen3DItem = CCMenuItemImage::create("button_3d_pressed.png","button_3d_pressed.png",this,menu_selector(MapMenu::open3D));
        
    }
    
      pOpenMapItem->setAnchorPoint(ccp(0, 0));
	pOpenMapItem->setPosition(ccp(0, 0));
    pOpen3DItem->setAnchorPoint(ccp(0, 0));
	pOpen3DItem->setPosition(ccp(110, 00));
    
    //  pCloseItem->setFontSize(35);
    // pCloseItem->setFontName("Helvetica");
    if (menu != NULL) {
    menu->removeFromParentAndCleanup(false);
    }
    menu = CCMenu::create(pOpenMapItem,pOpen3DItem,NULL);
    menu->setAnchorPoint(ccp(0, 0));
    
    if (winsize.height > 480) {
       menu->setPosition(ccp(50, 450));
    }
    else
    {
        menu->setPosition(ccp(50, 360));
    }

	
	layer->addChild(menu,101);
}

void MapMenu::InitHome()
{
    
	CCSprite* bg ;
    
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    char * imgName = "bg_maps.png";
    if (winsize.height > 480) {
        imgName = "bg_maps_4inch.png";
    }
    
    bg = CCSprite::create(imgName);
    bg->setAnchorPoint(ccp(0, 0));
	bg->setPosition(ccp(0, 0));
    
    
    
   layer = CCLayerColor::create(ccc4(255,255,255,255));

	layer->addChild(bg);

    this->addChild(layer);
    
      addMenu();
    this->openMap(NULL);
}
LayerWebView * layerWebView;
void MapMenu::openMap(CCObject* pSender)
{
  // CCDirector::sharedDirector()->end();
   // exit(0);
    
   // menu->removeFromParentAndCleanup(true);
    type = MAP_NORMAL;
    layer->removeChildByTag(Global::TAG_Child_Layer, false);
    layerWebView->forceRelease();
    layerWebView = new LayerWebView();
    //layerWebView->TargetLayer(*layer);
    layer->addChild(layerWebView,0,Global::TAG_Child_Layer);
 addMenu();
}

void MapMenu::open3D(CCObject* pSender)
{
  //  menu->removeFromParentAndCleanup(true);
    type = MAP_3D;

    layer->removeChildByTag(Global::TAG_Child_Layer, false);
    layerWebView->forceRelease();
    CCLayer * layer3d =Building3DLayer::create();
    layer->addChild(layer3d,0,Global::TAG_Child_Layer);
     addMenu();
}
void MapMenu::Remove()
{
    layerWebView->forceRelease();
    layer->removeChildByTag(Global::TAG_Child_Layer, true);
}


/*******************************************************************************************************************************/
/* IFixedMenu
 Menu com nenhum item selecionado ( padrao)
 */

