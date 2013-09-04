//
//  AlertViewScene.cpp
//  EinsteinPrototype
//
//  Created by Marco Rossi on 2/14/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//
#include "AlertViewScene.h"

using namespace cocos2d;

bool AlertViewScene::isActive =  false;

void AlertViewScene::showMessageRota(CCNode* sender, SEL_MenuHandler callbackFunction ,int wayPoint)
{
    if( AlertViewScene::isActive ==  true)
        return;
    AlertViewScene::isActive = true;

    //Backgroud Alert
    CCSprite *sprBackground = CCSprite::create("popup.png");
    sprBackground->setAnchorPoint(ccp(0,0));
    sprBackground->setScale(1);
    sprBackground->setPosition(ccp(25,100));
    
    //Layer - Container with all elements of Alert
    CCLayerColor *node = CCLayerColor::create(ccc4(0,0,0,255));
    node->setContentSize(CCDirector::sharedDirector()->getWinSize());
    node->addChild(sprBackground);
    node->setOpacity(200);
    node->setAnchorPoint(ccp(0,0));
    node->setPosition(ccp(0,0));

    //Close Button of frame
    CCSprite *btnClose = CCSprite::create("fechar_btn.png");
    CCSprite *btnClosePress = CCSprite::create("fechar_press.png");

    CCSprite *Icone = CCSprite::create("icone_horario.png");
    Icone->setPosition(ccp(45,225));
    node->addChild(Icone);
    
    CCMenuItemSprite *itembtnClose = CCMenuItemSprite::create(btnClose, btnClosePress, node, menu_selector(AlertViewScene::btnCloseAlert));
	itembtnClose->setAnchorPoint(ccp(0, 0));
	itembtnClose->setPosition(ccp(280,303));
    
    //Button Rota para o local
    CCSprite *btnSelect = CCSprite::create("Rota_btn.png");
    CCSprite *btnUnselect = CCSprite::create("Rota_press.png");
    
    CCMenuItemSprite *itembtnRota = CCMenuItemSprite::create( btnSelect,btnUnselect, node,callbackFunction);
	itembtnRota->setAnchorPoint(ccp(0, 0));
	itembtnRota->setPosition(ccp(80,150));
    //Button Rota para o local
    CCMenu* options = CCMenu::create();
    options->addChild(itembtnClose,0,44);
    options->addChild(itembtnRota,0,wayPoint);
    options->setPosition(ccp(0, -20));
    node->addChild(options,3,Global::TAG_Child_Layer);

    //Title of Alert
    CCLabelTTF *labelTitle;
    labelTitle = CCLabelTTF::create(IFixedMenu::LoadWayPointName(wayPoint), "LucidaGrandeBold", 16);
    labelTitle->setPosition(ccp(35, 260));
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(ccc3(55, 55, 55));
    node->addChild(labelTitle);
    
    CCLabelTTF *labelSubTitle;
    labelSubTitle = CCLabelTTF::create("Horário de funcionamento", "LucidaGrandeBold", 11);
    labelSubTitle->setPosition(ccp(60, 225));
    labelSubTitle->setAnchorPoint(ccp(0,0));
    labelSubTitle->setColor(ccc3(55, 55, 55));
    node->addChild(labelSubTitle);
    
    //Description of Alert
    CCLabelTTF *labelDescription;
    labelDescription = CCLabelTTF::create("Estamos aberto todos os dias das 9h as 24h*", "Lucida Grande", 10);
    labelDescription->setPosition(ccp(60, 210));
    labelDescription->setAnchorPoint(ccp(0,0));
    labelDescription->setColor(ccc3(55, 55, 55));
    node->addChild(labelDescription);
    
    ((CCLayerColor*)sender)->setOpacity(30);
    CCDirector::sharedDirector()->getRunningScene()->addChild(node,10,Global::TAG_Alert_Layer);
}


