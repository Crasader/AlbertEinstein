//
//  ASTile.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/29/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_ASTile_h
#define EinsteinPrototype_ASTile_h

#include "cocos2d.h"

using namespace cocos2d;

class ASTile{
    
public:
    ASTile();
    
    bool getPassable();
    void setPassable(bool value);
    int getPointX();
    void setPointX(int value);
    int getPointY();
    void setPointY(int value);
    int getCalcF();
    void setCalcF(int value);
    int getValueG();
    void setValueG(int value);
    int getCalcG();
    void setCalcG(int value);
    int getCalcH();
    void setCalcH(int value);
    
	int getFatherPointX();
    void setFatherPointX(int value);
    int getFatherPointY();
    void setFatherPointY(int value);
    
private:
    bool passable;
    int pointX;
    int pointY;
    int calcF;
    int valueG;
    int calcG;
    int calcH;
	
	int fatherPointX;
	int fatherPointY;
    
};



#endif
