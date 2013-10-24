//
//  Escalator.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 10/16/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Escalator_h
#define EinsteinPrototype_Escalator_h

#include "PFObject.h"

using namespace cocos2d;

class Escalator : public PFObject{
    
public:
    Escalator(int ID, CCString *name, PFObject *waypoint, int refID);
    ~Escalator();
    
    static Escalator *create(int ID, CCString *name, PFObject *waypoint, int refID);
    static Escalator *get(int ID);
	static void clearArray();
    
    PFObject *getWaypoint();
	int getRefID();
    static CCDictionary *arrayEscalators;
    PFObject *waypoint;
	int refID;
    
private:
    
    
};

#endif
