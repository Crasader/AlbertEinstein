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


using namespace cocos2d::extension;



class NewsScene : cocos2d::CCLayer , public cocos2d::extension::CCListView::CCKeypadDelegate, public cocos2d::extension::CCListViewDelegate{
    

private:
    
    std::string  currentData ;
    
    
    void onHttpRequestCompleted(CCObject *pSender, void *data);
    void createListView();
    CCSprite * getImageFromURL(const char* url);
    void btnBack(CCObject* node);
    void parseResult(char *retorno);
    void printReturn(JsonValue o);
    void setValue(char* o);
    
    void CCListView_numberOfCells(CCListView *listView, CCListViewProtrolData *data);
    void CCListView_cellForRow(CCListView *listView, CCListViewProtrolData *data);
    void CCListView_didClickCellAtRow(CCListView *listView, CCListViewProtrolData *data);
    void CCListView_didScrollToRow(CCListView *listView, CCListViewProtrolData *data);

public:
    
    
    CCListView * list;
    
    AnimatedSprite *loading;
    
    NewsScene();
   // ~NewsScene();
//    virtual bool onDraw(cocos2d::CCTextFieldTTF * pSender);
//    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
//    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
//    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
//    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
//    
//    void btnOkCallBack(CCObject *sender);
//    void OnBack();
};

#endif
