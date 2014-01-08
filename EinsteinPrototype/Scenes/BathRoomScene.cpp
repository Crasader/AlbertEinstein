
//
//  BathRoomScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 21/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "BathRoomScene.h"
#include "HomeScene.h"
BathRoomScene::BathRoomScene()
{
    state = TARGET_CATEGORY;
    actualCategory = BANHEIROS;
    initFixedMenu();
    Init();
    LoadListView();
    addButtonTop();
}

void BathRoomScene::setUpMenu()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
  
    
    if (size.height > 480) {
        size.height= 280;
    }
    else{
        size.height= 260;
    }

    
    
    size.width = 0.90*size.width;
    
    initListView(size, this);
    pList->setPosition(ccp(20,141));
}

void BathRoomScene::Init()
{
    setUpTopImage(Global::TOP_SRC_IMAGE_Banheiro);
}

void BathRoomScene::LoadListView()
{
    ///444 CategoriaID para Banheiros
    BuildCategoryState(444);
}

void BathRoomScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void BathRoomScene::BuildCategoryState(int serviceID)
{
    if( serviceID == 444)
        ChangeTitle(BANHEIROS);
    BuildCategoryWayPointState(serviceID);
    setUpMenu();
}

//Retorna para a tela inicial Home
void BathRoomScene::OnBack()
{
    returnToHome();
}

void BathRoomScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    setDefaultListView(listView, data, 0);
}

void BathRoomScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    //Redireciona para a rota
    StateMachine machine;
    machine.actual_category = actualCategory;
    machine.lastState = state;
    changeScene(new TraceRouteScene(0,machine) );
}

void BathRoomScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    
}

 
