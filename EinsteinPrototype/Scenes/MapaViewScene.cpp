//
//  MapaViewScene.cpp
//  EinsteinPrototype
//
//  Created by Anderson Gomes on 08/03/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//


#include "MapaViewScene.h"

#include "WayPointViewSceneChild.h"

#include <stdexcept>

bool loaded = 0;
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
    c->setPosition(ccp(0, 0));
    this->addChild(c,0,120);

    if(getCategory(targetWayPoint) != NONE)
      {
          RotasFileIO* pRotas = new RotasFileIO();
         // estava verificando o ponto de partida nos favoritos e por isso nunca vinha marcado.
          // this->isFavoritoChecked = pRotas->Exists(wayPoint);
          this->isFavoritoChecked = pRotas->Exists(targetWayPoint);
          CCMenu* menu3 = CCMenu::create();
    
          char* chFavoritos = "favoritos_Mapa_btn.png";
    
          if( isFavoritoChecked)
              chFavoritos = "favoritos_Mapa_press.png";
          
          createMenuItem(menu3, 112233, chFavoritos, chFavoritos,0, 0, menu_selector(MapViewScene::btnTopButton), this);
          CCSize winsize = CCDirector::sharedDirector()->getWinSize();
          
          if (winsize.height > 480) {
              menu3->setPosition(ccp(250,490));
          }
          else
          {
               menu3->setPosition(ccp(250,400));
          }

         
          menu3->setAnchorPoint(ccp(0,0));
          this->addChild(menu3,3,1002);

      }
}


MapViewScene::MapViewScene(int _wayPoint,int _targetWayPoint)
{
    this->infoOpen = false;
    this->touched = false;

    CCLOG("MapViewScene: WayPoint: %d  TargeWayPoint %d",_wayPoint, _targetWayPoint);
    Global::ULTIMO_PONTO_CHEGADA_WAYPOINT = _targetWayPoint;
    CCApplication::sharedApplication().hideBars(true);
    this->wayPoint = _wayPoint;
    targetWayPoint = _targetWayPoint;

	//black screen
	CCSprite *blackScreen = CCSprite::create("backInBlack.png");
	blackScreen->setScaleX(CCDirector::sharedDirector()->getWinSize().width / blackScreen->boundingBox().size.width);
	blackScreen->setScaleY(CCDirector::sharedDirector()->getWinSize().height / blackScreen->boundingBox().size.height);
    blackScreen->setPosition(ccp(0, 0));
    blackScreen->setContentSize(CCSizeMake(100, 100));
	blackScreen->setAnchorPoint(ccp(0,0));
	this->addChild(blackScreen);
    
	Pathfinder *pathfinder = Pathfinder::create();
    pathfinder->setAnchorPoint(ccp(0, 0));
    pathfinder->setScale(1.0f);
    pathfinder->start(_wayPoint, _targetWayPoint);
    pathfinder->m_CCFinishDelegate = this;
    
    this->addChild(pathfinder,0,Global::TAG_Child_Layer);
    setTouchEnabled(true);
	
    this->addChild(pathfinder->getLoading(), 1, 99001);
	//this->addChild(pathfinder->getLoading());

   
    CCMenu* menu2 = CCMenu::create();
    createMenuItem(menu2, 123, "Seta1.png", "Seta2.png",0, 0, menu_selector(IFixedMenu::btnTop), this);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    if (winsize.height > 480) {
       menu2->setPosition(ccp(10,508));
    }
    else
    {
        menu2->setPosition(ccp(10,418));
    }

    
    menu2->setAnchorPoint(ccp(0,0));
    this->addChild(menu2,3,1000);
    this->destination = strdup(LoadWayPointName(_targetWayPoint));
    
    std::string cS = (std::string("") + destination).c_str();
    if( cS.length() > 28)
        cS = cS.substr(0,24) + "...";

    CCLabelTTF *labelTitle2;
    labelTitle2 = CCLabelTTF::create(cS.c_str(), CCSizeMake(300, 55),  kCCTextAlignmentLeft, "Lucida Grande", 11);
    labelTitle2->setPosition(ccp(160, 3));
    labelTitle2->setAnchorPoint(ccp(0,0.5));
    labelTitle2->setColor(ccc3(255, 255, 255));
    this->addChild(labelTitle2,3,999999);
    setUp();

}
void MapViewScene::ccFinishJob()
{
    loaded = 1;
    std::cout<<"MAP FINISH LOADED AND RETURN: \n";
    this->setUpNavigationButtons();
    this->CreateButtons();
    
}
void MapViewScene::setUpNavigationButtons()
{
    CCMenu *menu = CCMenu::create();
    createMenuItem(menu, 111, "esquerda_Btn.png", "esquerda_Press.png", 0, 0, menu_selector(MapViewScene::btnMoveBack), menu);
    createMenuItem(menu, 222, "direita_Btn.png", "direita_Press.png", 68, 0, menu_selector(MapViewScene::btnMoveFoward), menu);
    menu->setAnchorPoint(ccp(0, 0));
	menu->setPosition(ccp(10, 10));
    this->addChild(menu,1,20);

}


