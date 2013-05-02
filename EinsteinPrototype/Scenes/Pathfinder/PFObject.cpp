//
//  PFObject.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "PFObject.h"

PFObject::PFObject(int ID, CCString *name){
    this->ID = ID;
    this->name = name;
}

PFObject::~PFObject(){
        
}

int PFObject::getID(){
    return this->ID;
}

CCString *PFObject::getName(){
    return this->name;
}

PFObject *PFObject::getFather(){
    return this->father;
}

void PFObject::setFather(PFObject *father){
    this->father = father;
}