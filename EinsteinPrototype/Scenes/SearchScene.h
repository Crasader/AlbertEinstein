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
    
    cocos2d::CCRepeatForever* m_pTextFieldAction;
    cocos2d::CCLabelTTF* cursorLabel;
    cocos2d::CCTextFieldTTF* _txtBusca;
    KeyboardNotificationLayer*	__pKeyboardNotificationLayer = 0;
    std::string  currentData ;
    
public:
    
    SearchScene();
    ~SearchScene();
    void pesquisar();
    void btnCleanTextBox(CCObject *sender);
    virtual bool onDraw(cocos2d::CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * sender);
    virtual void CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
    virtual void CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data);
  
    void  btnBusca(CCObject *sender);
    void btnOkCallBack(CCObject *sender);
    
    void OnBack();
   };

 #endif /* defined(__EinsteinPrototype__SearchScene__) */
