//
//  SpriteUtils.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 9/27/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_SpriteUtils_h
#define EinsteinPrototype_SpriteUtils_h

#include "cocos2d.h"

using namespace cocos2d;

class SpriteUtils{
	
public:
	static CCRect getBounds(CCNode* object);
	static bool hitTestObject(CCNode* obj1, CCNode* obj2);
	static bool hitTestPoint(CCPoint point, CCNode* obj);
	
};


#endif
