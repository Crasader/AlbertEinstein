//
//  Floor.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Floor_h
#define EinsteinPrototype_Floor_h

#include "PFObject.h"
#include "Waypoint.h"
#include "Escalator.h"

using namespace cocos2d;

class Floor : public PFObject{
    
public:
    Floor(int ID, CCString *name, int floorNumber, bool transferRight, bool transferLeft);
    ~Floor();
    
    static Floor *create(int ID, CCString *name, int floorNumber, bool transferRight, bool transferLeft);
    static Floor *get(int ID);
	static void clearArray();
    
    void addWaypoint(Waypoint *waypoint);
    int getFloorNumber();
    bool getTransferRight();
    bool getTransferLeft();
	int getStartID();
	void setStartID(int value);
	int getEndID();
	void setEndID(int value);
	int getTransferRightWaypointID();
	int getTransferLeftWaypointID();
	CCArray *getWCs();
	Escalator *getEscalator();
	void setEscalator(Escalator *value);
    
private:
    static CCDictionary *arrayFloors;
    
    int floorNumber;
    bool transferRight;
    bool transferLeft;
	int startID;
	int endID;
	Escalator *escalator;
	CCArray *arrayWaypoints;
    
};

#endif
