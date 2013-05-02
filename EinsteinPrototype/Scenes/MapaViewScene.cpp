//
//  MapaViewScene.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 08/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//


#include "MapaViewScene.h"

#include "WayPointViewSceneChild.h"

void MapViewScene::releaseData()
{
    delete destination;
}

void MapViewScene::setUp()
{
    CCSprite* TitleImg = CCSprite::create("TopImgMap.png");
    TitleImg->setAnchorPoint(ccp(0, 0));
	TitleImg->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height - TitleImg->boundingBox().size.height));
    this->addChild(TitleImg,0,2);
    
    CCLayerColor *c = CCLayerColor::create(ccc4(255,255,255,255));
    CCSprite* spriteBackButton = CCSprite::create("BackgroundButtonsMap.png");
    spriteBackButton->setAnchorPoint(ccp(0, 0));
	spriteBackButton->setOpacity(2000);
    spriteBackButton->setPosition(ccp(0, 0));
    c->setContentSize(spriteBackButton->getContentSize());
    c->addChild(spriteBackButton);
    this->addChild(c,0,120);

    if(getCategory(targetWayPoint) != NONE)
      {
          RotasFileIO* pRotas = new RotasFileIO();
          this->isFavoritoChecked = pRotas->Exists(wayPoint);
          CCMenu* menu3 = CCMenu::create();
    
          char* chFavoritos = "favoritos_Mapa_btn.png";
    
          if( isFavoritoChecked)
              chFavoritos = "favoritos_Mapa_press.png";
          
          createMenuItem(menu3, 112233, chFavoritos, chFavoritos,0, 0, menu_selector(MapViewScene::btnTopButton), this);
          menu3->setPosition(ccp(250,423));
          menu3->setAnchorPoint(ccp(0,0));
          this->addChild(menu3,3,1002);

      }
}


MapViewScene::MapViewScene(int _wayPoint,int _targetWayPoint)
{
    this->infoOpen = false;
    this->touched = false;

    CCLOG("MapViewScene: WayPoint: %d  TargeWayPoint %d",_wayPoint, _targetWayPoint);
    Global::ULTIMO_PONTO_CHEGADA_WAYPOINT = _wayPoint;
    CCApplication::sharedApplication().hideBars(true);
    this->wayPoint = _wayPoint;
    targetWayPoint = _targetWayPoint;

	//black screen
	CCSprite *blackScreen = CCSprite::create("backInBlack.png");
	blackScreen->setScaleX(CCDirector::sharedDirector()->getWinSize().width / blackScreen->boundingBox().size.width);
	blackScreen->setScaleY(CCDirector::sharedDirector()->getWinSize().height / blackScreen->boundingBox().size.height);
	blackScreen->setAnchorPoint(ccp(0,0));
	this->addChild(blackScreen);
    
    Pathfinder *pathfinder = Pathfinder::create();
    pathfinder->setAnchorPoint(ccp(0, 0));
    pathfinder->setScale(1.0f);
    pathfinder->start(_wayPoint, _targetWayPoint);
    this->addChild(pathfinder,0,Global::TAG_Child_Layer);
    setTouchEnabled(true);

    CCMenu *menu = CCMenu::create();
    createMenuItem(menu, 111, "esquerda_Btn.png", "esquerda_Press.png", 0, 0, menu_selector(MapViewScene::btnMoveBack), menu);
    createMenuItem(menu, 222, "direita_Btn.png", "direita_Press.png", 68, 0, menu_selector(MapViewScene::btnMoveFoward), menu);
    menu->setAnchorPoint(ccp(0, 0));
	menu->setPosition(ccp(10, 10));
    this->addChild(menu,1,20);

    CCMenu* menu2 = CCMenu::create();
    createMenuItem(menu2, 123, "Seta1.png", "Seta2.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
    menu2->setPosition(ccp(10,440));
    menu2->setAnchorPoint(ccp(0,0));
    this->addChild(menu2,3,1000);
    this->destination = strdup(LoadWayPointName(_targetWayPoint));
    
    std::string cS = (std::string("") + destination).c_str();
    if( cS.length() > 28)
        cS = cS.substr(0,24) + "...";

    CCLabelTTF *labelTitle2;
    labelTitle2 = CCLabelTTF::create(cS.c_str(), CCSizeMake(300, 55),  kCCTextAlignmentLeft, "Lucida Grande", 11);
    labelTitle2->setPosition(ccp(154, 3));
    labelTitle2->setAnchorPoint(ccp(0,0.5));
    labelTitle2->setColor(ccc3(255, 255, 255));
    this->addChild(labelTitle2,3,999999);
    setUp();

}

void MapViewScene::setUpButtonTop()
{
    char* chFavoritos = "favoritos_Mapa_btn.png";
    
    if( isFavoritoChecked)
        chFavoritos = "favoritos_Mapa_press.png";
    
    CCMenu* menu3 = CCMenu::create();
    createMenuItem(menu3, 112233, chFavoritos, chFavoritos,0, 0, menu_selector(MapViewScene::btnTopButton), this);
    menu3->setPosition(ccp(250,423));
    menu3->setAnchorPoint(ccp(0,0));
 
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer)->removeChildByTag(1002, true);
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Global::TAG_Parent_Layer)->addChild(menu3,3,1002);
}

