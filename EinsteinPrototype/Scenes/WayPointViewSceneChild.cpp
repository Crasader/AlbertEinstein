//
//  WayPointViewSceneChild.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 02/04/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "WayPointViewSceneChild.h"
void WayPointViewChildScene::createLayerChild(int wayPoint, bool *infoOpenRef)
{
  StateMachine stateMachinePreviousPage;
  stateMachinePreviousPage.lastState =  TARGET_PATH_FINDER_PAGE;
  new WayPointViewChildScene(wayPoint, infoOpenRef);
}

WayPointViewChildScene::WayPointViewChildScene(int wayPoint, bool *infoOpenRef)
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(this,10,Global::TAG_Child_WayPoint);
    this->wayPoint = wayPoint;
    InitView();
	this->infoOpen = infoOpenRef;
}

void WayPointViewChildScene::OnBack()
{
    CCLayer* parent =  ((CCLayer*)this->getParent());
	*infoOpen = false;
	//delete(infoOpen);
    parent->removeChildByTag(Global::TAG_Child_WayPoint, true);
    CCApplication::sharedApplication().hideBars(true);
}

void WayPointViewChildScene::LoadWayPoint()
{
    CCNode* node = this->getChildByTag(250);
    char s[20] ;
    sprintf(s, "%d.png", wayPoint);

    CCSprite *sprtEstabelecimento = CCSprite::create(s);
    if( sprtEstabelecimento == 0)
        sprtEstabelecimento = CCSprite::create("ModeloWayPointFoto.png");

    sprtEstabelecimento->setAnchorPoint(ccp(0,0));
    sprtEstabelecimento->setPosition(ccp(0,215));
    node->addChild(sprtEstabelecimento,5,645);
    
    CCLabelTTF *labelTitle;
    labelTitle = CCLabelTTF::create(LoadWayPointName(this->wayPoint), "Lucida Grande", 16);
    labelTitle->setPosition(ccp(30, 180));
    labelTitle->setAnchorPoint(ccp(0,0));
    labelTitle->setColor(ccc3(0, 0, 0));
    node->addChild(labelTitle,5,641);
}

void WayPointViewChildScene::InitView()
{
    CCLayerColor* c = CCLayerColor::create(ccc4(255,255,255,255));
    CCSprite* TitleImg = CCSprite::create(Global::TOP_SRC_IMAGE_Especialidade);
    TitleImg->setAnchorPoint(ccp(0, 0));
    TitleImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height - TitleImg->boundingBox().size.height-20));
    this->addChild(c,0,3);
    this->addChild(TitleImg,1,2);

    initFixedMenu();

    CCLayerColor *node = CCLayerColor::create(ccc4(0,0,0,0));
    addChild(node,0, 250);

    LoadWayPoint();
    ChangeTitle(LoadWayPointName(wayPoint));
    addButtonTop();
} 