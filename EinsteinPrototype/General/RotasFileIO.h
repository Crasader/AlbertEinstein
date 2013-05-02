//
//  RotasFileIO.h
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 13/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__RotasFileIO__
#define __EinsteinPrototype__RotasFileIO__

#include <iostream>
#include "cocos2d.h"
#include "Enumerations.h"

struct RotaRow
{
    char favorite;
    int wayPoint;
    
    //int targetwayPoint;
    char description[300];
    
};
class RotasFileIO
{
 
public:
    
        std::string getPathFileNamePath();
      bool SaveRota(RotaRow rota);
      bool SaveRota(std::vector<RotaRow> rota);
      bool DeleteRota(int _waypoint);
      bool Exists(int _waypoint);
    
    std::vector<RotaRow> LoadMinhasRotas();

     };

#endif /* defined(__EinsteinPrototype__RotasFileIO__) */
