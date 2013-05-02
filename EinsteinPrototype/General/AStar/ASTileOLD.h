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

class ASTile : public CCObject{
    
public:
    ASTile();
    ~ASTile();
    
    static ASTile *create();
    
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
    ASTile *getFather();
    void setFather(ASTile *value);
    CCSprite *getSprite();
    void setSprite(CCSprite *value);
    
private:
    bool passable;
    int pointX;
    int pointY;
    int calcF;
    int valueG;
    int calcG;
    int calcH;
    ASTile *father;
    CCSprite *sprite;
    
};



#endif
