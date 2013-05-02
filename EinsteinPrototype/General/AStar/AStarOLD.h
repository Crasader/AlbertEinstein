//
//  AStar.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/29/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_AStar_h
#define EinsteinPrototype_AStar_h

#include "cocos2d.h"
#include "ASTile.h"

using namespace cocos2d;

class AStar : public CCObject{
        
public:
    AStar();
    ~AStar();
    
    static AStar *create();
    
    CCArray *findBestPath(CCArray *tiles, ASTile *begin, ASTile *end, bool smoothed);
    void clear();
    
private:
    ASTile *startTile;
    ASTile *finalTile;
    CCArray *arrayTiles;
    CCArray *openList;
    CCArray *closedList;
    CCArray *nonPassableList;
    CCArray *pathArray;
    bool pathFounded;
    int nodesX;
    int nodesY;
    
    void startAStar();
	void smooth();
	CCArray *adjustPathToStartSmooth();
	void endSmooth(CCArray *array);
	bool validadeSmoothWalk(ASTile *start, ASTile *end);
	bool validadeSmoothSideWalk(ASTile *start, ASTile *end);
    
    void startCalc(ASTile *begin);
    void startChildren(ASTile *actualTile);
    bool validateClosedLists(ASTile *tile);
    bool validateOpenList(ASTile *tile);
    int calculateG(ASTile *fatherTile, int gValue);
    int calculateH(ASTile *tile);
    void chooseMinorFromOpenList();
    void arrangePath(ASTile *tile);
    
};

#endif






