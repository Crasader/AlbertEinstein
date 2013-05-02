//
//  AStar.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/29/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "AStar.h"


AStar::AStar(){
    this->pathFounded = false;
    this->nodesX = 0;
    this->nodesY = 0;
}

AStar::~AStar(){
	
}

AStar *AStar::create(){
    AStar *as = new AStar();
    as->autorelease();
    return as;
}

CCArray *AStar::findBestPath(CCArray *tiles, ASTile *begin, ASTile *end, bool smoothed){
    this->openList = CCArray::create();
    this->openList->retain();
    this->closedList = CCArray::create();
    this->closedList->retain();
    this->nonPassableList = CCArray::create();
    this->nonPassableList->retain();
    this->pathArray = CCArray::create();
    this->pathArray->retain();
    
    this->arrayTiles = tiles;
    this->nodesX = this->arrayTiles->count();
    this->nodesY = ((CCArray *)this->arrayTiles->objectAtIndex(0))->count();
    
    this->startTile = begin;
    this->finalTile = end;
    
    this->startAStar();
	
	if(smoothed){
		this->smooth();
	}
	
	CCArray *returnArray = this->pathArray;
    //this->clear();
    return returnArray;
}

//ASTAR
void AStar::startAStar(){
    for(int i = 0; i < this->nodesX; i++){
        CCArray *arrayX = (CCArray *)this->arrayTiles->objectAtIndex(i);
        for(int j = 0; j < this->nodesY; j++){
            ASTile *tile = (ASTile *)arrayX->objectAtIndex(j);
            if(!tile->getPassable()){
                this->nonPassableList->addObject(tile);
            }
        }
    }
    
    this->startCalc(this->startTile);
    this->arrangePath(this->finalTile);
}

void AStar::clear(){
    for(int i = 0; i < this->nodesX; i++){
        CCArray *arrayX = (CCArray *)this->arrayTiles->objectAtIndex(i);
        for(int j = 0; j < this->nodesY; j++){
            ASTile *tile = (ASTile *)arrayX->objectAtIndex(j);
            tile->setCalcF(0);
            tile->setCalcG(0);
            tile->setValueG(0);
            tile->setCalcH(0);
            tile->setFather(NULL);
        }
    }
    
	this->arrayTiles->removeAllObjects();
	//this->arrayTiles->release();
	
    this->openList->removeAllObjects();
    this->openList->release();
    this->closedList->removeAllObjects();
    this->closedList->release();
    this->nonPassableList->removeAllObjects();
    this->nonPassableList->release();
    this->pathArray->removeAllObjects();
    this->pathArray->release();
    
    this->pathFounded = false;
}

void AStar::startCalc(ASTile *begin){
    if(!this->pathFounded){
        ASTile *actualTile = begin;
        
		this->closedList->addObject(actualTile);
        int count = this->openList->count();
        for(int i = 0; i < count; i++){
            ASTile *tile = (ASTile *)this->openList->objectAtIndex(i);
            if(tile){
                if(tile == actualTile){
                    this->openList->removeObjectAtIndex(i);
                    break;
                }
            }
        }
        
        this->startChildren(actualTile);
        this->chooseMinorFromOpenList();
    }
}