void MapViewScene::btnTopButton(CCObject *sender)
{
    RotasFileIO* pRotas = new RotasFileIO();
    isFavoritoChecked =!isFavoritoChecked;
    if( isFavoritoChecked)
    {
        RotaRow row;
        row.wayPoint = targetWayPoint;
        row.favorite = 0;
        sprintf(row.description, " %s",LoadWayPointName(targetWayPoint));
        pRotas->SaveRota(row);
    }
    else
        pRotas->DeleteRota(wayPoint);
    setUpButtonTop();
}

void MapViewScene::OnBack()
{
    
  
    returnToHome();
}

void MapViewScene::draw()
{
    CCLabelTTF *labelTitle;
    std::string cS;
    
    if( strcmp(((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() , "VOCÊ CHEGOU AO SEU DESTINO") ==0)
        cS = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() +std::string("\n") + destination ;
    else
        cS =  ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() +std::string(" em direcao a ") + destination ;
    labelTitle = CCLabelTTF::create(cS.c_str(), CCSizeMake(302, 100),  kCCTextAlignmentLeft, "Lucida Grande", 16);
    labelTitle->setPosition(ccp(15, 370));
    labelTitle->setAnchorPoint(ccp(0,0.5));
    labelTitle->setColor(ccc3(0, 0, 0));
    
    if(this->getChildByTag(80) != NULL)
        this->removeChildByTag(80, true);
    
    this->addChild(labelTitle,3,80);
    
    
    Pathfinder *pathfinder = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) );
    char buffer [50];
    
    if( pathfinder->getTotalStep() > 0)
        sprintf(buffer,"Passo %d de %d", pathfinder->getStep(),pathfinder->getTotalStep());
    else
        sprintf(buffer," Aguarde Carregando ");

    CCLabelTTF *l = CCLabelTTF::create(buffer,"LucidaGrandeBold", 13);
    l->setPosition(ccp(80,455));
    l->setAnchorPoint(ccp(0,0.5));
    l->setColor(ccc3(255, 255, 255));
    
    if(this->getChildByTag(9090) != NULL)
        this->removeChildByTag(9090, true);
    this->addChild(l,3,9090);
}


void MapViewScene::btnMoveFoward(CCObject *sender)
{
    CCNode* node = ((CCNode*)sender)->getParent()->getParent()->getChildByTag(200);
    ((Pathfinder*)node)->step(-1, false);
}

void MapViewScene::btnMoveBack(CCObject *sender)
{
    CCNode* node = ((CCNode*)sender)->getParent()->getParent()->getChildByTag(200);
    ((Pathfinder*)node)->step(1, false);
}

void MapViewScene::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint location = touch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	
    /*CCSize winSize = CCDirector::sharedDirector()->getWinSize();
     if(location.x > winSize.width/2 - 200 && location.x < winSize.width/2 - 50 &&
     location.y > winSize.height/2 - 50 && location.y < winSize.height/2 + 50){
     ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer))->step(1, false);
     }
     
     if(location.x > winSize.width/2 + 50 && location.x < winSize.width/2 + 200 &&
     location.y > winSize.height/2 - 50 && location.y < winSize.height/2 + 50){
     ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer))->step(-1, false);
     }*/
	
	//MOVE MAP
	if(!infoOpen){
		this->touched = true;
		this->touchLocation = location;
	}
}

void MapViewScene::ccTouchesMoved(CCSet *touches, CCEvent *event){
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint location = touch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	
	if(this->touched)
    {
		Pathfinder *p = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer));
		p->setPosition(ccpAdd(p->getPosition(), ccp(location.x - this->touchLocation.x, location.y - this->touchLocation.y)));
		this->touchLocation = location;
	}
}

void MapViewScene::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint location = touch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	
	this->touched = false;
    
	if(!infoOpen){
        
		Pathfinder *p = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer));
		CCArray *icons = p->getArrayIcons();
		
		if(icons){
			int count = icons->count();
			if(count > 0){
				for(int i = 0; i < count; i++){
					CCSprite *icon = (CCSprite *)icons->objectAtIndex(i);
					int iconTag = icon->getTag();
					
					if(iconTag > 0){
						CCPoint iconPosition = p->convertToWorldSpace(icon->getPosition());
						CCPoint iconPositionFinal = ccpRotateByAngle(iconPosition, p->getAnchorPoint(), p->getRotation());
                        
						if(location.x > iconPosition.x - icon->boundingBox().size.width/2 && location.x < iconPosition.x + icon->boundingBox().size.width/2 &&
						   location.y > iconPosition.y - icon->boundingBox().size.height/2 && location.y < iconPosition.y + icon->boundingBox().size.height/2){
							infoOpen = true;
							WayPointViewChildScene::createLayerChild(iconTag, &infoOpen);
							break;
						}
					}
				}
			}
		}
	}
	
}