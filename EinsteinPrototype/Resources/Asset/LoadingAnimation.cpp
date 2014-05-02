//
//  LogoAnimation.cpp
//  EinsteinPrototype
//
//  Created by Camargo on 01/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "LoadingAnimation.h"
LoadingAnimation::LoadingAnimation()
{
    
  //cache
    cocos2d::CCSpriteFrameCache* framecache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    framecache->addSpriteFramesWithFile("loading.plist");
   //retain
    cocos2d::CCSpriteBatchNode *batch = cocos2d::CCSpriteBatchNode::create("loading.png");
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("loading.plist");
    this->addChild(batch, 1);
    
    //frames
  
    cocos2d::CCAnimation* loadingAnim =cocos2d::CCAnimation::create();
    loadingAnim->setDelayPerUnit(0.5);
    
    theframes = new cocos2d::CCArray();
    for (int i = 1; i <= 145; i++) {
        cocos2d::CCString* filename = cocos2d::CCString::createWithFormat("loader_einstein%d.png", i);
        cocos2d::CCSpriteFrame* frame = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename->getCString());
        theframes->addObject(frame);
        loadingAnim->addSpriteFrame(frame);
    }
    
    //animation
    
    //sprite
    loadingFrame = cocos2d::CCSprite::createWithSpriteFrameName("loader_einstein1.png");
    cocos2d::CCSize bubFrame = cocos2d::CCDirector::sharedDirector()->getWinSize();
    loadingFrame->setPosition(ccp(bubFrame.width / 2, bubFrame.height-120));
    loadingFrame->setScale(1.0f);
    
    //action
    action = cocos2d::CCRepeatForever::create(cocos2d::CCAnimate::create(loadingAnim));
    this->addChild(loadingFrame,100);
    

}
void LoadingAnimation::runAnimation()
{
   loadingFrame->runAction(action);
}

void LoadingAnimation::stopAnimation()
{
    loadingFrame->stopAction(action);
}
