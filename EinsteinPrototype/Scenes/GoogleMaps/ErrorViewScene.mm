//
//  ErrorViewScene.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 25/04/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "ErrorViewScene.h"


ErrorViewScene::ErrorViewScene()
{
    ShowPage();
}

void ErrorViewScene::ShowPage()
{
    cocos2d::CCSprite *sprtFundoEstabelecimento = cocos2d::CCSprite::create("Logo.png");
    sprtFundoEstabelecimento->setAnchorPoint(ccp(0,0));
    sprtFundoEstabelecimento->setPosition(ccp(0, 180));
    this->addChild(sprtFundoEstabelecimento);
}