void MapViewScene::setUpButtonTop()
{
    char* chFavoritos = "favoritos_Mapa_btn.png";
    
    if( isFavoritoChecked)
        chFavoritos = "favoritos_Mapa_press.png";
    
    CCMenu* menu3 = CCMenu::create();
    createMenuItem(menu3, 112233, chFavoritos, chFavoritos,0, 0, menu_selector(MapViewScene::btnTopButton), this);
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    if (winsize.height > 480) {
         menu3->setPosition(ccp(250,490));
    }
    else
    {
        menu3->setPosition(ccp(250,400));
    }
   
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
        pRotas->DeleteRota(targetWayPoint);
    setUpButtonTop();
}



void MapViewScene::OnBack()
{
  this->removeChildByTag(Global::TAG_Child_Layer, true);
  
    returnToHome();
}

void MapViewScene::CreateButtons()
{
    if(this->getChildByTag(11)!=NULL)
        this->removeChildByTag(11, true);
    
//    CCMenu* menuEntradaSaida = CCMenu::create();
//    if(type == MAP_NORMAL )
//    {
//        createMenuItem(menuEntradaSaida, EST_SAIDA, "Saida_btn.png", "Saida_press.png", 0,0, menu_selector( ParkingScene::btnEntradaSaida),this);
//        createMenuItem(menuEntradaSaida, EST_ENTRADA, "Entrada_Press.png", "Entrada_Press.png", 83,0, menu_selector( ParkingScene::btnEntradaSaida),this);
//    }
//    else
//    {
//        createMenuItem(menuEntradaSaida, EST_SAIDA, "Saida_press.png", "Saida_press.png", 0,0, menu_selector( ParkingScene::btnEntradaSaida),this);
//        createMenuItem(menuEntradaSaida, EST_ENTRADA, "Entrada_btn.png", "Entrada_Press.png", 83,0, menu_selector( ParkingScene::btnEntradaSaida),this);
//    }
//    
//    if(this->getChildByTag(12)!=NULL)
//        removeChildByTag(12, true);
//    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
//    if (winsize.height > 480) {
//        menuEntradaSaida->setPosition(ccp(73,445));
//    }
//    else
//    {
//        menuEntradaSaida->setPosition(ccp(73,365));
//    }
//    
//    this->addChild(menuEntradaSaida,0,2);
}

