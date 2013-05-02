//
//  Building.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Building.h"

CCDictionary *Building::arrayBuildings;
int Building::numBuildings;

Building::Building(int ID, CCString *name) : PFObject(ID, name){
    this->arrayElevators = CCArray::create();
    this->arrayFloors = CCArray::create();
    this->maxFloor = -9;
    this->minFloor = 9;
    
}

Building::~Building(){
    
    
}

void Building::create(int ID, CCString *name){
    Building *b = new Building(ID, name);
    b->autorelease();
    
    if(!arrayBuildings){
        arrayBuildings = CCDictionary::create();
        numBuildings = 0;
    }
        
    arrayBuildings->setObject(b, ID);
    numBuildings++;
}

Building *Building::get(int ID){
    return (Building *)arrayBuildings->objectForKey(ID);
}

void Building::addElevator(Elevator *elevator){
    this->arrayElevators->addObject(elevator);
    elevator->setFather(this);
}

void Building::addFloor(Floor *floor){
    this->arrayFloors->addObject(floor);
    floor->setFather(this);
    
    for(int i = 0; i < this->arrayElevators->count(); i++){
        Elevator *elevator = (Elevator *)this->arrayElevators->objectAtIndex(i);
        floor->addWaypoint((Waypoint *)elevator->getWaypoint());
    }
    
    
    if(minFloor > floor->getFloorNumber()){
        minFloor = floor->getFloorNumber();
    }
    
    if(maxFloor < floor->getFloorNumber()){
        maxFloor = floor->getFloorNumber();
    }
}

Floor *Building::findNearestTransferRightFloor(int actualFloorNumber){
    CCArray *transferFloors = this->getTransferFloors();
    Floor *actualFloor = NULL;
    int actualDiff = 99;
    
    for(int i = 0; i < transferFloors->count(); i++){
        Floor *floor = (Floor *)transferFloors->objectAtIndex(i);
        if(floor->getTransferRight()){
            int floorNumber = floor->getFloorNumber();
            int diff = abs(actualFloorNumber - floorNumber);
            if(diff < actualDiff){
                actualDiff = diff;
                actualFloor = floor;
            }
        }
    }
    
    return actualFloor;
}


Floor *Building::findNearestTransferLeftFloor(int actualFloorNumber){
    CCArray *transferFloors = this->getTransferFloors();
    Floor *actualFloor = NULL;
    int actualDiff = 99;
    
    for(int i = 0; i < transferFloors->count(); i++){
        Floor *floor = (Floor *)transferFloors->objectAtIndex(i);
        if(floor->getTransferLeft()){
            int floorNumber = floor->getFloorNumber();
            int diff = abs(actualFloorNumber - floorNumber);
            if(diff < actualDiff){
                actualDiff = diff;
                actualFloor = floor;
            }
        }
    }
    
    return actualFloor;
}

CCArray *Building::getTransferFloors(){
    CCArray *transferFloors = CCArray::create();
    
    for(int i = 0; i < this->arrayFloors->count(); i++){
        Floor *floor = (Floor *)this->arrayFloors->objectAtIndex(i);
        if(floor->getTransferLeft() || floor->getTransferRight()){
            transferFloors->addObject(floor);
        }
    }
    
    return transferFloors;
}

Elevator *Building::getDefaultElevator(){
    if(this->arrayElevators){
        if(this->arrayElevators->count() > 0){
            return (Elevator *)this->arrayElevators->objectAtIndex(0);
        }
    }
    
    return NULL;
}

Floor *Building::getFloor(int floorNumber){
    for(int i = 0; i < this->arrayFloors->count(); i++){
        Floor *floor = (Floor *)this->arrayFloors->objectAtIndex(i);
        if(floor->getFloorNumber() == floorNumber){
            return floor;
        }
    }
    
    return NULL;
}

void Building::clearArray(){
	if(arrayBuildings){
		arrayBuildings->removeAllObjects();
		arrayBuildings = NULL;
	}
}

int Building::getMaxFloor(){
	return maxFloor;
}

int Building::getMinFloor(){
	return minFloor;
}

