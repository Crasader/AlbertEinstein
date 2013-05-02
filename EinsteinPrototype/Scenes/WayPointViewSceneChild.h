//
//  WayPointViewSceneChild.h
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 02/04/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__WayPointViewSceneChild__
#define __EinsteinPrototype__WayPointViewSceneChild__

#include <iostream>

#include "Enumerations.h"
#include "TraceRouteScene.h"
#include "AlertViewScene.h"
#include "LayerWebView.h"
#include "HowToGoScene.h"
#include "CCApplication.h"

class WayPointViewChildScene : public IFixedMenu
{
    
private:
    
    StateMachine stateMachinePreviousPage;
    int wayPoint;
	bool *infoOpen;
    
public:
    
    static void createLayerChild(int wayPoint, bool *infoOpenRef);
 
    WayPointViewChildScene( int wayPoint, bool *infoOpenRef);
 
    void LoadWayPoint();
    void InitView();
 
    virtual void OnBack();
 
 
    
};
#endif /* defined(__EinsteinPrototype__WayPointViewSceneChild__) */