void AlertViewScene::showMessageParking(CCNode* parent, SEL_MenuHandler callbackFunction  ,int wayPoint)
{
    if( AlertViewScene::isActive ==  true)
        return;
    
    AlertViewScene::isActive = true;
    //Layer - Container with all elements of Alert
    CCLayerColor *node = CCLayerColor::create(ccc4(0,0,0,255));
    node->setContentSize(CCDirector::sharedDirector()->getWinSize());
    node->setOpacity(200);
    node->setAnchorPoint(ccp(0,0));
    node->setPosition(ccp(0,0));

    
    //Fundo do Popup
    CCSprite *sprBackground = CCSprite::create("popup.png");
    sprBackground->setAnchorPoint(ccp(0,0));
    sprBackground->setScale(1);
    sprBackground->setPosition(ccp(25,100));
    node->addChild(sprBackground);

    //Close Button of frame
    CCSprite *btnClose = CCSprite::create("fechar_btn.png");
    CCSprite *btnClosePress = CCSprite::create("fechar_press.png");

    CCSprite *Icone = CCSprite::create("icone_estacionamento.png");
    Icone->setPosition(ccp(45,225));
    node->addChild(Icone);
    
    CCMenuItemSprite *itembtnClose = CCMenuItemSprite::create(btnClose, btnClosePress, node, callbackFunction);
	itembtnClose->setAnchorPoint(ccp(0, 0));
	itembtnClose->setPosition(ccp(280,303));
    
    //Button Rota para o local
    CCSprite *btnSelect = CCSprite::create("ok_btn.png");
    CCSprite *btnUnselect = CCSprite::create("ok_press.png");
    
    CCMenuItemSprite *itembtnRota = CCMenuItemSprite::create( btnSelect,btnUnselect, node,callbackFunction);
	itembtnRota->setAnchorPoint(ccp(0, 0));
	itembtnRota->setPosition(ccp(115,150));
    //Button Rota para o local
    CCMenu* options = CCMenu::create();
    options->addChild(itembtnClose,0,44);
    options->addChild(itembtnRota,0,wayPoint);
    options->setPosition(ccp(0, -20));
    node->addChild(options,3,Global::TAG_Child_Layer);
    
    //Title of Alert
    CCLabelTTF *labelTitle;
    labelTitle = CCLabelTTF::create("Alerta", "LucidaGrandeBold", 16);
    labelTitle->setPosition(ccp(35, 260));
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(ccc3(55, 55, 55));
    node->addChild(labelTitle);
    
    
    //sub-Title of Alert
    CCLabelTTF *labelSubTitle;
    labelSubTitle = CCLabelTTF::create("Atenção", "LucidaGrandeBold", 11);
    labelSubTitle->setPosition(ccp(60, 225));
    labelSubTitle->setAnchorPoint(ccp(0,0));
    labelSubTitle->setColor(ccc3(55, 55, 55));
    node->addChild(labelSubTitle);

    //Description of Alert
    CCLabelTTF *labelDescription;
    labelDescription = CCLabelTTF::create("       Ao entrar por este bloco você só poderá retirar", "Lucida Grande", 10);
    labelDescription->setPosition(ccp(40, 210));
    labelDescription->setAnchorPoint(ccp(0,0));
    labelDescription->setColor(ccc3(55, 55, 55));
    node->addChild(labelDescription);
    
    //Description of Alert
    CCLabelTTF *labelDescription2;
    labelDescription2 = CCLabelTTF::create("seu veículo neste mesmo bloco.", "Lucida Grande", 10);
    labelDescription2->setPosition(ccp(35, 200));
    labelDescription2->setAnchorPoint(ccp(0,0));
    labelDescription2->setColor(ccc3(55, 55, 55));
    node->addChild(labelDescription2);


    parent->addChild(node,10,Global::TAG_Alert_Layer);
}
/******************************************************************************************
 *                                                                                        *
 ******************************************************************************************/
void AlertViewScene::btnCloseAlert(CCObject *sender)
{
    CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Global::TAG_Alert_Layer,true);
    AlertViewScene::isActive = false;
}
 