void MapViewScene::draw()
{
    
    
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    

    CCLabelTTF *labelTitle;
    std::string cS;
    
	Pathfinder *pathfinder = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) );
	
	CCString *info;
	try {
        int actualIdx =pathfinder->getActualMapIndex();
  		int buildingActual = pathfinder->getMapBuildings().at(actualIdx);
        int buildingNext;
        if (pathfinder->getMapBuildings().size()>actualIdx+1) {
             buildingNext = pathfinder->getMapBuildings().at(actualIdx+ 1);
        }
        else
        {
            buildingNext = pathfinder->getMapBuildings().at(actualIdx);
        }
        
        int floorNext;
        
        if (pathfinder->getMapBuildings().size()>actualIdx+1) {
            floorNext = pathfinder->getMapFloors().at(actualIdx + 1);
        }
        else
        {
            floorNext = pathfinder->getMapFloors().at(actualIdx);
        }
        
		int floorNumber = floorNext <= 0 ? floorNext * -1 + 1 : floorNext;
		
		if(buildingActual == buildingNext)
		{
			if(floorNext <= 0){
				info = CCString::createWithFormat("o elevador até o Intermediário %d", floorNumber);
			}else{
				info = CCString::createWithFormat("o elevador até o %dº Andar", floorNumber);
			}
		}
		else
		{
			switch(buildingNext){
				case 1:
					info = CCString::create("o Bloco A1");
					break;
				case 2:
					info = CCString::create("o Bloco A");
					break;
				case 3:
					info = CCString::create("o Bloco B");
					break;
				case 4:
					info = CCString::create("o Bloco D");
					break;
				case 5:
					info = CCString::create("o Bloco E");
					break;
			}
		}
	}
	catch(std::out_of_range)
	{
		info = CCString::createWithFormat(" %s",destination);
	}
	
	std::string tmpinfo = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep;
    
    if( strcmp(((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() , "Você chegou ao seu destino") ==0)
        cS = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() +std::string("\n") + destination ;
    else
	{
		if(((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.find("Siga para") != std::string::npos ||
			((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.find("Pegue o elevador") != std::string::npos)
			cS = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() +std::string(" em direção a ") + destination;
		else
			cS = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->descriptionStep.c_str() +std::string(" em direção a") + info->getCString();
	}
	
    int p1 =abs(((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->getStepActual());
    int p2 =abs(((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer) )->getStepsCount());
    if( p1 == p2){
        cS  = "Você chegou ao seu destino" +std::string("\n") + destination ;
    }

    
	if(!(pathfinder->getTotalStep() > 0))
		cS = "";
	if (!loaded) {
        cS="";//Carregando...
    }
    labelTitle = CCLabelTTF::create(cS.c_str(), CCSizeMake(302, 100),  kCCTextAlignmentLeft, "Lucida Grande", 16);
    if (winsize.height > 480) {
        labelTitle->setPosition(ccp(15, 430));
    }
    else
    {
        labelTitle->setPosition(ccp(15, 330));
    }
    
    labelTitle->setAnchorPoint(ccp(0,0.5));
    labelTitle->setColor(ccc3(0, 0, 0));
    
    if(this->getChildByTag(80) != NULL)
        this->removeChildByTag(80, true);
    
    this->addChild(labelTitle,3,80);
    
    char buffer [50];
    int pos = 0;
    
    
    if( (pathfinder->getTotalStep() > 0) && (pathfinder->getTotalStep() < 9999) && (abs(pathfinder->getStepsCount()) >0)&& (abs(pathfinder->getStepsCount()) <9999)&& (abs(pathfinder->getStepActual()) >0)&& (abs(pathfinder->getStepActual()) <9999))
    {
        sprintf(buffer,"Passo %d de %d", abs(pathfinder->getStepActual()),pathfinder->getStepsCount());
        pos = 110;
    }
    else
    {
        sprintf(buffer," Aguarde Carregando ");
        pos = 80;
    }

    CCLabelTTF *l = CCLabelTTF::create(buffer,"LucidaGrandeBold", 13);
    if (winsize.height > 480) {
         l->setPosition(ccp(pos,523));
    }
    else
    {
         l->setPosition(ccp(pos ,433));
    }

   
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
	
	Pathfinder *p = ((Pathfinder*)this->getChildByTag(Global::TAG_Child_Layer));

	if(this->touched && !p->getStepLock())
    {
		p->setPosition(ccpAdd(p->getPosition(), ccp(location.x - this->touchLocation.x, location.y - this->touchLocation.y)));
		CCPoint pos = p->getPosition();
		
		CCSprite* mapImage = (CCSprite*)p->getChildByTag(999); //map image
		
		if(pos.x > mapImage->boundingBox().size.width/2) {
			pos.x = mapImage->boundingBox().size.width/2;
			p->setPosition(pos);
		}
		else if(pos.x < mapImage->boundingBox().size.width/-2) {
			pos.x = mapImage->boundingBox().size.width/-2;
			p->setPosition(pos);
		}
		
		if(pos.y > mapImage->boundingBox().size.height/2) {
			pos.y = mapImage->boundingBox().size.height/2;
			p->setPosition(pos);
		}
		else if(pos.y < mapImage->boundingBox().size.height/-2) {
			pos.y = mapImage->boundingBox().size.height/-2;
			p->setPosition(pos);
		}
		
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
