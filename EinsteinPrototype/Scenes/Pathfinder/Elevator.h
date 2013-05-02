//
//  Elevator.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Elevator_h
#define EinsteinPrototype_Elevator_h

#include "PFObject.h"

using namespace cocos2d;

class Elevator : public PFObject{
    
public:
    Elevator(int ID, CCString *name, PFObject *waypoint);
    ~Elevator();
    
    static Elevator *create(int ID, CCString *name, PFObject *waypoint);
    static Elevator *get(int ID);
	static void clearArray();
    
    PFObject *getWaypoint();
    
private:
    static CCDictionary *arrayElevators;
    PFObject *waypoint;
    
};

#endif
