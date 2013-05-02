//
//  SearchScene.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 20/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__SearchScene__
#define __EinsteinPrototype__SearchScene__

#include <iostream>
#include "enumerations.h"
#include "CCTextFieldTTF.h"
#include "TraceRouteScene.h"
class SearchScene : public IFixedMenu, public cocos2d::CCTextFieldDelegate, public cocos2d::extension::CCListViewDelegate 
{
private:
    
    CCRepeatForever* m_pTextFieldAction;
    CCLabelTTF* cursorLabel;
    CCTextFieldTTF* _txtBusca;
    KeyboardNotificationLayer*	__pKeyboardNotificationLayer = 0;
    std::string  currentData ;
    
public:
    SearchScene();
    ~SearchScene();
    void btnCleanTextBox(CCObject *sender);
    virtual bool onDraw(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    void  btnBusca(CCObject *sender);
    
    void OnBack();
   };

 #endif /* defined(__EinsteinPrototype__SearchScene__) */
