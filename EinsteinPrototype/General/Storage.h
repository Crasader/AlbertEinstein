//
//  Storage.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 12/10/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Storage_h
#define EinsteinPrototype_Storage_h

#include "cocos2d.h"

using namespace cocos2d;

class Storage{
	
public:
	
	static void start();
	static void deleteFavorite(int ID);
	static void saveFavorite(int ID);
	static CCArray *getFavorites();
	
private:
	static int actualCount;
	
	
};

#endif
