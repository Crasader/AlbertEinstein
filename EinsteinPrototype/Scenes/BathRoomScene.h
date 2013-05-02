//
//  BathRoomScene.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 21/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__BathRoomScene__
#define __EinsteinPrototype__BathRoomScene__

#include <iostream>
#include <iostream>
#include "cocos2d.h"
#include "WayPointViewScene.h"
#include "ParkingScene.h"
#include "Pathfinder.h"
#include "Enumerations.h"
#include "extensions/CCScrollView/CCScrollView.h"
#include "extensions/CCListView/CCListView.h"
#include "TraceRouteScene.h"
class BathRoomScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate
{
private:
 
    MenuListState state;
public:
    BathRoomScene();
    void Init();
    void setUpMenu();
    void LoadListView();
 
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void OnBack();
    void BuildCategoryState(int serviceID);
    
};
#endif /* defined(__EinsteinPrototype__BathRoomScene__) */
