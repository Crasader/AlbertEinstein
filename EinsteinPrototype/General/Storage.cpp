//
//  Storage.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 12/10/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Storage.h"

int Storage::actualCount = 0;

void Storage::start(){
	if(CCUserDefault::sharedUserDefault()->getIntegerForKey("0") == 0){
		CCUserDefault::sharedUserDefault()->setIntegerForKey("0", 1);
	}
	
	actualCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("0");
}

CCArray *Storage::getFavorites(){
	CCArray *favorites = CCArray::create();
	
	for(int i = 1; i < actualCount; i++){
		int ID = CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("%d", i)->getCString());
		if(ID > 0){
			favorites->addObject(CCInteger::create(ID));
		}
	}
	
	return favorites;
}

void Storage::saveFavorite(int ID){
	CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("%d", actualCount)->getCString(), ID);
	actualCount++;
	CCUserDefault::sharedUserDefault()->setIntegerForKey("0", actualCount);
	CCUserDefault::sharedUserDefault()->flush();
}

void Storage::deleteFavorite(int ID){
	for(int i = 1; i < actualCount; i++){
		int actualID = CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("%d", i)->getCString());
		if(actualID == ID){
			CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("%d", i)->getCString(), 0);
			return;
		}
	}
}