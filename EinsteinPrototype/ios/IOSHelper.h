//
//  IOSHelper.h
//  EinsteinPrototype
//
//  Created by Camargo on 19/03/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//


#include "cocos2d.h"

#include <string>

using namespace std;

class IOSHelper {
public:
    
    void nativeMessage(char * title,char * message);
    
    IOSHelper();
    virtual ~IOSHelper();
};


