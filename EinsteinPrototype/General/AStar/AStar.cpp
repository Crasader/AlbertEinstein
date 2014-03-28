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
	this->arrayTiles.clear();
	this->openList.clear();
	this->closedList.clear();
	this->nonPassableList.clear();
	this->pathArray.clear();
}

void AStar::clear(){
	this->startTile = NULL;
	this->finalTile = NULL;
	
	/*for(int i = 0; i < this->arrayTiles.size(); i++){
		std::vector<ASTile *> arrTiles;
		for(int j = 0; j < arrayTiles.at(i).size(); j++){
			//delete arrayTiles.at(i).at(j);
		}
	}*/
	
	
	//this->arrayTiles.clear();
	//this->openList.clear();
	//this->closedList.clear();
	//this->nonPassableList.clear();
	//this->pathArray.clear();
	
}

std::vector<ASTile> AStar::findBestPath(std::vector< std::vector<ASTile> >tiles, ASTile begin, ASTile end, bool smoothed){
	this->openList = std::vector<ASTile*>();
	this->closedList = std::vector<ASTile*>();
	this->nonPassableList = std::vector<ASTile*>();
	this->pathArray = std::vector<ASTile>();
	
	this->arrayTiles = std::vector< std::vector<ASTile *> >();
	for(int i = 0; i < tiles.size(); i++){
		std::vector<ASTile *> arrTiles;
		for(int j = 0; j < tiles.at(i).size(); j++){
			arrTiles.push_back(&tiles.at(i).at(j));
		}
		
		this->arrayTiles.push_back(arrTiles);
	}
	
	this->nodesX = this->arrayTiles.size();
    this->nodesY = this->arrayTiles.at(0).size();
    
	std::cout<<"X "<<this->nodesX<<"("<<begin.getPointX()<<" "<<end.getPointX()<<")";
	std::cout<<" Y "<<this->nodesY<<"("<<begin.getPointY()<< " " << end.getPointY() <<")\n";
	
    this->startTile = this->arrayTiles.at(begin.getPointX()).at(begin.getPointY());
	
	CCLOG("pX, %d", end.getPointX());
	CCLOG("pY, %d", end.getPointY());
	//LUCAS
    try {
        this->finalTile = this->arrayTiles.at(end.getPointX()).at(end.getPointY());
    } catch(std::exception& e) {
		std::cout<<"Exception at "<<__LINE__<<" "<<__FILE__<<"\n";
		//this->finalTile = "";
	}
	
	
    this->startAStar();
	if(smoothed){
		this->smooth();
	}
	
	this->eraseUnnecessaryTiles();
	this->clear();
	return this->pathArray;
}

void AStar::eraseUnnecessaryTiles(){
	int indexTileStart = 0;
	int indexTileInValidation = 1;
	int indexTileNext = 2;
	
	while(indexTileNext < this->pathArray.size()){
		ASTile tileStart = this->pathArray.at(indexTileStart);
		ASTile tileInValidation = this->pathArray.at(indexTileInValidation);
		ASTile tileNext = this->pathArray.at(indexTileNext);
		
		int angleValidation_Start = atan2f(tileStart.getPointY() - tileInValidation.getPointY(), tileStart.getPointX() - tileInValidation.getPointX()) * 180 / M_PI;
		int angleNext_Validation = atan2f(tileInValidation.getPointY() - tileNext.getPointY(), tileInValidation.getPointX() - tileNext.getPointX()) * 180 / M_PI;
		
		float distance = ccpDistance(ccp(tileStart.getPointX(), tileStart.getPointY()), ccp(tileInValidation.getPointX(), tileInValidation.getPointY()));
		
		int realAngle = angleValidation_Start-angleNext_Validation;
		std::cout<<"A: "<<realAngle<<" "<<angleNext_Validation<<" "<<angleValidation_Start<<"\n";
		
		//if(angleNext_Validation == angleValidation_Start /*|| distance < 3*/){
		if(realAngle > -30 && realAngle < 30) {
			this->pathArray.erase(this->pathArray.begin() + indexTileInValidation);
		}else{
			indexTileStart++;
			indexTileInValidation++;
			indexTileNext++;
		}
	}
}


