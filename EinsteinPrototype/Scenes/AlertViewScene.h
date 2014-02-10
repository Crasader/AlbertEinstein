//
//  AlertViewScene.h
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/14/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__AlertViewScene__
#define __EinsteinPrototype__AlertViewScene__

#include <iostream>


#include "Enumerations.h"
 
/**************************************************************
 **** Exibe Popup de Rota e Estacionamento                  ***
 **************************************************************/
class AlertViewScene  {
private:

     public:
      static bool isActive  ;
    
    static void showMessageRota(cocos2d::CCNode* sender, cocos2d::SEL_MenuHandler callbackFunction  , int wayPoint);
    static void showMessageParking
    (cocos2d::CCNode* sender, cocos2d::SEL_MenuHandler callbackFunction  , int wayPoint);
     static void showMessageParkingOption(cocos2d::CCNode* parent, cocos2d::SEL_MenuHandler entering,cocos2d::SEL_MenuHandler outing,cocos2d::SEL_MenuHandler quiting);
    
    void btnCloseAlert(cocos2d::CCObject *sender);

 
    
};


#endif /* defined(__EinsteinPrototype__AlertViewScene__) */
