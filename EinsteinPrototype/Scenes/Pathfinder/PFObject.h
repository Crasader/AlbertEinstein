//
//  PFObject.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_PFObject_h
#define EinsteinPrototype_PFObject_h

#include "cocos2d.h"


using namespace cocos2d;

class PFObject : public CCObject{
    
public:
    PFObject(int ID, CCString *name);
    ~PFObject();
    
    int getID();
    CCString *getName();
    void setFather(PFObject *father);
    PFObject *getFather();
    
private:
    CCString *name;
    int ID;
    PFObject *father;
};

#endif
