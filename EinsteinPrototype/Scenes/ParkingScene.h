//
//  ParkingScene.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 18/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__ParkingScene__
#define __EinsteinPrototype__ParkingScene__


#include <iostream>
#include "cocos2d.h"
#include "Enumerations.h"
#include "AlertViewScene.h"
#include "ParkingViewScene.h"

class ParkingScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate 
{
    EstacionamentoState state;

public:
    ParkingScene();
    ParkingScene(EstacionamentoState state);

    void Start();
    void ShowMenu();
    void setUpMenu();
    void addButtonTop();
    void OnBack();
    void LoadListView();
    void insertItemListView( int tagValue, const char* datatext);
    void btnResponseEntering(CCObject *sender);
    void btnResponseOuting(CCObject *sender);
    void btnResponseQuiting(CCObject *sender);
    void btnEntradaSaida(CCObject *sender);
       virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
};

#endif /* defined(__EinsteinPrototype__ParkingScene__) */
