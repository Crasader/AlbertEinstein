//
//  ChangeAnchorPoint.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 10/8/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_ChangeAnchorPoint_h
#define EinsteinPrototype_ChangeAnchorPoint_h

#include "cocos2d.h"

using namespace cocos2d;

class ChangeAnchorPoint : public CCActionInstant{
	
public:
	ChangeAnchorPoint(CCPoint anchor, bool absolutePoint);
	~ChangeAnchorPoint();
	static ChangeAnchorPoint *createWithAnchorPoint(CCPoint anchor, bool absolutePoint);
	void startWithTarget(CCNode *target);

private:
	CCPoint newAP;
	bool absolute;
	
};


#endif