void AStar::startChildren(ASTile *actualTile){
    if(!this->pathFounded){
        int gValue = 0;
        
        for(int i = 0; i < 8; i++){
            ASTile *tile = NULL;
            
            switch (i) {
                case 0:
                    if(actualTile->getPointX() + 1 <= this->nodesX - 1){
                        tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() + 1))->objectAtIndex(actualTile->getPointY());
                        gValue = 10;
                    }
                    break;
                case 1:
                    if(actualTile->getPointX() - 1 >= 0){
                        tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() - 1))->objectAtIndex(actualTile->getPointY());
                        gValue = 10;
                    }
                    break;
                case 2:
                    if(actualTile->getPointY() - 1 >= 0){
                        tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX()))->objectAtIndex(actualTile->getPointY() - 1);
                        gValue = 10;
                    }
                    break;
                case 3:
                    if(actualTile->getPointY() + 1 <= this->nodesY-1){
                        tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX()))->objectAtIndex(actualTile->getPointY() + 1);
                        gValue = 10;
                    }
                    break;
                case 4:
                    if(actualTile->getPointX() - 1 >= 0 && actualTile->getPointY() - 1 >= 0){
                        if(((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() - 1))->objectAtIndex(actualTile->getPointY()))->getPassable() &&
                           ((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX()))->objectAtIndex(actualTile->getPointY() - 1))->getPassable()){
                            tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() - 1))->objectAtIndex(actualTile->getPointY() - 1);
                            gValue = 14;
                        }
                    }
                    break;
                case 5:
                    if(actualTile->getPointX() + 1 <= this->nodesX - 1 && actualTile->getPointY() -1  >= 0){
                        if(((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() + 1))->objectAtIndex(actualTile->getPointY()))->getPassable() &&
                           ((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX()))->objectAtIndex(actualTile->getPointY() - 1))->getPassable()){
                            tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() + 1))->objectAtIndex(actualTile->getPointY() - 1);
                            gValue = 14;
                        }
                    }
                    break;
                case 6:
                    if(actualTile->getPointX() - 1 >= 0 && actualTile->getPointY() + 1 <= this->nodesY - 1){
                        if(((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() - 1))->objectAtIndex(actualTile->getPointY()))->getPassable() &&
                           ((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX()))->objectAtIndex(actualTile->getPointY() + 1))->getPassable()){
                            tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() - 1))->objectAtIndex(actualTile->getPointY() + 1);
                            gValue = 14;
                        }
                    }
                    break;
                case 7:
                    if(actualTile->getPointX() + 1 <= this->nodesX - 1 && actualTile->getPointY() + 1 <= this->nodesY - 1){
                        if(((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() + 1))->objectAtIndex(actualTile->getPointY()))->getPassable() &&
                           ((ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX()))->objectAtIndex(actualTile->getPointY() + 1))->getPassable()){
                            tile = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(actualTile->getPointX() + 1))->objectAtIndex(actualTile->getPointY() + 1);
                            gValue = 14;
                        }
                    }
                    break;
                default:
                    break;
            }
        
            if(tile){
                if(!this->validateClosedLists(tile)){
                    if(!this->validateOpenList(tile)){
                        tile->setFather(actualTile);
                        this->openList->addObject(tile);
                        tile->setValueG(gValue);
                        tile->setCalcG(this->calculateG(tile->getFather(), tile->getValueG()));
                        tile->setCalcH(this->calculateH(tile));
                        tile->setCalcF(tile->getCalcG() + tile->getCalcH());
                    }else{
                        int newG = this->calculateG(actualTile, gValue);
                        if(newG <= tile->getCalcG()){
                            tile->setFather(actualTile);
                            tile->setValueG(gValue);
                            tile->setCalcG(this->calculateG(tile->getFather(), tile->getValueG()));
                            tile->setCalcF(tile->getCalcG() + tile->getCalcH());
                        }
                    }
                }
            }
        }
    }
}


bool AStar::validateClosedLists(ASTile *tile){
    int i;
    int count = this->closedList->count();
    for(i = 0; i < count; i++){
        ASTile *closedTile = (ASTile *)this->closedList->objectAtIndex(i);
        if(tile == closedTile){
            return true;
        }
    }
    
    count = this->nonPassableList->count();
    for(i = 0; i < count; i++){
        ASTile *nonPassableTile = (ASTile *)this->nonPassableList->objectAtIndex(i);
        if(tile == nonPassableTile){
            return true;
        }
    }
    
    return false;
}

bool AStar::validateOpenList(ASTile *tile){
    int count = this->openList->count();
    for(int i = 0; i < count; i++){
        ASTile *openTile = (ASTile *)this->openList->objectAtIndex(i);
        if(openTile == tile){
            return true;
        }
    }
    
    return false;
}

int AStar::calculateG(ASTile *fatherTile, int gValue){
    gValue += fatherTile->getValueG();
    if(fatherTile->getFather()){
        gValue = this->calculateG(fatherTile->getFather(), gValue);
    }
    
    return gValue;
}

int AStar::calculateH(ASTile *tile){
    return ((abs(this->finalTile->getPointX() - tile->getPointX()) + abs(this->finalTile->getPointY() - tile->getPointY())) * 10);
}

