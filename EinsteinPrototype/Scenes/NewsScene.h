//
//  NewsScene.h
//  EinsteinPrototype
//
//  Created by Camargo on 02/05/14.
//  Copyright (c) 2014 Farofa Studios. All rights reserved.
//

#ifndef EinsteinPrototype_NewsScene_h
#define EinsteinPrototype_NewsScene_h

#include <iostream>
#include "enumerations.h"
#include "CCTextFieldTTF.h"
#include "TraceRouteScene.h"
#include "CCHttpRequest.h"
#include "extensions/CCListView/CCListView.h"
#include "AnimatedSprite.h"
#include "BrowserScene.h"
#include "gason.h"
#include "News.h"
#include "NewsDetailScene.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class NewsScene : CCLayer , public CCListView::CCKeypadDelegate, public CCListViewDelegate {
    
private:
    string currentData;
    vector<News *> listNews;
    
    void onHttpRequestCompleted(CCObject *pSender, void *data);
    void createListView();
    CCSprite* getImageFromURL(const char* url);
    void btnBack(CCObject *node);
    void parseResult(char *retorno);
    void printReturn(JsonValue o);
    void setValue(string value);
    
    void CCListView_numberOfCells(CCListView *listView, CCListViewProtrolData *data);
    void CCListView_cellForRow(CCListView *listView, CCListViewProtrolData *data);
    void CCListView_didClickCellAtRow(CCListView *listView, CCListViewProtrolData *data);
    void CCListView_didScrollToRow(CCListView *listView, CCListViewProtrolData *data);

public:
    CCListView *list;
    AnimatedSprite *loading;
    
    NewsScene();
};

#endif
