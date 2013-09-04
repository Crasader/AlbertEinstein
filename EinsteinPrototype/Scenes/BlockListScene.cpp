//
//  BlockListScene.cpp
//  EinsteinPrototype
//
//  Created by Lucas Bittencourt on 7/4/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//


//
//  BathRoomScene.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 21/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "BlockListScene.h"
#include "HomeScene.h"
#include "WayPointViewSceneChild.h"

bool* active_;

void BlockListScene::createLayerChild(int blockID, bool *infoOpenRef, bool* active)
{
  *active = false;
  active_ = active;
  StateMachine stateMachinePreviousPage;
  stateMachinePreviousPage.lastState =  ACTUAL_BUILDING;
  new BlockListScene(blockID, active);
}

BlockListScene::BlockListScene(int blockID, bool* active)
{
	this->blockID = blockID;
    state = TARGET_CATEGORY;
    actualCategory = BANHEIROS;
    initFixedMenu();
    Init();
    LoadListView();
    addButtonTop();
	
	CCDirector::sharedDirector()->getRunningScene()->addChild(this,10,Global::TAG_Child_WayPoint);
}

BlockListScene::~BlockListScene()
{
	*active_ = true;
}

void BlockListScene::setUpMenu()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    size.height= 0.80*size.height-20;
    size.width = 0.90*size.width;
    
    initListView(size, this);
    pList->setPosition(ccp(20,51));
}

void BlockListScene::Init()
{
    setUpTopImage(Global::TOP_SRC_IMAGE_Banheiro);
}

void BlockListScene::LoadListView()
{
    ///444 CategoriaID para Banheiros
    BuildCategoryState(this->blockID);
}

void BlockListScene::CCListView_numberOfCells(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    data->nNumberOfRows = this->plistMenuCurrent.size();
}

void BlockListScene::BuildCategoryState(int serviceID)
{
    //if( serviceID == 444)
	ChangeTitle(SERVICOS);
    BuildWayPointByBuilding(serviceID);
    setUpMenu();
}

//Retorna para a tela inicial Home
void BlockListScene::OnBack()
{
	if(secondScreen) {
		secondScreen = false;
		LoadListView();
		return;
	}
	*active_ = true;
    //returnToHome();
	CCLayer* parent =  ((CCLayer*)this->getParent());
	//*infoOpen = false;
	//delete(infoOpen);
    parent->removeChildByTag(Global::TAG_Child_WayPoint, true);
    //CCApplication::sharedApplication().hideBars(true);
}

void BlockListScene::CCListView_cellForRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
    setDefaultListView(listView, data, 0);
}

void BlockListScene::CCListView_didClickCellAtRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
	if(secondScreen) {
		bool* e = new bool(false);
		WayPointViewChildScene::createLayerChild(plistMenuCurrent[data->nRow].key,e);
		return;
	}

	BuildWayPointState(plistMenuCurrent[data->nRow].key, false, 0);
	secondScreen = true;
}

void BlockListScene::CCListView_didScrollToRow(cocos2d::extension::CCListView *listView, cocos2d::extension::CCListViewProtrolData *data)
{
	
}

 


/*#include "BlockListScene.h"
#include "cocos2d.h"

using namespace cocos2d;

void BlockListScene::createLayerChild(int wayPoint, bool *infoOpenRef)
{
  StateMachine stateMachinePreviousPage;
  stateMachinePreviousPage.lastState =  TARGET_PATH_FINDER_PAGE;
  new BlockListScene(wayPoint, infoOpenRef);
}

BlockListScene::BlockListScene(int wayPoint, bool *infoOpenRef)
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(this,10,Global::TAG_Child_WayPoint);
    this->wayPoint = wayPoint;
    InitView();
	this->infoOpen = infoOpenRef;
}

void BlockListScene::OnBack()
{
    CCLayer* parent =  ((CCLayer*)this->getParent());
	*infoOpen = false;
	//delete(infoOpen);
    parent->removeChildByTag(Global::TAG_Child_WayPoint, true);
    //CCApplication::sharedApplication().hideBars(true);
}

void BlockListScene::LoadWayPoint()
{
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    size.height= 0.60*size.height-20;
    size.width = 0.90*size.width;
	
	insertItemListView(0, "Teste1");
	insertItemListView(1, "Teste2");
	insertItemListView(2, "Teste3");
	
    initListView(size, this);
    pList->setPosition(ccp(20,141));
	
	insertLabel(ccc3(72, 72, 72), ccp(20, 180), true, "Bloco A1", pList, 16);
	insertLabel(ccc3(72, 72, 72), ccp(20, 180), true, "Bloco B", pList, 16);
	insertLabel(ccc3(72, 72, 72), ccp(20, 180), true, "Bloco D/E", pList, 16);
	
    /*CCNode* node = this->getChildByTag(250);
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
    node->addChild(labelTitle,5,641); // AQUI
}

void BlockListScene::InitView()
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
} */