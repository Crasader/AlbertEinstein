//
//  MapMenu.h
//  EinsteinPrototype
//
//  Created by Camargo on 06/01/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_MapMenu_h
#define EinsteinPrototype_MapMenu_h

#endif



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

class MapMenu : public cocos2d::CCLayer{
    MapType type;
    
public:
    MapMenu();
    void addMenu();
    void InitHome();
    void Remove();
    void openMap(CCObject* pSender);
    void open3D(CCObject* pSender);
    ~MapMenu();
};
