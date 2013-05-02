//
//  WayPointViewScene.h
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/8/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__WayPointViewScene__
#define __EinsteinPrototype__WayPointViewScene__

#include <iostream>

#include "Enumerations.h"
#include "TraceRouteScene.h"
#include "AlertViewScene.h"
#include "LayerWebView.h"
#include "HowToGoScene.h"
#include "CCApplication.h"

class WayPointViewScene : public IFixedMenu
{
    
private:
 
    StateMachine stateMachinePreviousPage;
    int wayPoint;
    int  targetCategoriaiD;
    bool enableCheckin;
    bool isChangeToRoute;
    bool is4SquareLoginCalled;
    
    
    
public:

    WayPointViewScene(HomeMenuState category, int wayPoint, StateMachine stateMachinePreviousPage);
    WayPointViewScene( int wayPoint, StateMachine stateMachinePreviousPage);
    
    void LoadMenu();
    void LoadWayPoint();
    void InitView();
    void ReloadWayPointView();
    void btnRotaCallBack(CCObject *sender);
    void btnHorarioCallBack(CCObject *sender);
    void btnResponseRota(CCObject *sender);
    void btnCheckin(CCObject *sender);
    virtual void OnBack();
	void draw();
    
    ~WayPointViewScene();
    
};

#endif /* defined(__EinsteinPrototype__WayPointViewScene__) */
