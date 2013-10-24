//
//  Waypoint.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_Waypoint_h
#define EinsteinPrototype_Waypoint_h

#include "Elevator.h"
#include "Escalator.h"

using namespace cocos2d;

class Waypoint : public PFObject{
    
public:
	Waypoint(int ID, CCString *name);
    Waypoint(int ID, CCString *name, bool isElevator, bool isEscalator, bool isWC);
    Waypoint(int ID, CCString *name, bool isTransferRight, bool isTransferLeft);
    ~Waypoint();
    
	static Waypoint *create(int ID, CCString *name);
    static Waypoint *create(int ID, CCString *name, bool isElevator, bool isEscalator, bool isWC);
    static Waypoint *create(int ID, CCString *name, bool isTransferRight, bool isTransferLeft);
    static Waypoint *get(int ID);
	static void clearArray();
    
    void setElevatorException(Elevator *elevatorException);
    void setEscalatorException(Escalator *escalatorException);
    Elevator *getElevatorException();
    Escalator *getEscalatorException();
	bool getIsTransferRight();
	bool getIsTransferLeft();
    bool getIsElevator();
	bool getIsEscalator();
	bool getIsWC();
    
private:
    static CCDictionary *arrayWaypoints;
    
    Elevator *elevatorException;
    Escalator *escalatorException;
	bool isTransferRight;
	bool isTransferLeft;
    bool isElevator;
	bool isEscalator;
	bool isWC;
    
};

#endif
