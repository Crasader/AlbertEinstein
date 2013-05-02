//
//  MinhasRotasScenes.h
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 14/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__MinhasRotasScenes__
#define __EinsteinPrototype__MinhasRotasScenes__

#include <iostream>
#include "Enumerations.h"
#include "RotasFileIO.h"

#include "HomeScene.h"


class MinhasRotasScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate
    {
    
    private :
        
        std::vector<RotaRow> lstRows;
        StateMachine state;
        
    public:
        
        MinhasRotasScene();
        void setUpMenu();
        void LoadListView();
        virtual void OnBack();
        virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
        virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
        virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
        virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);

    };
#endif /* defined(__EinsteinPrototype__MinhasRotasScenes__) */
