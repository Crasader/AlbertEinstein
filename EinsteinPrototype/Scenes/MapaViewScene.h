//
//  MapaViewScene.h
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 08/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__MapaViewScene__
#define __EinsteinPrototype__MapaViewScene__

#include <iostream>
#include "Enumerations.h"
#include "HomeScene.h"
#include "RotasFileIO.h"
#import "CCFinishDelegate.h"

class MapViewScene: public IFixedMenu , public CCFinishJobDelegate
{
    
private:
    
    bool isFavoritoChecked = false;
    bool isRouteChecked = false;
    const char * destination;
    
    StateMachine statePrevious;
    EstacionamentoState type;

public:

    void releaseData();
    MapViewScene(int _wayPoint,int _targetWayPoint);
    
    void setUpNavigationButtons();
    void ccFinishJob();
    void Initialize();
    void setUpButtonTop();
    void setUp();
    void btnMoveFoward(CCObject *sender);
    void btnMoveBack(CCObject *sender);
    void ccTouchesBegan(CCSet *touches, CCEvent *event);
    void ccTouchesMoved(CCSet *touches, CCEvent *event);
    void ccTouchesEnded(CCSet *touches, CCEvent *event);
    
    
    virtual void OnBack();
    virtual void draw();
    void CreateButtons();
    void btnTopButton(CCObject *sender);
	CCPoint touchLocation;
	bool touched;
	bool infoOpen;
    
};
#endif /* defined(__EinsteinPrototype__MapaViewScene__) */
