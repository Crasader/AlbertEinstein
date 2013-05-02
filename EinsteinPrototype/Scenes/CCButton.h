//
//  CCButton.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 27/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__CCButton__
#define __EinsteinPrototype__CCButton__

#include <iostream>

//
//  CCButton.h
//  test
//
//  Created by  小 苏 http://www.cocos2d-x.org/boards/6/topics/7963
//

#pragma once
#include "cocos2d.h"

 
#include "CCTMXXMLParser.h"
#include "CCTMXLayer.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"

using namespace cocos2d;

#define TAP_MAX_DRAG 10

enum {
    CCControlStateNormal       = 0,
    CCControlStateSelected     = 1 << 0,                  // flag usable by app (see below)
    CCControlStateDisabled     = 1 << 1,
    CCControlStateHighlighted  = 1 << 2                 // todo
};
typedef unsigned int CCControlState;

class CCButton : public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
private:
    bool initWithFile(const char* filename, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);
    cocos2d::CCObject* target;
    cocos2d::SEL_MenuHandler selector;
    bool touched;
    cocos2d::CCPoint touchPoint;
    
    bool isDisabled;
    
    CCControlState curState;
    
cocos2d:cc
    
    cocos2d::CCMutableDictionary<int, cocos2d::CCNode*> * backgroundNodes; //zOrder is 0
    cocos2d::CCMutableDictionary<int, cocos2d::CCNode*> * touchNodes;        //zOrder is 1
    cocos2d::CCMutableDictionary<int, cocos2d::CCString*> * titleCCStrings;  //zOrder is 2
    
    void setState(CCControlState mState);
    
public:
    CCButton();
    ~CCButton();
    
    void initWithFile(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector,const char * mNormal,const char * mSelected,const char * mDisabled=NULL);
    static CCButton * buttonWithFiles(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector,const char * mNormal,const char * mSelected,const char * mDisabled=NULL);
    
    void setTitleForState(const char * mTitle,CCControlState mState);
    const char* titleForState(CCControlState mState);
    
    void setDisable(bool mIsDisabled);
    
    //todo
    virtual void setTitleColorForState(cocos2d::ccColor3B mColor,CCControlState mState){CC_UNUSED_PARAM(mColor);CC_UNUSED_PARAM(mState);};
    virtual cocos2d::ccColor3B titleColorForState(CCControlState mState){CC_UNUSED_PARAM(mState);return cocos2d::ccYELLOW;};
    
    void setImageForState(CCNode* mNode,CCControlState mState);
    CCNode * imageForState(CCControlState mState);
    void setBackgroundImageForState(CCNode *mNode,CCControlState mState);// default is NULL
    CCNode * backgroundImageForState(CCControlState mState);
    
    // touch events
    cocos2d::CCRect rect();
    bool containsTouchLocation(cocos2d::CCTouch* touch);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
};



#endif /* defined(__EinsteinPrototype__CCButton__) */