void AStar::chooseMinorFromOpenList(){
    ASTile *minorTile = NULL;
    
    int count = this->openList->count();
    for(int i = 0; i < count; i++){
        ASTile *openTile = (ASTile *)this->openList->objectAtIndex(i);
        if(openTile){
            if(!minorTile){
                minorTile = openTile;
            }else{
                if(openTile->getCalcF() <= minorTile->getCalcF()){
                    minorTile = openTile;
                }
            }
        }
    }
    
    if(minorTile){
        if(minorTile == this->finalTile){
            this->pathFounded = true;
        }else{
            this->startCalc(minorTile);
        }
    }
}

void AStar::arrangePath(ASTile *tile){
    this->pathArray->addObject(tile);
    if(tile->getFather()){
        this->arrangePath(tile->getFather());
    }
}



//SMOOTH
void AStar::smooth(){
	CCArray *path = this->adjustPathToStartSmooth();
	CCArray *finalPath = CCArray::create();

	for(int i = 0; i < path->count(); i++){
		ASTile *actual = (ASTile *)path->objectAtIndex(i);
		ASTile *current = (i + 1 <= path->count() - 1 ? (ASTile *)path->objectAtIndex(i + 1) : NULL);
		ASTile *next = (i + 2 <= path->count() - 1 ? (ASTile *)path->objectAtIndex(i + 2) : NULL);
		
		if(next && current){
			if(!this->validadeSmoothWalk(actual, next)){
				finalPath->addObject(current);
			}else if(!this->validadeSmoothSideWalk(actual, next)){
				finalPath->addObject(actual);
			}
		}else{
			if(i == path->count() - 1){
				finalPath->addObject(actual);
			}
		}
	}
	
	this->endSmooth(finalPath);
}

CCArray *AStar::adjustPathToStartSmooth(){
	CCArray *array = CCArray::create();
	
	for(int i = this->pathArray->count() - 1; i >= 0; i--){
		ASTile *tile = (ASTile *)this->pathArray->objectAtIndex(i);
		array->addObject(tile);
	}
	
	return array;
}

void AStar::endSmooth(CCArray *array){
	this->pathArray->removeAllObjects();
	
	int i, k = array->count() - 1;
	for(i = 0; i < array->count(); i++){
		this->pathArray->addObject(array->objectAtIndex(k));
		k--;
	}
}

bool AStar::validadeSmoothWalk(ASTile *start, ASTile *end){
	int startPosX = start->getPointX();
	int startPosY = start->getPointY();
	int endPosX = end->getPointX();
	int endPosY = end->getPointY();
	
	ASTile *tile1; 
	ASTile *tile2;
	
	if(startPosX != endPosX && startPosY != endPosY){
		if(startPosX > endPosX && startPosY > endPosY){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY - 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
		
		if(startPosX > endPosX && startPosY < endPosY){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY + 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
		
		if(startPosX < endPosX && startPosY > endPosY){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY - 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
		
		if(startPosX < endPosX && startPosY < endPosY){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY + 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
	}
	
	return true;
}

bool AStar::validadeSmoothSideWalk(ASTile *start, ASTile *end){
	int startPosX = start->getPointX();
	int startPosY = start->getPointY();
	int endPosX = end->getPointX();
	int endPosY = end->getPointY();
	
	ASTile *tile1;
	ASTile *tile2;
	ASTile *tile3; 
	ASTile *tile4;
	
	if((startPosX != endPosX) && (startPosY == endPosY)){
		if(startPosX > endPosX){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY - 1);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY - 1);
			tile3 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY + 1);
			tile4 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY + 1);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
			
		}
		
		if(startPosX < endPosX){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY - 1);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY - 1);
			tile3 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY + 1);
			tile4 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX))->objectAtIndex(startPosY + 1);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
		}
		
	}else if((startPosX == endPosX) && (startPosY != endPosY)){
		if(startPosY > endPosY){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY - 1);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY);
			tile3 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY - 1);
			tile4 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
		}
		
		if(startPosY < endPosY){
			tile1 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY + 1);
			tile2 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX - 1))->objectAtIndex(startPosY);
			tile3 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY + 1);
			tile4 = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(startPosX + 1))->objectAtIndex(startPosY);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
		}
	}
	
	return true;
}
