//
//  PListFunctions.h
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 19/04/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__PListFunctions__
#define __EinsteinPrototype__PListFunctions__

#include <iostream>
#include<vector>

struct ServiceSection
{
 public:
    int key;
    const char* name;
    int categoryID;
    const char* hasChild;
    const char* isFeatured;
};


struct BuildingSection
{
    
 public:
    int key;
    const char* name;
};


struct FloorSection
{
  public:
    int key;
    int buildingID;
    const char* name;
};

struct KeyWordSection
{
    public:
    int key;
    std::vector<std::string> keywords;
};

struct WayPoint
{
    int key;
    int floorID;
    const char* name;
    const char* desc;
    bool isWC;
    bool isElevator ;
    bool isEscalator;
    bool isTransferRight ;
    bool isTransferLeft ;
};

 class PListFunctions
{
    public:
        static std::vector<ServiceSection> readServicesSection();
        static std::vector<BuildingSection> readBuildingSection();
        static std::vector<FloorSection> readFloorSection();
        static std::vector<WayPoint> readWayPointSection();
    static std::vector<std::string> readWayPointNameFormatado(int key);
    static std::vector<std::string> readWayPointDescriptionFormatado(int key);
    static std::vector<KeyWordSection> readKeyWordSection();
};

#endif /* defined(__EinsteinPrototype__PListFunctions__) */
