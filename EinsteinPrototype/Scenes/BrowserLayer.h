//
//  BrowserLayer.h
//  EinsteinPrototype
//
//  Created by Camargo on 12/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "CCCommon.h"
#include "cocos2d.h"
#include "AnimatedSprite.h"
#include "Enumerations.h"


class BrowserLayer : public cocos2d::CCLayer
{
    
private:
    
public:
    AnimatedSprite *loading;
    BrowserLayer();
    ~BrowserLayer();
    void openBrowserWithUrl(char* turl);
    void setUpMenu();
  //  void onBack();
    
 
};
