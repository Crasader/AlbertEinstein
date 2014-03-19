//
//  NativeInterface.h
//  EinsteinPrototype
//
//  Created by Camargo on 19/03/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//




#include "cocos2d.h"
using namespace cocos2d;

class NativeInterface {
public:
    
    static void nativeMessage(char * title,char * message);
    
    NativeInterface();
    virtual ~NativeInterface();
};


