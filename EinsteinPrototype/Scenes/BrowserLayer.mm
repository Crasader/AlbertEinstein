//
//  BrowserLayer.m
//  EinsteinPrototype
//
//  Created by Camargo on 12/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#include "BrowserLayer.h"
#include "UIWebViewBridge.h"
#include "HomeScene.h"
#include "curl.h"
static UIWebViewBridge *pUIWebViewBridge=0;


BrowserLayer::BrowserLayer()
{
     setUpMenu();
}


void BrowserLayer::openBrowserWithUrl(char* turl)
{
    cocos2d::CCSpriteFrameCache* sprite_cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    sprite_cache->addSpriteFramesWithFile("loading.plist");
    
    pUIWebViewBridge = [[UIWebViewBridge alloc] init];
    [pUIWebViewBridge setStartLayerWebViewWithURL:this andUrl:turl];

}


void BrowserLayer::setUpMenu()
{
        // CCSize size = CCSize(750, 400);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        
        CCLayerColor * fundo =  CCLayerColor::create(ccc4(255, 255, 255, 255), size.width, size.height);
        fundo->setAnchorPoint(ccp(0,0));
        fundo->setPosition(ccp(0, 0));
        this->addChild(fundo,10);
        
        
        CCSprite *sprHeader;
        sprHeader = CCSprite::create("Header_Blue.png");
        
        sprHeader->setAnchorPoint(ccp(0,0));
        sprHeader->setScale(1);
        sprHeader->setPosition(ccp(0, size.height -90));
        this->addChild(sprHeader,100);
        CCSprite *sprTitle;
        sprTitle = CCSprite::create("tit_comochegar.png");
        
        sprTitle->setAnchorPoint(ccp(0,0));
        sprTitle->setScale(1);
        sprTitle->setPosition(ccp(110, size.height -50));
        this->addChild(sprTitle,101);
        
        
        CCMenu* menu = CCMenu::create();
        
        CCSprite *spriteDefault = CCSprite::create("Seta1.png");
        CCSprite *spriteSelected = CCSprite::create("Seta2.png");
      //  CCMenuItemSprite *item = CCMenuItemSprite::create(spriteDefault, spriteSelected, this,  menu_selector(BrowserLayer::goBack));
//        item->setAnchorPoint(ccp(0, 0));
//        item->setPosition(ccp(0, size.height -90));
//        menu->addChild(item, 0, 100);
//        menu->setPosition(ccp(10,30));
//        menu->setAnchorPoint(ccp(0,0));
//        this->addChild(menu,102);
    
        
        this->loading = new AnimatedSprite("loader_einstein1.png");
        this->loading->addAnimation(AnimatedSprite::animationWithFile("loader_einstein", 24, 0.030f), "anim");//frame_count
        
        this->addChild(this->loading,999);
        this->loading->runAnimation("anim",true, true);
        this->loading->setAnchorPoint(ccp(0.5f, 0.5f));
        this->loading->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 - 30));
        this->loading->setVisible(true);
        
 
}
// void BrowserScene::goBack()
//{
//    
//    this->removeFromParentAndCleanup(true);
//}


BrowserLayer::~BrowserLayer()
{
    [pUIWebViewBridge release];
}



