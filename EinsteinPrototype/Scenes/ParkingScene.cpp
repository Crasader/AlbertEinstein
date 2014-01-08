//
//  ParkingScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 18/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "ParkingScene.h"
#include "HomeScene.h"
ParkingScene::ParkingScene()
{
    this->state =  EST_ENTRADA;
    initFixedMenu();
    Init();
    LoadListView();
    
    this->addButtonTop();
}




void ParkingScene::OnBack()
{
    returnToHome();
}
 

ParkingScene::ParkingScene(EstacionamentoState state)
{
    this->state = state;
    initFixedMenu();
    Init();
    LoadListView();
    
    this->addButtonTop();
    
}
 

void ParkingScene::setUpMenu()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (size.height > 480) {
         size.height= 280;
    }
    else
    {
         size.height= 260;
    }

   
    size.width = 0.90*size.width;
    
    IFixedMenu::initListView(size, this);
    pList->setPosition(ccp(20,55));
}

void ParkingScene::Init()
{
    setUpTopImage(Global::TOP_SRC_IMAGE_Estacionamentos);
    ChangeTitle(ESTACIOMENTOS);
    
    CCSprite* spriteBackButton = CCSprite::create("Background_Estacionamento_botoes.png");
    spriteBackButton->setAnchorPoint(ccp(0, 0));
	spriteBackButton->setOpacity(2000);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();

    if (winsize.height > 480) {
        spriteBackButton->setPosition(ccp(0, 430));
    }
    else
    {
        spriteBackButton->setPosition(ccp(0, 350));
    }
    
    this->addChild(spriteBackButton);
    
    CreateButtons();
}

void ParkingScene::CreateButtons()
{
    if(this->getChildByTag(11)!=NULL)
        this->removeChildByTag(11, true);
 
    CCMenu* menuEntradaSaida = CCMenu::create();
    if(state == EST_ENTRADA )
    {
      createMenuItem(menuEntradaSaida, EST_SAIDA, "Saida_btn.png", "Saida_press.png", 0,0, menu_selector( ParkingScene::btnEntradaSaida),this);
      createMenuItem(menuEntradaSaida, EST_ENTRADA, "Entrada_Press.png", "Entrada_Press.png", 83,0, menu_selector( ParkingScene::btnEntradaSaida),this);
    }
    else
    {
        createMenuItem(menuEntradaSaida, EST_SAIDA, "Saida_press.png", "Saida_press.png", 0,0, menu_selector( ParkingScene::btnEntradaSaida),this);
        createMenuItem(menuEntradaSaida, EST_ENTRADA, "Entrada_btn.png", "Entrada_Press.png", 83,0, menu_selector( ParkingScene::btnEntradaSaida),this);
    }

    if(this->getChildByTag(12)!=NULL)
        removeChildByTag(12, true);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    if (winsize.height > 480) {
        menuEntradaSaida->setPosition(ccp(73,445));
    }
    else
    {
        menuEntradaSaida->setPosition(ccp(73,365));
    }
    
    this->addChild(menuEntradaSaida,0,2);
}

void ParkingScene::LoadListView()
{
    plistMenuCurrent.clear();
    plistTagMenuCurrent.clear();
    char* namesprite = NULL;
      CCPoint position;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    if (winsize.height > 480) {
        position = ccp(25, 411 );
    }
    else
    {
        position = ccp(25, 314 );
    }

  
    if(state == EST_ENTRADA )
    {
        if (winsize.height > 480) {
            position = ccp(30, 415);
        }
        else
        {
            position = ccp(30, 318);
        }
        
        
        namesprite = "VizualizePontosEntrada.png";
        insertItemListView( 19 ,"Bloco A1");
        insertItemListView( 51 ,"Bloco B");
        insertItemListView( 89 ,"Bloco D/E");
    }
    else
    {
        
        namesprite = "SelecioneEntrada_Estacionamento.png";
        insertItemListView( 19 ,"Bloco A");
        insertItemListView( 51 ,"Bloco A1");
        insertItemListView( 89 ,"Bloco D/E");
    }
    CCSprite* spriteSelecione = CCSprite::create(namesprite);
    spriteSelecione->setAnchorPoint(ccp(0, 0));
    spriteSelecione->setPosition(position);
    spriteSelecione->setScale(1);
    this->addChild(spriteSelecione,0,12);
    setUpMenu();
}

void ParkingScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void ParkingScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    this->setDefaultListView(listView, data,0);
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
    if(state != (EstacionamentoState)((CCNode*)sender)->getTag())
    {
        if( state == EST_ENTRADA)
            state = EST_SAIDA;
        else
            state = EST_ENTRADA;
        
        plistMenuCurrent.clear();
        plistTagMenuCurrent.clear();
        state =  (EstacionamentoState)((CCNode*)sender)->getTag();
        CreateButtons();
        LoadListView();
    }
}