//
//  Escalator.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 10/16/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Escalator.h"

CCDictionary *Escalator::arrayEscalators;

Escalator::Escalator(int ID, CCString *name, PFObject *waypoint, int refID) : PFObject(ID, name){
    this->waypoint = waypoint;
	this->refID = refID;
	
}

Escalator::~Escalator(){
    
    
}

Escalator *Escalator::create(int ID, CCString *name, PFObject *waypoint, int refID){
    Escalator *e = new Escalator(ID, name, waypoint, refID);
    e->autorelease();
    
    if(!arrayEscalators){
        arrayEscalators = CCDictionary::create();
    }
	
    arrayEscalators->setObject(e, ID);
    return e;
}

Escalator *Escalator::get(int ID){
    return (Escalator *)arrayEscalators->objectForKey(ID);
}

PFObject *Escalator::getWaypoint(){
    return this->waypoint;
}

int Escalator::getRefID(){
	return this->refID;
}

void Escalator::clearArray(){
	if(arrayEscalators){
		arrayEscalators->removeAllObjects();
		arrayEscalators = NULL;
	}
}


