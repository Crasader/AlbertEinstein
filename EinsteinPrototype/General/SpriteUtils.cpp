//
//  SpriteUtils.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 9/27/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "SpriteUtils.h"

CCRect SpriteUtils::getBounds(CCNode* object){
	CCSize objectSize = object->getContentSize();
	CCNode* father = object->getParent();
	CCPoint converted = father->convertToWorldSpace(object->getPosition());
	
	return CCRectMake(converted.x - objectSize.width * object->getAnchorPoint().x, 
					  converted.y - objectSize.height * object->getAnchorPoint().y, 
					  objectSize.width, objectSize.height);
}

bool SpriteUtils::hitTestObject(CCNode* obj1, CCNode* obj2){
	if(CCRect::CCRectIntersectsRect(getBounds(obj1), getBounds(obj2))){
		return true;
	}
	
	return false;
}

bool SpriteUtils::hitTestPoint(CCPoint point, CCNode* obj){
	if(CCRect::CCRectContainsPoint(getBounds(obj), point)){
		return true;
	}
	
	return false;
}