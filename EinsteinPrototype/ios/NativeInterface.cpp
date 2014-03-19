//
//  NativeInterface.cpp
//  EinsteinPrototype
//
//  Created by Camargo on 19/03/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "NativeInterface.h"

#include "IOSHelper.h"
#include <string>
using namespace std;

void NativeInterface::nativeMessage(char * title,char * message){
    IOSHelper *hios = new IOSHelper();
    hios->nativeMessage(title,message);
    //string tmp(str);
//    string ret = hios->nativeMessageSearch(tmp);
//    CCString *sret = CCString::create(ret.c_str());
//    return sret;
}
NativeInterface::NativeInterface() {
    
    
}

NativeInterface::~NativeInterface() {
    
}