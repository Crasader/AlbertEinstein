//
//  Building.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Building_h
#define EinsteinPrototype_Building_h

#include "Elevator.h"
#include "Escalator.h"
#include "Floor.h"
#include <map>

using namespace cocos2d;

class Building : public PFObject{
    
public:
    Building(int ID, CCString *name);
    ~Building();
    
    static void create(int ID, CCString *name);
    static Building *get(int ID);
    static void clearArray();
	
    void addElevator(Elevator *elevator);
    void addFloor(Floor *floor);
    void addEscalator(Escalator *escalator);
    Floor *findNearestTransferRightFloor(int actualFloorNumber);
    Floor *findNearestTransferLeftFloor(int actualFloorNumber);
    CCArray *getTransferFloors();
    Elevator *getDefaultElevator();
    Escalator *getDefaultEscalator();
    Floor *getFloor(int floorNumber);
	int getMaxFloor();
	int getMinFloor();
    CCArray *arrayElevators;
    CCArray *arrayEscalators;
    CCArray *arrayFloors;
private:
	static CCDictionary *arrayBuildings;
    static int numBuildings;
    
    
    int maxFloor;
    int minFloor;
};

#endif
