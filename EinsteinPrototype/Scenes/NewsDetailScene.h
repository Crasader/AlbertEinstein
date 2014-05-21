//
//  NewsScene.h
//  EinsteinPrototype
//
//  Created by Camargo on 02/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_NewsDetailScene_h
#define EinsteinPrototype_NewsDetailScene_h

#include <iostream>
#include "enumerations.h"
#include "CCTextFieldTTF.h"
#include "TraceRouteScene.h"
#include "CCHttpRequest.h"
#include "extensions/CCListView/CCListView.h"
#include "AnimatedSprite.h"
#include "gason.h"
#include "News.h"


using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class NewsDetailScene : CCLayer{
    
private:
    string currentData;
    
    void onHttpRequestCompleted(CCObject *pSender, void *data);
    CCSprite* getImageFromURL(const char* url);
    void btnBack(CCObject *node);

    // Touch Handlers

public:
    
    NewsDetailScene();
    void btnMore(CCObject* node);

    
     CCListView *baseLayer;
    void initWithNews(News * varNews);
    News * news;
};

#endif

