//
//  HomeScene.h
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/6/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__HomeScene__
#define __EinsteinPrototype__HomeScene__

#include <iostream>
#include "cocos2d.h"
 
#include "Pathfinder.h"
#include "Enumerations.h"
#include "HowToGoScene.h"
#include "ParkingScene.h"
#include "SearchScene.h"
#include "BathRoomScene.h"
#include "TraceRouteScene.h"
#include "MinhasRotasScenes.h"
#include "LayerWebView.h"
 
class HomeScene : public IFixedMenu{
public:
    HomeScene();
    void InitHome();
    ~HomeScene();
};



#endif /* defined(__EinsteinPrototype__HomeScene__) */
