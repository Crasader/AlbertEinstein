//
//  Elevator.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Elevator.h"

CCDictionary *Elevator::arrayElevators;

Elevator::Elevator(int ID, CCString *name, PFObject *waypoint) : PFObject(ID, name){
    this->waypoint = waypoint;
}

Elevator::~Elevator(){
    
    
}

Elevator *Elevator::create(int ID, CCString *name, PFObject *waypoint){
    Elevator *e = new Elevator(ID, name, waypoint);
    e->autorelease();
    
    if(!arrayElevators){
        arrayElevators = CCDictionary::create();
		arrayElevators->retain();
    }
        
    arrayElevators->setObject(e, ID);
    return e;
}

Elevator *Elevator::get(int ID){
    return (Elevator *)arrayElevators->objectForKey(ID);
}

PFObject *Elevator::getWaypoint(){
    return this->waypoint;
}

void Elevator::clearArray(){
	/*if(arrayElevators != NULL){
		if(arrayElevators->count() > 0){
			arrayElevators->removeAllObjects();
		}
		arrayElevators = NULL;
	}*/
	
	arrayElevators->release();
	arrayElevators = NULL;
}