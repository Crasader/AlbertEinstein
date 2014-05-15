//
//  BrowserScene.h
//  EinsteinPrototype
//
//  Created by Camargo on 12/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_BrowserScene_h
#define EinsteinPrototype_BrowserScene_h

#include <iostream>
#include "enumerations.h"
#include "CCTextFieldTTF.h"
#include "TraceRouteScene.h"
#include "CCHttpRequest.h"
#include "AnimatedSprite.h"
#include "LayerWebView.h"


#include "gason.h"


using namespace cocos2d::extension;

class BrowserScene : cocos2d::CCLayer {
    
private:
    void createScreen();
    void btnBack(CCObject* node);

    
public:

    AnimatedSprite *loading;
    
    BrowserScene();

};


#endif
