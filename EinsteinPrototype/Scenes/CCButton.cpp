//
//  CCButton.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 27/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "CCButton.h"



//
//  CCButton.h
//  test
//
//  Created by  小 苏 http://www.cocos2d-x.org/boards/6/topics/7963
//

#include "CCButton.h"

#define CCButtonZorder0    0
#define CCButtonZorder1    1
#define CCButtonZorder2    2

#define CCButtonBgTag  1000001
#define CCButtonTouchedTag  1000002
#define CCButtonTitleTag 1000003

using namespace cocos2d;

CCButton::CCButton():CCNode()
{
    backgroundNodes = new CCMutableDictionary<int,CCNode*>();
    touchNodes = new CCMutableDictionary<int,CCNode*>();
    titleCCStrings = new CCMutableDictionary<int,CCString*>();
    isDisabled = false;
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

CCButton::~CCButton()
{
    CC_SAFE_DELETE(backgroundNodes);
    CC_SAFE_DELETE(touchNodes);
    CC_SAFE_DELETE(titleCCStrings);
}

void CCButton::initWithFile(CCObject* target, SEL_MenuHandler selector,const char * mNormal,const char * mSelected,const char * mDisabled)
{
    this->target = target;
    this->selector = selector;
    
    if (mNormal != NULL) {
        CCSprite * mNormalSprite = CCSprite::spriteWithFile(mNormal);
        mNormalSprite->setTag(CCButtonTouchedTag);
        this->addChild(mNormalSprite, CCButtonZorder1);
        touchNodes->setObject(mNormalSprite,CCControlStateNormal);
        
        //set the content size.
        CCSize mContentSize = mNormalSprite->getContentSize();
        this->setContentSize(mContentSize);
    }
    
    if (mSelected != NULL) {
        CCSprite * mSelectedSprite = CCSprite::spriteWithFile(mSelected);
        mSelectedSprite->setTag(CCButtonTouchedTag);
        touchNodes->setObject(mSelectedSprite,CCControlStateSelected);
    }
    
    if (mDisabled != NULL) {
        CCSprite * mDisabledSprite = CCSprite::spriteWithFile(mDisabled);
        mDisabledSprite->setTag(CCButtonTouchedTag);
        touchNodes->setObject(mDisabledSprite,CCControlStateDisabled);
    }
    
    touched = false;
}

CCButton * CCButton::buttonWithFiles(CCObject* target, SEL_MenuHandler selector,const char * mNormal,const char * mSelected,const char * mDisabled)
{
    CCButton *pRet = new CCButton();
    if (pRet)
    {
        pRet->initWithFile(target, selector,mNormal,mSelected,mDisabled);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet)
    return NULL;
}

void CCButton::setState(CCControlState mState)
{
    ////set background node
    CCNode * bgNode = backgroundImageForState(mState);
    if (bgNode != NULL) {
        bgNode->setTag(CCButtonBgTag);
        CCNode * mTepNode = this->getChildByTag(CCButtonBgTag);
        if (mTepNode != NULL) {
            mTepNode->removeFromParentAndCleanup(true);
        }
        this->addChild(bgNode, CCButtonZorder0);
    }
    
    //set touch node
    CCNode * stateNode = imageForState(mState);
    if (stateNode != NULL) {
        stateNode->setTag(CCButtonTouchedTag);
        CCNode * mTepNode = this->getChildByTag(CCButtonTouchedTag);
        if (mTepNode != NULL) {
            mTepNode->removeFromParentAndCleanup(true);
        }
        this->addChild(stateNode, CCButtonZorder1);
    }
    
    //set title node
    const char*  titleForState = this->titleForState(mState);
    if (titleForState != NULL) {
        this->setTitleForState(titleForState,mState);
    }
}

void CCButton::setDisable(bool mIsDisabled)
{
    this->isDisabled = mIsDisabled;
    if (mIsDisabled == true) {
        setState(CCControlStateDisabled);
    }
    else {
        setState(CCControlStateNormal);
    }
}

void CCButton::setTitleForState(const char * mTitle,CCControlState mState)
{
    if (mTitle == NULL) {
        return;
    }
    
    CCLabelTTF * mTepNode = (CCLabelTTF *) (this->getChildByTag(CCButtonTitleTag));
    if (mTepNode != NULL) {
        mTepNode->setString(mTitle);
    }
    else {
        CCSize mContentSize = this->getContentSize();
        mTepNode = CCLabelTTF::labelWithString(mTitle, "Arial", 14);
        mTepNode->setTag(CCButtonTitleTag);
        this->setPosition(ccp(mContentSize.width/2.0,mContentSize.height/2.0));
        this->addChild(mTepNode, CCButtonZorder2);
    }
    
    CCSize mContentSize = mTepNode->getContentSize();
    mContentSize.width += 8;
    mContentSize.height += 6;
    this->setContentSize(mContentSize);
    
    CCString * mTepNodeInList = (CCString *) (this->titleCCStrings->objectForKey((int)mState));
    if (mTepNodeInList != NULL) {
        std::string mTepString = mTepNodeInList->toStdString();
        std::string mTepCompareString(mTitle);
        if (mTepString.compare(mTepCompareString) == 0) {
            return;
        }
        this->titleCCStrings->removeObjectForKey((int)mState);
    }
    
    //save string
    CCString * mNormalCCString = new CCString(mTitle);
    mNormalCCString->autorelease();
    this->titleCCStrings->setObject(mNormalCCString,mState);
}

const char * CCButton::titleForState(CCControlState mState)
{
    CCString * mTepNodeInList = (CCString *) (this->titleCCStrings->objectForKey((int)mState));
    if (mTepNodeInList != NULL) {
        return mTepNodeInList->toStdString().c_str();
    }
    return NULL;
}

/*
 void CCButton::setTitleColorForState(ccColor3B mColor,CCControlState mState)
 {
 CC_UNUSED_PARAM(mColor);
 CC_UNUSED_PARAM(mState);
 }
 
 ccColor3B CCButton::titleColorForState(CCControlState mState)
 {
 CC_UNUSED_PARAM(mState);
 }
 */

void CCButton::setImageForState(CCNode* mNode,CCControlState mState)
{
    if (mNode == NULL) {
        return;
    }
    
    CCSize mContentSize = mNode->getContentSize();
    this->setContentSize(mContentSize);
    
    CCNode * mTepNodeInList = (CCNode *) (this->touchNodes->objectForKey((int)mState));
    if (mTepNodeInList != NULL) {
        this->touchNodes->removeObjectForKey((int)mState);
    }
    
    this->touchNodes->setObject(mNode,(int)mState);
    
    if (mState == CCControlStateNormal) {
        this->setState(CCControlStateNormal);
    }
}

CCNode * CCButton::imageForState(CCControlState mState)
{
    if (this->touchNodes->count() == 0) {
        return NULL;
    }
    
    CCNode * mTepNodeInList = (CCNode *) (this->touchNodes->objectForKey((int)mState));
    if (mTepNodeInList != NULL) {
        return mTepNodeInList;
    }
    return NULL;
}

void CCButton::setBackgroundImageForState(CCNode *mNode,CCControlState mState)// default is nil
{
    if (mNode == NULL) {
        return;
    }
    
    CCSize mContentSize = mNode->getContentSize();
    this->setContentSize(mContentSize);
    
    CCNode * mTepNodeInList = (CCNode *) (this->backgroundNodes->objectForKey((int)mState));
    if (mTepNodeInList != NULL) {
        this->backgroundNodes->removeObjectForKey((int)mState);
    }
    
    this->backgroundNodes->setObject(mNode,(int)mState);
    
    if (mState == CCControlStateNormal) {
        this->setState(CCControlStateNormal);
    }
}

CCNode * CCButton::backgroundImageForState(CCControlState mState)
{
    if (this->backgroundNodes->count() == 0) {
        return NULL;
    }
    
    CCNode * mTepNodeInList = (CCNode *) (this->backgroundNodes->objectForKey((int)mState));
    if (mTepNodeInList != NULL) {
        return mTepNodeInList;
    }
    return NULL;
}

void CCButton::onEnter()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
    CCNode::onEnter();
}

void CCButton::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

CCRect CCButton::rect()
{
    CCSize s = this->getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool CCButton::containsTouchLocation(CCTouch* touch)
{
    return CCRect::CCRectContainsPoint(rect(), convertTouchToNodeSpaceAR(touch));
}

bool CCButton::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    if (!this->isDisabled && containsTouchLocation(pTouch))
    {
        touched = true;
        touchPoint = pTouch->locationInView();
        this->setState(CCControlStateSelected);
        return true;
    }
    return false;
}

void CCButton::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint currTouchPoint = pTouch->locationInView();
    // cancel touch if dragged too much
    if (ccpDistance(touchPoint,currTouchPoint) > TAP_MAX_DRAG)
    {
        this->setState(CCControlStateNormal);
        
        touched = false;
    }
}

void CCButton::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    if (touched)
    {
        this->setState(CCControlStateNormal);
        
        //perform selector
        (target->*selector)(this);
        touched = false;
    }
}

void CCButton::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    this->setState(CCControlStateNormal);
    
    touched = false;
}

void CCButton::touchDelegateRetain()
{
    this->retain();
}
void CCButton::touchDelegateRelease()
{
    this->release();
}