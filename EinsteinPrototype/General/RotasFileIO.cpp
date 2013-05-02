//
//  RotasFileIO.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 13/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "RotasFileIO.h"
 
bool RotasFileIO::SaveRota(RotaRow rota)
{
     FILE *fp = fopen(getPathFileNamePath().c_str(), "a+");
    fwrite(&rota, 1, sizeof(RotaRow), fp);
    fclose(fp);
    return true;
}

bool RotasFileIO::SaveRota(std::vector<RotaRow> lstRotas)
{
    FILE *fp = fopen(getPathFileNamePath().c_str(), "w");
    for(int i = 0; i < lstRotas.size();i++)
        fwrite(&lstRotas[i], 1, sizeof(RotaRow), fp);
    fclose(fp);
    return true;
}

std::string RotasFileIO::getPathFileNamePath()
{
    return cocos2d::CCFileUtils().getWriteablePath() + "/DataUserRoutes.dat";
}

bool RotasFileIO::Exists(int _waypoint)
{
    std::vector<RotaRow> lstRotas = LoadMinhasRotas();
    for(int i = 0; i < lstRotas.size();i++)
        if(lstRotas[i].wayPoint == _waypoint)
            return true;
    return false;
}

bool RotasFileIO::DeleteRota(int _waypoint)
{
    std::vector<RotaRow> lstRotas = LoadMinhasRotas();
    std::vector<RotaRow> newlstRotas;
    
    for(int i = 0; i < lstRotas.size();i++)
        if((lstRotas[i].wayPoint != _waypoint))
            newlstRotas.push_back(lstRotas[i]);
    SaveRota(newlstRotas);
    return true;
}

std::vector<RotaRow> RotasFileIO::LoadMinhasRotas()
{
    std::vector<RotaRow> lstRotas;
    size_t result;
    FILE *fp = 0;
    fp = fopen(getPathFileNamePath().c_str(), "r");
    if(fp != 0)
    {
        while(true)
        {
            RotaRow row;
            result = fread(&row, 1, sizeof(RotaRow),  fp);
            if(row.wayPoint == 0)
                result = 0;
            if(result == 0)
                break;
            else
                lstRotas.push_back(row);
        }
        fclose(fp);
    }
    return lstRotas;
}