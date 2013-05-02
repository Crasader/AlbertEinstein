//
//  ASTile.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/29/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "ASTile.h"

ASTile::ASTile(){
	this->passable = true;
    this->pointX = -1;
	this->pointY = -1;
	this->calcF = 0;
	this->valueG = 0;
	this->calcG = 0;
	this->calcH = 0;
	
	this->fatherPointX = -1;
	this->fatherPointY = -1;
}

bool ASTile::getPassable(){
    return this->passable;
}

void ASTile::setPassable(bool value){
    this->passable = value;
}

int ASTile::getPointX(){
    return this->pointX;
}

void ASTile::setPointX(int value){
    this->pointX = value;
}

int ASTile::getPointY(){
    return this->pointY;
}

void ASTile::setPointY(int value){
    this->pointY = value;
}

int ASTile::getCalcF(){
    return this->calcF;
}

void ASTile::setCalcF(int value){
    this->calcF = value;
}

int ASTile::getValueG(){
    return this->valueG;
}

void ASTile::setValueG(int value){
    this->valueG = value;
}

int ASTile::getCalcG(){
    return this->calcG;
}

void ASTile::setCalcG(int value){
    this->calcG = value;
}

int ASTile::getCalcH(){
    return this->calcH;
}

void ASTile::setCalcH(int value){
    this->calcH = value;
}

int ASTile::getFatherPointX(){
    return this->fatherPointX;
}

void ASTile::setFatherPointX(int value){
    this->fatherPointX = value;
}

int ASTile::getFatherPointY(){
    return this->fatherPointY;
}

void ASTile::setFatherPointY(int value){
    this->fatherPointY = value;
}

