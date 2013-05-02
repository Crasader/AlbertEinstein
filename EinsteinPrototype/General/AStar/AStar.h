//
//  AStar.h
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/29/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_AStar_h
#define EinsteinPrototype_AStar_h

#include "ASTile.h"
#include <vector>

class AStar{
        
public:
    AStar();
	~AStar();
	
    std::vector<ASTile> findBestPath(std::vector< std::vector<ASTile> > tiles, ASTile begin, ASTile end, bool smoothed);
	void clear();
    
private:
    ASTile *startTile;
    ASTile *finalTile;
    std::vector< std::vector<ASTile *> > arrayTiles;
	std::vector<ASTile *> openList;
	std::vector<ASTile *> closedList;
	std::vector<ASTile *> nonPassableList;
	std::vector<ASTile> pathArray;
    bool pathFounded;
    int nodesX;
    int nodesY;
    
    void startAStar();
	void eraseUnnecessaryTiles();
	void smooth();
	std::vector<ASTile *> adjustPathToStartSmooth();
	void endSmooth(std::vector<ASTile *> array);
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
	ASTile *getFather(int pointX, int pointY);    
	
};

#endif






