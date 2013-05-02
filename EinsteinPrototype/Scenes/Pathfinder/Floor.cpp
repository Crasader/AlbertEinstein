//
//  Floor.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Floor.h"

CCDictionary *Floor::arrayFloors;

Floor::Floor(int ID, CCString *name, int floorNumber, bool transferRight, bool transferLeft): PFObject(ID, name){
    this->floorNumber = floorNumber;
    this->transferRight = transferRight;
    this->transferLeft = transferLeft;
    this->arrayWaypoints = CCArray::create();
	this->escalator = NULL;
}

Floor::~Floor(){
    
    
}

Floor *Floor::create(int ID, CCString *name, int floorNumber, bool transferRight, bool transferLeft){
	Floor *f = new Floor(ID, name, floorNumber, transferRight, transferLeft);
	f->autorelease();
	
	if(!arrayFloors){
		arrayFloors = CCDictionary::create();
	}
		
	arrayFloors->setObject(f, ID);
    return f;
}

Floor *Floor::get(int ID){
    return (Floor *)arrayFloors->objectForKey(ID);
}

void Floor::addWaypoint(Waypoint *waypoint){
    this->arrayWaypoints->addObject(waypoint);
    waypoint->setFather(this);
}

int Floor::getTransferRightWaypointID(){
	if(this->transferRight){
		int count = this->arrayWaypoints->count();
		if(count > 0){
			for(int i = 0; i < count; i++){
				Waypoint *waypoint = (Waypoint *)this->arrayWaypoints->objectAtIndex(i);
				if(waypoint->getIsTransferRight()){
					return waypoint->getID();
				}
			}
		}
	}

	return 0;
}

int Floor::getTransferLeftWaypointID(){
	if(this->transferLeft){
		int count = this->arrayWaypoints->count();
		if(count > 0){
			for(int i = 0; i < count; i++){
				Waypoint *waypoint = (Waypoint *)this->arrayWaypoints->objectAtIndex(i);
				if(waypoint->getIsTransferLeft()){
					return waypoint->getID();
				}
			}
		}
	}

	return 0;
}

CCArray *Floor::getWCs(){
	CCArray *wcs = CCArray::create();
	int count = this->arrayWaypoints->count();
	if(count > 0){
		for(int i = 0; i < count; i++){
			Waypoint *waypoint = (Waypoint *)this->arrayWaypoints->objectAtIndex(i);
			if(waypoint->getIsWC()){
				wcs->addObject(waypoint);
			}
		}
	}
	
	return wcs;
}

int Floor::getFloorNumber(){
    return this->floorNumber;
}

bool Floor::getTransferRight(){
    return this->transferRight;
}

bool Floor::getTransferLeft(){
    return this->transferLeft;
}

int Floor::getStartID(){
	return this->startID;
}

void Floor::setStartID(int value){
	this->startID = value;
}

int Floor::getEndID(){
	return this->endID;
}

void Floor::setEndID(int value){
	this->endID = value;
}

Escalator *Floor::getEscalator(){
	return this->escalator;
}

void Floor::setEscalator(Escalator *value){
	this->escalator = value;
}

void Floor::clearArray(){
	if(arrayFloors){
		arrayFloors->removeAllObjects();
		arrayFloors = NULL;
	}
}

