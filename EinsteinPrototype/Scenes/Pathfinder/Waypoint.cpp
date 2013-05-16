//
//  Waypoint.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Waypoint.h"
#include "Pathfinder.h"

CCDictionary *Waypoint::arrayWaypoints;

Waypoint::Waypoint(int ID, CCString *name) : PFObject(ID, name){
	this->isTransferRight = false;
	this->isTransferLeft = false;
    this->isElevator = false;
	this->isEscalator = false;
	this->isWC = false;
    this->elevatorException = NULL;
}

Waypoint::Waypoint(int ID, CCString *name, bool isElevator, bool isEscalator, bool isWC) : PFObject(ID, name){
	this->isTransferRight = false;
	this->isTransferLeft = false;
    this->isElevator = isElevator;
	this->isEscalator = isEscalator;
	this->isWC = isWC;
    this->elevatorException = NULL;
}

Waypoint::Waypoint(int ID, CCString *name, bool isTransferRight, bool isTransferLeft) : PFObject(ID, name){
	this->isTransferRight = isTransferRight;
	this->isTransferLeft = isTransferLeft;
    this->isElevator = false;
	this->isEscalator = false;
	this->isWC = false;
    this->elevatorException = NULL;
}

Waypoint::~Waypoint(){
    
}

Waypoint *Waypoint::create(int ID, CCString *name, bool isElevator, bool isEscalator, bool isWC){
    Waypoint *w = new Waypoint(ID, name, isElevator, isEscalator, isWC);
    w->autorelease();
    
    if(!arrayWaypoints){
        arrayWaypoints = CCDictionary::create();
		arrayWaypoints->retain();
    }
    
    arrayWaypoints->setObject(w, ID);
	return w;
}

Waypoint *Waypoint::create(int ID, CCString *name){
    Waypoint *w = new Waypoint(ID, name);
    w->autorelease();
    
    if(!arrayWaypoints){
        arrayWaypoints = CCDictionary::create();
		arrayWaypoints->retain();
    }
        
    arrayWaypoints->setObject(w, ID);
    return w;
}

Waypoint *Waypoint::create(int ID, CCString *name, bool isTransferRight = false, bool isTransferLeft = false){
    Waypoint *w = new Waypoint(ID, name, isTransferRight, isTransferLeft);
    w->autorelease();
    
    if(!arrayWaypoints){
        arrayWaypoints = CCDictionary::create();
		arrayWaypoints->retain();
    }
        
    arrayWaypoints->setObject(w, ID);
    return w;
}

Waypoint *Waypoint::get(int ID){
    return (Waypoint *)arrayWaypoints->objectForKey(ID);
}

Elevator *Waypoint::getElevatorException(){
    return elevatorException;
}

void Waypoint::setElevatorException(Elevator *elevatorException){
    this->elevatorException = elevatorException;
}

bool Waypoint::getIsTransferRight(){
	return this->isTransferRight;
}

bool Waypoint::getIsTransferLeft(){
	return this->isTransferLeft;
}

bool Waypoint::getIsElevator(){
    return this->isElevator;
}

bool Waypoint::getIsEscalator(){
	return this->isEscalator;
}

bool Waypoint::getIsWC(){
	return this->isWC;
}

void Waypoint::clearArray(){
	/*if(arrayWaypoints != NULL){
		if(arrayWaypoints->count() > 0){
			arrayWaypoints->removeAllObjects();
		}
		arrayWaypoints = NULL;
	}*/
	
	arrayWaypoints->release();
	arrayWaypoints = NULL;
}


