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
#include "Floor.h"

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
    Floor *findNearestTransferRightFloor(int actualFloorNumber);
    Floor *findNearestTransferLeftFloor(int actualFloorNumber);
    CCArray *getTransferFloors();
    Elevator *getDefaultElevator();
    Floor *getFloor(int floorNumber);
	int getMaxFloor();
	int getMinFloor();

private:
    static CCDictionary *arrayBuildings;
    static int numBuildings;
    
    CCArray *arrayElevators;
    CCArray *arrayFloors;
    int maxFloor;
    int minFloor;
};

#endif
