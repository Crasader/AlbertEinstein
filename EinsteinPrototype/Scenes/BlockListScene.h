//
//  BlockListScene.h
//  EinsteinPrototype
//
//  Created by Lucas Bittencourt on 7/4/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

//
//  BathRoomScene.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 21/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__BlockListScene__
#define __EinsteinPrototype__BlockListScene__

#include <iostream>
#include "cocos2d.h"
#include "WayPointViewScene.h"
#include "ParkingScene.h"
#include "Pathfinder.h"
#include "Enumerations.h"
#include "extensions/CCScrollView/CCScrollView.h"
#include "extensions/CCListView/CCListView.h"
#include "TraceRouteScene.h"

class BlockListScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate
{
private:
 
    MenuListState state;
	bool* active;
	int blockID;
	bool secondScreen = false;
public:
	static void createLayerChild(int wayPoint, bool *infoOpenRef, bool* active);
    BlockListScene(int blockID, bool* active);
    void Init();
    void setUpMenu();
    void LoadListView();
 
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void OnBack();
    void BuildCategoryState(int serviceID);
	
	~BlockListScene();
    
};
#endif /* defined(__EinsteinPrototype__BathRoomScene__) */


/*#ifndef __EinsteinPrototype__BlockListScene__
#define __EinsteinPrototype__BlockListScene__

#include "Enumerations.h"

class BlockListScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate
{
    
private:
    
    StateMachine stateMachinePreviousPage;
    int wayPoint;
	bool *infoOpen;
    
public:
    
    static void createLayerChild(int wayPoint, bool *infoOpenRef);
 
    BlockListScene( int wayPoint, bool *infoOpenRef);
 
    void LoadWayPoint();
    void InitView();
 
    virtual void OnBack();
	
	virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data){};
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data){};
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data){};
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data){};
 
 
    
};


#endif /* defined(__EinsteinPrototype__BlockListScene__) */
