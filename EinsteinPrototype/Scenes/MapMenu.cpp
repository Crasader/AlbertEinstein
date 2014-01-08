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

void MapMenu::addMenu()
{
    CCMenuItem *pOpenMapItem = CCMenuItemImage::create("button_comochegar.png","button_comochegar_on.png",this,menu_selector(MapMenu::openMap));
    pOpenMapItem->setAnchorPoint(ccp(0, 0));
	pOpenMapItem->setPosition(ccp(40, 200));
    CCMenuItem *pOpen3DItem = CCMenuItemImage::create("button_3dView.png","button_3dView_on.png",this,menu_selector(MapMenu::open3D));
    pOpen3DItem->setAnchorPoint(ccp(0, 0));
	pOpen3DItem->setPosition(ccp(40, 300));
    
    //  pCloseItem->setFontSize(35);
    // pCloseItem->setFontName("Helvetica");
    menu = CCMenu::create(pOpenMapItem,pOpen3DItem,NULL);
    menu->setAnchorPoint(ccp(0, 0));
	menu->setPosition(ccp(0, 0));
	layer->addChild(menu);
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
}

void MapMenu::openMap(CCObject* pSender)
{
  // CCDirector::sharedDirector()->end();
   // exit(0);
    
    menu->removeFromParentAndCleanup(true);
    this->addChild(new LayerWebView());

}

void MapMenu::open3D(CCObject* pSender)
{
    menu->removeFromParentAndCleanup(true);
    layer->addChild(Building3DLayer::create(),0,Global::TAG_Child_Layer);
}


/*******************************************************************************************************************************/
/* IFixedMenu
 Menu com nenhum item selecionado ( padrao)
 */