//ASTAR
void AStar::startAStar(){
	int i, j;
	for(i = 0; i < this->nodesX; i++){
        std::vector<ASTile*> arrayX = this->arrayTiles.at(i);
        for(j = 0; j < this->nodesY; j++){
            ASTile* tile = arrayX.at(j);
            if(!tile->getPassable()){
                this->nonPassableList.push_back(tile);
            }
        }
    }
	
    this->startCalc(this->startTile);
	this->arrangePath(this->finalTile);
}

void AStar::startCalc(ASTile* begin){
	if(!this->pathFounded){
        ASTile* actualTile = begin;
        
		this->closedList.push_back(actualTile);
        int count = this->openList.size();
        for(int i = 0; i < count; i++){
            ASTile* tile = this->openList.at(i);
			if(tile == actualTile){
				this->openList.erase(this->openList.begin() + i);
				break;
			}
        }
        
        this->startChildren(actualTile);
        this->chooseMinorFromOpenList();
    }
}

void AStar::startChildren(ASTile* actualTile){
    if(!this->pathFounded){
        for(int i = 0; i < 8; i++){
			int gValue = 0;
            ASTile* tile;
            
            switch (i) {
                case 0:
                    if(actualTile->getPointX() + 1 <= this->nodesX - 1){
                        tile = this->arrayTiles.at(actualTile->getPointX() + 1).at(actualTile->getPointY());
                        gValue = 10;
                    }
                    break;
                case 1:
                    if(actualTile->getPointX() - 1 >= 0){
                        tile = this->arrayTiles.at(actualTile->getPointX() - 1).at(actualTile->getPointY());
                        gValue = 10;
                    }
                    break;
                case 2:
                    if(actualTile->getPointY() - 1 >= 0){
                        tile = this->arrayTiles.at(actualTile->getPointX()).at(actualTile->getPointY() - 1);
                        gValue = 10;
                    }
                    break;
                case 3:
                    if(actualTile->getPointY() + 1 <= this->nodesY-1){
                        tile = this->arrayTiles.at(actualTile->getPointX()).at(actualTile->getPointY() + 1);
                        gValue = 10;
                    }
                    break;
                case 4:
                    if(actualTile->getPointX() - 1 >= 0 && actualTile->getPointY() - 1 >= 0){
                        if(this->arrayTiles.at(actualTile->getPointX() - 1).at(actualTile->getPointY())->getPassable() &&
                           this->arrayTiles.at(actualTile->getPointX()).at(actualTile->getPointY() - 1)->getPassable()){
                            tile = this->arrayTiles.at(actualTile->getPointX() - 1).at(actualTile->getPointY() - 1);
                            gValue = 14;
                        }
                    }
                    break;
                case 5:
                    if(actualTile->getPointX() + 1 <= this->nodesX - 1 && actualTile->getPointY() -1  >= 0){
                        if(this->arrayTiles.at(actualTile->getPointX() + 1).at(actualTile->getPointY())->getPassable() &&
                           this->arrayTiles.at(actualTile->getPointX()).at(actualTile->getPointY() - 1)->getPassable()){
                            tile = this->arrayTiles.at(actualTile->getPointX() + 1).at(actualTile->getPointY() - 1);
                            gValue = 14;
                        }
                    }
                    break;
                case 6:
                    if(actualTile->getPointX() - 1 >= 0 && actualTile->getPointY() + 1 <= this->nodesY - 1){
                        if(this->arrayTiles.at(actualTile->getPointX() - 1).at(actualTile->getPointY())->getPassable() &&
                           this->arrayTiles.at(actualTile->getPointX()).at(actualTile->getPointY() + 1)->getPassable()){
                            tile = this->arrayTiles.at(actualTile->getPointX() - 1).at(actualTile->getPointY() + 1);
                            gValue = 14;
                        }
                    }
                    break;
                case 7:
                    if(actualTile->getPointX() + 1 <= this->nodesX - 1 && actualTile->getPointY() + 1 <= this->nodesY - 1){
                        if(this->arrayTiles.at(actualTile->getPointX() + 1).at(actualTile->getPointY())->getPassable() &&
                           this->arrayTiles.at(actualTile->getPointX()).at(actualTile->getPointY() + 1)->getPassable()){
                            tile = this->arrayTiles.at(actualTile->getPointX() + 1).at(actualTile->getPointY() + 1);
                            gValue = 14;
                        }
                    }
                    break;
                default:
                    break;
            }
			
            if(gValue > 0){
                if(!this->validateClosedLists(tile)){
					if(!this->validateOpenList(tile)){
						tile->setFatherPointX(actualTile->getPointX());
						tile->setFatherPointY(actualTile->getPointY());
                        tile->setValueG(gValue);
                        tile->setCalcG(this->calculateG(this->getFather(tile->getFatherPointX(), tile->getFatherPointY()), tile->getValueG()));
                        if (actualTile== NULL || tile == NULL) {
                            CCLOG("erro");
                        }
                        tile->setCalcH(this->calculateH(tile));
                        tile->setCalcF(tile->getCalcG() + tile->getCalcH());
						this->openList.push_back(tile);
                    }else{
                        int newG = this->calculateG(actualTile, gValue);
                        if(newG <= tile->getCalcG()){
							tile->setFatherPointX(actualTile->getPointX());
							tile->setFatherPointY(actualTile->getPointY());
                            tile->setValueG(gValue);
                            tile->setCalcG(this->calculateG(this->getFather(tile->getFatherPointX(), tile->getFatherPointY()), tile->getValueG()));
                            tile->setCalcF(tile->getCalcG() + tile->getCalcH());
                        }
                    }
                }
            }
        }
    }
}


