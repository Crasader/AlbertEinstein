//
//  ParkingScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 18/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "ParkingScene.h"
#include "HomeScene.h"
ParkingScene * me;
ParkingScene::ParkingScene()
{
    
    me = this;
    ShowMenu();
    
   

}




void ParkingScene::OnBack()
{
    returnToHome();
}
 

ParkingScene::ParkingScene(EstacionamentoState state)
{
    
  
    me = this;
    me->state = state;
   
    ShowMenu();
    
    
    
}
 

void ParkingScene::setUpMenu()
{

}
CCLayerColor *mainNode ;

void ParkingScene::ShowMenu()
{
    mainNode = CCLayerColor::create(ccc4(190,110,110,100));
    mainNode->setAnchorPoint(ccp(0, 0));
    mainNode->setPosition(ccp(0,0));
    me->addChild(mainNode,299, 250);
    
    CCLayerColor* c = CCLayerColor::create(ccc4(255,255,255,255));
    CCSprite* TitleImg = CCSprite::create(Global::TOP_SRC_IMAGE_Estacionamentos);
    TitleImg->setAnchorPoint(ccp(0, 0));
    TitleImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height - TitleImg->boundingBox().size.height-20));
    
    CCSprite* statusImg = CCSprite::create("bg_status.png");
    statusImg->setAnchorPoint(ccp(0, 0));
    statusImg->setScale(2);
    statusImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height -20));
    
    
    mainNode->addChild(c,11,3);
    mainNode->addChild(statusImg,12,3);
    
    mainNode->addChild(TitleImg,13,2);
    CCSize size =  CCDirector::sharedDirector()->getWinSize();
    
    
    CCLabelTTF *labelTitle;
    labelTitle = CCLabelTTF::create("Estacionamento", "LucidaGrandeBold", 15);
    labelTitle->setPosition(ccp(100,size.height-43));
    labelTitle->setAnchorPoint(ccp(0,0.5));
    labelTitle->setColor(ccc3(255, 255, 255));
    mainNode->addChild(labelTitle,14);

    //todo initFixedMenu(INICIO, mainNode);
    
    
     AlertViewScene::showMessageParkingOption((CCNode*)mainNode,  menu_selector(ParkingScene::btnResponseEntering), menu_selector(ParkingScene::btnResponseOuting), menu_selector(ParkingScene::btnResponseQuiting));

    
}

void ParkingScene::Start()
{
    
   

}

void ParkingScene::btnResponseEntering(CCObject *sender)
{

   mainNode->removeChildByTag(Global::TAG_Alert_Layer, true);
    AlertViewScene::isActive = false;
     state = EST_ENTRADA;
    me->addButtonTop();
    me->LoadListView();
}

void ParkingScene::btnResponseOuting(CCObject *sender)
{
 
    mainNode->removeChildByTag(Global::TAG_Alert_Layer, true);
    AlertViewScene::isActive = false;
    state = EST_SAIDA;
    me->addButtonTop();
    me->LoadListView();

}
void ParkingScene::addButtonTop()
{
    
    CCLayerColor* c = CCLayerColor::create(ccc4(255,255,255,255));
    CCSprite* TitleImg = CCSprite::create(Global::TOP_SRC_IMAGE_Estacionamentos);
    TitleImg->setAnchorPoint(ccp(0, 0));
    TitleImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height - TitleImg->boundingBox().size.height-20));
    
    CCSprite* statusImg = CCSprite::create("bg_status.png");
    statusImg->setAnchorPoint(ccp(0, 0));
    statusImg->setScale(2);
    statusImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height -20));
    
    
    mainNode->addChild(c,11,3);
    mainNode->addChild(statusImg,12,3);
    
    mainNode->addChild(TitleImg,13,2);
    CCSize size =  CCDirector::sharedDirector()->getWinSize();
    size.height =size.height - TitleImg->boundingBox().size.height;
    
 
    
    CCLabelTTF *labelTitle;
    labelTitle = CCLabelTTF::create("Estacionamento", "LucidaGrandeBold", 15);
     labelTitle->setPosition(ccp(100,size.height-43));
    labelTitle->setAnchorPoint(ccp(0,0.5));
    labelTitle->setColor(ccc3(255, 255, 255));
    mainNode->addChild(labelTitle,14);
  
    
    CCMenu* menu = CCMenu::create();
    createMenuItem(menu, 123, "Seta1.png", "Seta2.png",0, 0, menu_selector(ParkingScene::btnResponseQuiting), me );
    menu->setPosition(ccp(10,size.height+10));
    menu->setAnchorPoint(ccp(0,0));
    mainNode->addChild(menu,15);
   // mainNode->addChild(c,2,1000);
}



void ParkingScene::btnResponseQuiting(CCObject *sender)
{
    mainNode->removeFromParentAndCleanup(true);
     AlertViewScene::isActive = false;

    CCScene* newScene = CCScene::create();
    newScene->addChild(new HomeScene(),0,Global::TAG_Parent_Layer);
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Global::TAG_Parent_Layer, true);
    CCDirector::sharedDirector()->replaceScene(newScene);

}


void ParkingScene::insertItemListView( int tagValue, const char* datatext)
{
    PairKey data ;
    data.key = tagValue;
    data.text = strdup(datatext);
    data.hasChild =  false;
    data.keyParent = 0;
    
    me->plistMenuCurrent.push_back(data);
}

void ParkingScene::LoadListView()
{
//    plistMenuCurrent.clear();
//    plistTagMenuCurrent.clear();
    plistMenuCurrent.clear();
//    
    
    char* namesprite = NULL;
    CCPoint position;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    CCSize theSize = winsize;
    position = ccp(25, winsize.height-150 );
    
    if(state == EST_ENTRADA )
    {
        
        
        
        namesprite = "VizualizePontosEntrada.png";
        me->insertItemListView( 19 ,"Bloco A1");
        me->insertItemListView( 51 ,"Bloco B");
        me->insertItemListView( 89 ,"Bloco D/E");

    }
    else
    {
        
        namesprite = "SelecioneEntrada_Estacionamento.png";
        me->insertItemListView( 19 ,"Bloco A");
        me->insertItemListView( 51 ,"Bloco A1");
        me->insertItemListView( 89 ,"Bloco D/E");
    }
    
     theSize.width = 0.90*winsize.width;
    theSize.height = winsize.height -70;
     pList = cocos2d::extension::CCListView::create(cocos2d::extension::CCListViewModeVertical);
    pList->setOpacity(0);
    
    pList->setContentSize(theSize);
    pList->setDelegate(me);
    pList->setSeparatorStyle(cocos2d::extension::CCListViewCellSeparatorStyleSingleLine);
    
    pList->setAnchorPoint(ccp(0,0));
    pList->setPosition(ccp(20,0));
    mainNode->addChild(pList,50,600);

}

void ParkingScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = me->plistMenuCurrent.size();
}

void ParkingScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    me->setDefaultListView(listView, data,0);
}

void ParkingScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    changeScene(new ParkingViewScene(state, plistTagMenuCurrent[data->nRow]));
}

void ParkingScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{

}

void ParkingScene::btnEntradaSaida(CCObject *sender)
{
   // if(state != (EstacionamentoState)((CCNode*)sender)->getTag())
   // {
//        if( state == EST_ENTRADA)
//            state = EST_SAIDA;
//        else
//            state = EST_ENTRADA;
        
       // plistMenuCurrent.clear();
       // plistTagMenuCurrent.clear();
       // state =  (EstacionamentoState)((CCNode*)sender)->getTag();
       // LoadListView();
  //  }
    
   //    LoadListView();
    
  //  this->addButtonTop();
}