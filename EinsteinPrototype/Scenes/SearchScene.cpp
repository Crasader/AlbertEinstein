//
//  SearchScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 20/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.

#include "SearchScene.h"
#include "LayerWebView.h"

using namespace cocos2d;

bool SearchScene::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
    this->cursorLabel->setVisible(true);
    
    if(__pKeyboardNotificationLayer->IsCleanUp)
    {
         _txtBusca->setContentSize(CCSizeMake(800, 40));
        __pKeyboardNotificationLayer->IsCleanUp  = false;
        this->currentData.clear();
    }
    
    return false;
}

void SearchScene::OnBack()
{
    _txtBusca->detachWithIME();
    this->returnToHome();
}

bool SearchScene::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    this->cursorLabel->setVisible(false);
    if(__pKeyboardNotificationLayer->IsCleanUp)
    {
         _txtBusca->setContentSize(CCSizeMake(800, 40));
        __pKeyboardNotificationLayer->IsCleanUp  = false;
        this->currentData.clear();
    }
    return false;
}

bool SearchScene::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    if(currentData.length() > 18)
        return true;

        currentData.insert(currentData.end(), nLen, *text);
    return false;
}

    
bool SearchScene::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
     currentData = currentData.substr(0,currentData.size() - nLen);
     return false;
}

SearchScene::SearchScene()
{
      setUpTopImage(Global::TOP_SRC_IMAGE_Procurar);
      initFixedMenu(PROCURAR);
      setTouchEnabled(true);

      currentData = " ";
      CCSprite* TitleImg = CCSprite::create("TextBox_Fundo.png");
      _txtBusca = CCTextFieldTTF::textFieldWithPlaceHolder(currentData.c_str(),  "Lucida Grande", 15);
      __pKeyboardNotificationLayer = new KeyboardNotificationLayer();
      
      TitleImg->addChild(_txtBusca);
      TitleImg->addChild(__pKeyboardNotificationLayer );

      TitleImg->setAnchorPoint(ccp(0, 0));
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();

    if (winsize.height > 480) {
         TitleImg->setPosition(ccp(10, 440));
    }
    else
    {
         TitleImg->setPosition(ccp(10, 360));
    }

    
      
      _txtBusca->setColor(ccc3(0, 0, 0));
      _txtBusca->setDelegate(this);
      _txtBusca->setVisible(true);
      _txtBusca->setAnchorPoint(ccp(0,0));
      _txtBusca->setPosition(ccp(5,7));
      _txtBusca->setContentSize(CCSizeMake(800, TitleImg->boundingBox().size.height));
      _txtBusca->setColorSpaceHolder(ccc3(255,0,0));
 
     this->addButtonTop(PROCURAR);
      _txtBusca->setColor(ccc3(0,0,0));
      __pKeyboardNotificationLayer->setAnchorPoint(ccp(0,0));
      __pKeyboardNotificationLayer->setTextField(_txtBusca);
      __pKeyboardNotificationLayer->autorelease();
      __pKeyboardNotificationLayer->setContentSize(TitleImg->boundingBox().size);
      this->addChild(TitleImg,2,2);

      m_pTextFieldAction = cocos2d::CCRepeatForever::create((cocos2d::CCActionInterval*)cocos2d::CCSequence::create(cocos2d::CCFadeOut::create(0.25),cocos2d::CCFadeIn::create(0.25),0));
      m_pTextFieldAction->retain();
      cursorLabel = CCLabelTTF::create("|","Helvetica", 32);
      cursorLabel->setVisible(false);
      cursorLabel->setColor(ccBLUE);
      cursorLabel->setPosition(ccp(0,0));
      TitleImg->addChild(cursorLabel,0);
      cursorLabel->runAction(m_pTextFieldAction);
      
      CCMenu* mnBusca = CCMenu::create();
      createMenuItem(mnBusca, 302, "Busca_Button.png", "Busca_Button_H.png", 0, 0, menu_selector(SearchScene::btnBusca), this);
    
    if (winsize.height > 480) {
       mnBusca->setPosition(ccp(250, 445));
    }
    else
    {
       mnBusca->setPosition(ccp(250, 365));
    }

    
      this->addChild(mnBusca,0);
      btnBusca(0);
}

bool SearchScene::onDraw(CCTextFieldTTF * pSender)
{
    
    //Desenha um cursor caso esteja em modo de edição de texto.
    if(cursorLabel->isVisible())
    {
        if(strcmp(currentData.c_str(), " ") == 0)
            cursorLabel->setPositionX( 12);
        else
            cursorLabel->setPositionX( pSender->getTextDimensions()+5);
    
        cursorLabel->setPositionY(18);
    }
    return false;
}

void SearchScene::btnBusca(CCObject *sender)
{
    currentData = ((CCTextFieldTTF*)__pKeyboardNotificationLayer->__pTrackNode)->getContentText();

    CCSize size =  CCDirector::sharedDirector()->getWinSize();
    if (size.height > 480) {
        size.height= 300;
    }
    else
    {
       size.height= 200;
    }
    

    
    //size.width = 0.75*size.width;
     size.width = 0.90*CCDirector::sharedDirector()->getWinSize().width;
    
    
    if(this->getChildByTag(Global::TAG_Child_Layer)!=NULL)
    {
        ((cocos2d::extension::CCListView*)getChildByTag(Global::TAG_Child_Layer))->cleanup();
        this->removeChildByTag(Global::TAG_Child_Layer, true);
    }
    
    SearchItem(((CCTextFieldTTF*)__pKeyboardNotificationLayer->__pTrackNode)->getContentText());
   
    //Reinicializa os items (ListView ) com novos dados
    this->initListView(size, this);
    
    //Ajusta o fundo botao para vermelho 1-vermelho 2- amarelo - Default azul
    pList->BackgroundID = 1;
}

void SearchScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void SearchScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    //Monta a linha com o padrao default
    setDefaultListView(listView,data,0);
}

void SearchScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    StateMachine _statePreviousPage;
    
    _statePreviousPage.lastState = TARGET_SEARCH;
     targetWayPoint = plistMenuCurrent[data->nRow].key;
    
    //Muda a cena para a Rota
     changeScene(new TraceRouteScene(targetWayPoint,_statePreviousPage));
}

void SearchScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    
}

SearchScene::~SearchScene()
{ 
    m_pTextFieldAction->release();
    cursorLabel->release();
    _txtBusca->release();
    __pKeyboardNotificationLayer->release();
}