bool AStar::validateClosedLists(ASTile* tile){
    int i;
    int count = this->closedList.size();
    for(i = 0; i < count; i++){
        ASTile* closedTile = this->closedList.at(i);
        if(tile == closedTile){
            return true;
        }
    }
    
    count = this->nonPassableList.size();
    for(i = 0; i < count; i++){
        ASTile* nonPassableTile = this->nonPassableList.at(i);
        if(tile == nonPassableTile){
            return true;
        }
    }
    
    return false;
}

bool AStar::validateOpenList(ASTile* tile){
    int count = this->openList.size();
    for(int i = 0; i < count; i++){
        ASTile* openTile = this->openList.at(i);
        if(openTile == tile){
            return true;
        }
    }
    
    return false;
}

int AStar::calculateG(ASTile* fatherTile, int gValue){
    gValue += fatherTile->getValueG();
    if(fatherTile->getFatherPointX() > -1){
        gValue = this->calculateG(this->getFather(fatherTile->getFatherPointX(), fatherTile->getFatherPointY()), gValue);
    }
    
    return gValue;
}

int AStar::calculateH(ASTile* tile){
    if (finalTile == NULL) {
        finalTile = finalTile;
    }
         return ((abs(this->finalTile->getPointX() - tile->getPointX()) + abs(this->finalTile->getPointY() - tile->getPointY())) * 10);
    //}

      //  return 0;
}

void AStar::chooseMinorFromOpenList(){
    ASTile* minorTile;
	int valueF = 0;
    
	int count = this->openList.size();
    for(int i = 0; i < count; i++){
        ASTile* openTile = this->openList.at(i);
        
		if(valueF == 0){
			minorTile = openTile;
			valueF = minorTile->getCalcF();
		}else{
			if(openTile->getCalcF() <= minorTile->getCalcF()){
				minorTile = openTile;
				valueF = minorTile->getCalcF();
			}
		}
    }
    
    if(valueF > 0){
        if(minorTile == this->finalTile){
			this->pathFounded = true;
        }else{
			this->startCalc(minorTile);
        }
    }
}

void AStar::arrangePath(ASTile* tile){
	this->pathArray.push_back(*tile);
	if(tile->getFatherPointX() > -1){
        this->arrangePath(this->getFather(tile->getFatherPointX(), tile->getFatherPointY()));
    }
}


//SMOOTH
void AStar::smooth(){
	std::vector<ASTile*> path = this->adjustPathToStartSmooth();
	std::vector<ASTile*> finalPath = std::vector<ASTile*>();

	for(int i = 0; i < path.size(); i++){
		ASTile* actual = (ASTile*)path.at(i);
		ASTile* current = (i + 1 <= path.size() - 1 ? path.at(i + 1) : NULL);
		ASTile* next = (i + 2 <= path.size() - 1 ? path.at(i + 2) : NULL);
		
		if(next && current){
			if(!this->validadeSmoothWalk(actual, next)){
				finalPath.push_back(current);
			}else if(!this->validadeSmoothSideWalk(actual, next)){
				finalPath.push_back(actual);
			}
		}else{
			if(i == path.size() - 1){
				finalPath.push_back(actual);
			}
		}
	}
	
	this->endSmooth(finalPath);
}

