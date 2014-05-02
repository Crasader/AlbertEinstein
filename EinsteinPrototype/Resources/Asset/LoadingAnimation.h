//
//  LogoAnimation.h
//  EinsteinPrototype
//
//  Created by Camargo on 01/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_LoadingAnimation_h
#define EinsteinPrototype_LoadingAnimation_h

#include "cocos2d.h"

class LoadingAnimation : public cocos2d::CCNode
{
public:
    cocos2d::CCSprite* loadingFrame;
    cocos2d::CCAction* action;
    cocos2d::CCArray* theframes;
    
    cocos2d::CCAnimation CCAnimationRef;
    LoadingAnimation();
    void runAnimation();
    void stopAnimation();
private:
  
};


#endif
