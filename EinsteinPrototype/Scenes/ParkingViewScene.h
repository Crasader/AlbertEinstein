//
//  ParkingViewScene.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 19/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__ParkingViewScene__
#define __EinsteinPrototype__ParkingViewScene__

#include <iostream>

#include "Enumerations.h"
#include "TraceRouteScene.h"
#include "ParkingScene.h"

class ParkingViewScene : public IFixedMenu
{
private:
    int wayPointID;
    bool isChangeToRoute ;
public:
    EstacionamentoState state;
    ParkingViewScene(EstacionamentoState state,int wayPointID );
    void OnBack();
    void LoadParkingId();
    void btnRotaCallBack(CCObject *sender);
    void btnResponse(CCObject *sender);
    void btnCloseBack(CCObject *sender);
    void draw();

};
#endif /* defined(__EinsteinPrototype__ParkingViewScene__) */
