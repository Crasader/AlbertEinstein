//
//  MinhasRotasScenes.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 14/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "MinhasRotasScenes.h"
#include "MapaViewScene.h"

MinhasRotasScene::MinhasRotasScene()
{
    setUpTopImage(Global::TOP_SRC_IMAGE_Rotas);
    initFixedMenu(MINHAS_ROTAS);
    LoadListView();
    addButtonTop(MINHAS_ROTAS);
}

void MinhasRotasScene::setUpMenu()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (size.height > 480) {
        size.height= 373;
    }
    else{
        size.height= 285;
    }

    
   // size.height= 0.79*CCDirector::sharedDirector()->getWinSize().height;
    size.width = 0.90*CCDirector::sharedDirector()->getWinSize().width;
    initListView(size, this);
        pList->BackgroundID = 2;
    pList->setPosition(ccp(20,45));
}

void MinhasRotasScene::LoadListView()
{
    RotasFileIO* mRotas = new RotasFileIO();
    lstRows = mRotas->LoadMinhasRotas();
    for(int i = 0; i < lstRows.size(); i++)
        insertItemListView( i ,lstRows[i].description);
    setUpMenu();
}

void MinhasRotasScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void MinhasRotasScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    setDefaultListView(listView,data,0);
}

void MinhasRotasScene::OnBack()
{
    //Retorna para pagina inicial
    returnToHome();
}

void MinhasRotasScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    this->state.lastState = TARGET_FAVORITOS;
    this->state.actual_category = getCategory(lstRows[data->nRow].wayPoint);
    changeScene( new WayPointViewScene( this->state.actual_category, lstRows[data->nRow].wayPoint,this->state) );
}

void MinhasRotasScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
}