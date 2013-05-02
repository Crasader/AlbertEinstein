//
//  ListViewScene.h
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__ListViewScene__
#define __EinsteinPrototype__ListViewScene__

#include <iostream>
#include "cocos2d.h"
#include "WayPointViewScene.h"
#include "ParkingScene.h"
#include "Pathfinder.h"
#include "Enumerations.h"
 

class HowToGoScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate {
    
    
private:
 
    MenuListState state;
    StateMachine machine;
    int lastIdInserted =0;
 
public:

    HowToGoScene(HomeMenuState category);
    HowToGoScene(StateMachine state);
    void initListView(HomeMenuState category);
    void setUpMenu();
    void setUpImageTop();
 

 
    virtual void BuildCategoryState(); 
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void OnBack();

    void btnResponse(CCObject *sender);
    
   
    
     private:
    CCSize size ;
    
    
    ~HowToGoScene();
 };
#endif /* defined(__EinsteinPrototype__ListViewScene__) */
