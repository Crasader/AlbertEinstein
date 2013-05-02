//
//  ChangeAnchorPoint.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 10/8/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "ChangeAnchorPoint.h"


ChangeAnchorPoint::ChangeAnchorPoint(CCPoint anchor, bool absolutePoint){	
	this->newAP = anchor;
	this->absolute = absolutePoint;
}

ChangeAnchorPoint::~ChangeAnchorPoint(){
	
}

ChangeAnchorPoint *ChangeAnchorPoint::createWithAnchorPoint(CCPoint anchor, bool absolutePoint){
	ChangeAnchorPoint *cap = new ChangeAnchorPoint(anchor, absolutePoint);
    cap->autorelease();
    return cap;
}

void ChangeAnchorPoint::startWithTarget(CCNode *target)
{
	this->cocos2d::CCAction::startWithTarget(target);
	
	CCPoint oldPosition = target->getAnchorPointInPoints();
	
	if(!absolute){
		newAP = ccpAdd(target->getAnchorPointInPoints(), newAP);
	}
	
	target->setAnchorPoint(ccp(newAP.x / (target->getContentSize().width * target->getScaleX()), newAP.y / (target->getContentSize().height * target->getScaleY())));
	
	CCPoint translate = ccpSub(target->convertToWorldSpace(newAP), target->convertToWorldSpace(oldPosition));
	
	target->setPosition(ccpAdd(target->getPosition(), translate));
}