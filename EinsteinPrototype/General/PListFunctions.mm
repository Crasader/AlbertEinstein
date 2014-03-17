//
//  PListFunctions.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 19/04/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "PListFunctions.h"
/* 
 Reimplementacao de leitura de plist para efeitos de gerenciamento de memoria e otimizacao
 - Le uma ou manys secoes da chave Services
 */
std::vector<ServiceSection> PListFunctions::readServicesSection()
{
    std::vector<ServiceSection> lstVector;
    NSString *pathOfFile = [[NSBundle mainBundle]pathForResource:@"waypoints" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:pathOfFile];
    NSArray *dataFromPlist = [dict valueForKey:@"Services"];
    
    for(NSString *aKey in dataFromPlist) {
        ServiceSection section;
        
        NSDictionary* oArrChilds = [dataFromPlist valueForKey:aKey];
        section.key = [aKey intValue];
        section.hasChild = false;
        section.isFeatured = false;
         section.isFeatured = false;
        for(NSString *aKey2 in oArrChilds)
        {
            NSString* value = [oArrChilds valueForKey:aKey2];
            if( [aKey2 isEqualToString:@"hasChild"])
                section.hasChild  = [value UTF8String];
            else  if( [aKey2 isEqualToString:@"isFeatured"])
                section.isFeatured  = [value UTF8String];
            else if( [aKey2 isEqualToString:@"categoryID"])
                section.categoryID = (int)[value intValue];
            else  if( [aKey2 isEqualToString:@"name"])
                section.name = [value UTF8String];

        }
        lstVector.push_back(section);
    }
    return lstVector;
}


/*
 Reimplementacao de leitura de plist para efeitos de gerenciamento de memoria e otimizacao
 - Le uma ou mais secoes da chave Buildings
 */
std::vector<BuildingSection> PListFunctions::readBuildingSection()
{
    std::vector<BuildingSection> lstVector;
    NSString *pathOfFile = [[NSBundle mainBundle]pathForResource:@"waypoints" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:pathOfFile];
    NSArray *dataFromPlist = [dict valueForKey:@"buildings"];
    
    for(NSString *aKey in dataFromPlist)
    {
        BuildingSection building;
        building.key = [aKey intValue];
        building.name = [[dataFromPlist valueForKey:aKey] UTF8String];
        lstVector.push_back(building);
    }
    return lstVector;
}


/*
 Reimplementacao de leitura de plist para efeitos de gerenciamento de memoria e otimizacao
 - Le uma ou mais secoes da chave Floors
 */
std::vector<FloorSection> PListFunctions::readFloorSection()
{
    std::vector<FloorSection> lstVector;
    NSString *pathOfFile = [[NSBundle mainBundle]pathForResource:@"waypoints" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:pathOfFile];
    NSArray *dataFromPlist = [dict valueForKey:@"floors"];
    
    for(NSString *aKey in dataFromPlist)
    {
        FloorSection floor;
        floor.key = [aKey intValue];
        NSDictionary* oArrChilds = [dataFromPlist valueForKey:aKey];
        for(NSString *aKey2 in oArrChilds)
        {
            NSString* value = [oArrChilds valueForKey:aKey2];

            if( [aKey2 isEqualToString:@"name"])
                floor.name  = [value UTF8String];
            else if( [aKey2 isEqualToString:@"buildingID"])
                floor.buildingID = (int)[value intValue];
     
        }
        lstVector.push_back(floor);
    }
    return lstVector;
}

/*
 Reimplementacao de leitura de plist para efeitos de gerenciamento de memoria e otimizacao
 - Le uma ou mais secoes da chave Waypoint
 */
std::vector<WayPoint>  PListFunctions::readWayPointSection()
{
    std::vector<WayPoint> lstVector;
    NSString *pathOfFile = [[NSBundle mainBundle]pathForResource:@"waypoints" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:pathOfFile];
    NSArray *dataFromPlist = [dict valueForKey:@"waypoints"];
    
    for(NSString *aKey in dataFromPlist)
    {
        WayPoint way;
        way.key = [aKey intValue];
        NSDictionary* oArrChilds = [dataFromPlist valueForKey:aKey];
        for(NSString *aKey2 in oArrChilds)
        {
            way.isWC = false;
            way.isElevator = false;
            way.isEscalator =false;
            way.isTransferRight= false;
            way.isTransferLeft = false;
    
            NSString* value = [oArrChilds valueForKey:aKey2];
            if( [aKey2 isEqualToString:@"name"])
                way.name  = [value UTF8String];
            else if( [aKey2 isEqualToString:@"floorID"])
                way.floorID = (int)[value intValue];
            else if( [aKey2 isEqualToString:@"wc"])
                way.isWC = (int)[value boolValue];
            else if( [aKey2 isEqualToString:@"elevator"])
                way.isElevator = (int)[value boolValue];
            else if( [aKey2 isEqualToString:@"escalator"])
                way.isEscalator = (int)[value boolValue];
            else if( [aKey2 isEqualToString:@"transferRight"])
                way.isTransferRight= (int)[value boolValue];
            else if( [aKey2 isEqualToString:@"transferLeft"])
                way.isTransferLeft = (int)[value boolValue];
        }
        lstVector.push_back(way);
    }
    return lstVector;
}

/* Reimplementacao de leitura de plist para efeitos de gerenciamento de memoria e otimizacao
 - Retorna um nome formato de um waypoint. Cada string representa uma linha. */
std::vector<std::string> PListFunctions::readWayPointNameFormatado(int key)
{
    NSString *pathOfFile = [[NSBundle mainBundle]pathForResource:@"waypoints" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:pathOfFile];
    NSArray *dataFromPlist = [dict valueForKey:@"WaypointNomeFormatado"];

    std::vector<std::string> lstVector;
    for(NSString *aKey in dataFromPlist)
    {
        if( [aKey intValue] == key)
        {
            NSDictionary* oArrChilds = [dataFromPlist valueForKey:aKey];
            for(NSString *aKey2 in oArrChilds)
            {
                NSArray* value = [oArrChilds valueForKey:aKey2];
                for(NSString* i in value)
                    lstVector.push_back([i UTF8String]);
            }
            return lstVector;
        }
    }

    std::vector<WayPoint> lstGeral = readWayPointSection();
    for(int i = 0;i < lstGeral.size(); i++)
    {
        if( lstGeral[i].key == key)
            lstVector.push_back(lstGeral[i].name);
    }
    return lstVector;
}

/** Retorna todas as keywords cadastradas no PList **/
std::vector<KeyWordSection> PListFunctions::readKeyWordSection( )
{
    NSString *pathOfFile = [[NSBundle mainBundle]pathForResource:@"waypoints" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:pathOfFile];
    NSArray *dataFromPlist = [dict valueForKey:@"keywords"];
    std::vector<KeyWordSection>  lstVector;
    for(NSString *aKey in dataFromPlist)
    {
        KeyWordSection keyWord;
        keyWord.key = [aKey intValue];
        
 
            NSDictionary* oArrChilds = [dataFromPlist valueForKey:aKey];
            for(NSString *aKey2 in oArrChilds)
            {
                id value = [oArrChilds valueForKey:aKey2];
                if ([value isKindOfClass:[NSArray class]])
                {
                    NSArray * dvalue = value;
                    for(NSString* i in dvalue)
                    {
                        keyWord.keywords.push_back([i UTF8String]);
                    }
                }
                //desc
//                else
//                {
//                    NSString * dvalue = value;
//                    keyWord.keywords.push_back([dvalue UTF8String]);
//            }
            }

    
        lstVector.push_back(keyWord);
    }
   return lstVector;
}