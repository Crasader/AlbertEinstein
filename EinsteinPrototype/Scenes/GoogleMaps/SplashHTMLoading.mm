//
//  SplashHTMLoading.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 19/04/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "SplashHTMLoading.h"



SplashHTMLoading::SplashHTMLoading(): AnimatedSprite("loader_einstein1.png")
{
    initView();
}

void SplashHTMLoading::initView()
{
    addAnimation(AnimatedSprite::animationWithFile("loader_einstein", 24, 0.030f), "anim");
    runAnimation("anim",true, false);
    cocos2d::CCSize s = cocos2d::CCDirector::sharedDirector()->getWinSize();
    this->setPosition(ccp(s.width/2,s.height/2));
}