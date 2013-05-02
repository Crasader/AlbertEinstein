//
//  TraceRouteScene.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 18/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__TraceRouteScene__
#define __EinsteinPrototype__TraceRouteScene__


#include <iostream>

#include "Enumerations.h"



typedef enum{
    
    
    ROTA_AZ,
    ROTA_POR_ANDAR,
    ROTA_FAVORITOS
} RotaState;


class TraceRouteScene : public IFixedMenu, public cocos2d::extension::CCListViewDelegate
{
    RotaState stateRota;
    char currentText[200];
    int  currentFloor;
    
    char buildingHeader[40];
    char buildingHeaderFloor[40];
    
    MenuListState state;
    EstacionamentoState stateEstacionamento;
    StateMachine statePreviousPage;

  
    
    int insertedBuildingKey;
    int insertedFloorKey;
    
    
    ~TraceRouteScene();
    
public:
	
    void OnBackPreviousPage();

    TraceRouteScene(int wayPointID,StateMachine statePreviousPage );
	
    TraceRouteScene(int waypointid, EstacionamentoState state ,StateMachine statePreviousPage);
	
    
    virtual void OnBack();
    
    void Init();
    void setUpMenu();
    
    void UpdateHeader();
    
    void clearHeader();
    
    
    void LoadListView();
    void CreateButtons(RotaState newState);
    
    void btnEntradaSaida(CCObject *sender);
    //void insertItemListView( int tagValue, const char* datatext);
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    


};
#endif /* defined(__EinsteinPrototype__TraceRouteScene__) */

/*
 
 ROTA_AZ,
 ROTA_POR_ANDAR,
 ROTA_FAVORITOS
 } RotaState;
 
 */