std::vector<ASTile*> AStar::adjustPathToStartSmooth(){
	std::vector<ASTile*> array = std::vector<ASTile*>();
	
	for(int i = this->pathArray.size() - 1; i >= 0; i--){
		ASTile* tile = &this->pathArray.at(i);
		array.push_back(tile);
	}
	
	return array;
}

void AStar::endSmooth(std::vector<ASTile*> array){
	this->pathArray.clear();
	
	int i, k = array.size() - 1;
	for(i = 0; i < array.size(); i++){
		this->pathArray.push_back(*array.at(k));
		k--;
	}
}

bool AStar::validadeSmoothWalk(ASTile* start, ASTile* end){
	int startPosX = start->getPointX();
	int startPosY = start->getPointY();
	int endPosX = end->getPointX();
	int endPosY = end->getPointY();
	
	ASTile* tile1;
	ASTile* tile2;
	
	if(startPosX != endPosX && startPosY != endPosY){
		if(startPosX > endPosX && startPosY > endPosY){
			tile1 = this->arrayTiles.at(startPosX - 1).at(startPosY);
			tile2 = this->arrayTiles.at(startPosX).at(startPosY - 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
		
		if(startPosX > endPosX && startPosY < endPosY){
			tile1 = this->arrayTiles.at(startPosX - 1).at(startPosY);
			tile2 = this->arrayTiles.at(startPosX).at(startPosY + 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
		
		if(startPosX < endPosX && startPosY > endPosY){
			tile1 = this->arrayTiles.at(startPosX + 1).at(startPosY);
			tile2 = this->arrayTiles.at(startPosX).at(startPosY - 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
		
		if(startPosX < endPosX && startPosY < endPosY){
			tile1 = this->arrayTiles.at(startPosX + 1).at(startPosY);
			tile2 = this->arrayTiles.at(startPosX).at(startPosY + 1);
			if(!tile1->getPassable() || !tile2->getPassable()){
				return false;
			}
		}
	}
	
	return true;
}

bool AStar::validadeSmoothSideWalk(ASTile* start, ASTile* end){
	int startPosX = start->getPointX();
	int startPosY = start->getPointY();
	int endPosX = end->getPointX();
	int endPosY = end->getPointY();
	
	ASTile* tile1;
	ASTile* tile2;
	ASTile* tile3;
	ASTile* tile4;
	
	if((startPosX != endPosX) && (startPosY == endPosY)){
		if(startPosX > endPosX){
			tile1 = this->arrayTiles.at(startPosX - 1).at(startPosY - 1);
			tile2 = this->arrayTiles.at(startPosX).at(startPosY - 1);
			tile3 = this->arrayTiles.at(startPosX - 1).at(startPosY + 1);
			tile4 = this->arrayTiles.at(startPosX).at(startPosY + 1);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
			
		}
		
		if(startPosX < endPosX){
			tile1 = this->arrayTiles.at(startPosX + 1).at(startPosY - 1);
			tile2 = this->arrayTiles.at(startPosX).at(startPosY - 1);
			tile3 = this->arrayTiles.at(startPosX + 1).at(startPosY + 1);
			tile4 = this->arrayTiles.at(startPosX).at(startPosY + 1);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
		}
		
	}else if((startPosX == endPosX) && (startPosY != endPosY)){
		if(startPosY > endPosY){
			tile1 = this->arrayTiles.at(startPosX - 1).at(startPosY - 1);
			tile2 = this->arrayTiles.at(startPosX - 1).at(startPosY);
			tile3 = this->arrayTiles.at(startPosX + 1).at(startPosY - 1);
			tile4 = this->arrayTiles.at(startPosX + 1).at(startPosY);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
		}
		
		if(startPosY < endPosY){
			tile1 = this->arrayTiles.at(startPosX - 1).at(startPosY + 1);
			tile2 = this->arrayTiles.at(startPosX - 1).at(startPosY);
			tile3 = this->arrayTiles.at(startPosX + 1).at(startPosY + 1);
			tile4 = this->arrayTiles.at(startPosX + 1).at(startPosY);
			
			if((!tile1->getPassable() && tile2->getPassable()) || (!tile3->getPassable() && tile4->getPassable()) ){
				return false;
			}
		}
	}
	
	return true;
}

ASTile* AStar::getFather(int pointX, int pointY){
	return this->arrayTiles.at(pointX).at(pointY);
}
