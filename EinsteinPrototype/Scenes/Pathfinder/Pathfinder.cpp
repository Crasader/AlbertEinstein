//
//  Pathfinder.cpp
//  EinsteinPrototype
//
//  Created by Farofa Studios on 8/23/12.
//  Copyright (c) 2012 Farofa Studios. All rights reserved.
//

#include "Pathfinder.h"
#include "Building.h"
#include "Floor.h"
#include "Waypoint.h"
#include "ChangeAnchorPoint.h"

Pathfinder::Pathfinder(){
	
	std::cout<<__PRETTY_FUNCTION__<<"\n";

	this->plistWaypoints = CCDictionary::createWithContentsOfFileThreadSafe("waypoints.plist");
	this->plistWaypoints->retain();

	
	this->loading = new AnimatedSprite("loader_einstein1.png");
	this->loading->addAnimation(AnimatedSprite::animationWithFile("loader_einstein", 145, 0.030f), "anim");
	
	this->white = CCRenderTexture::create(1024, 1024, kCCTexture2DPixelFormat_RGBA4444);
	this->white->clear(1, 1, 1, 0);
	
	
    init();
	//this->aStar = AStar::create();
	
	this->setAnchorPoint(ccp(0,0));
	
    this->arrayMaps = CCArray::create();
    this->arrayMaps->retain();
    
    this->arrayMapNames = CCArray::create();
    this->arrayMapNames->retain();
    
	this->mapBuildings = std::vector<int>();
	this->mapFloors = std::vector<int>();
	
	
    //this->arrayPoints = CCArray::create();
    //this->arrayPoints->retain();
	
	this->arrayLines = CCArray::create();
	this->arrayLines->retain();
	
	this->arrayActualSteps = CCArray::create();
	this->arrayActualSteps->retain();
	
	this->arrayIcons = NULL;
	
	angle = 0;
	stepLock = true;
}

Pathfinder *Pathfinder::create(){
    Pathfinder *p = new Pathfinder();
    p->autorelease();
    return p;
}


Pathfinder::~Pathfinder(){
	CCLOG("ABCDEc");
	
	//REMOVER MAP
	this->releaseActualMap();
	
	//if(this->arrayTiles){
		//this->arrayTiles->removeAllObjects();
		//this->arrayTiles->release();
	//}
	
	if(this->arrayIcons){
		this->arrayIcons->removeAllObjects();
		this->arrayIcons->release();
	}
	
    this->arrayMaps->removeAllObjects();
    this->arrayMaps->release();
	this->arrayMaps = NULL;
    
    this->arrayMapNames->removeAllObjects();
    this->arrayMapNames->release();
	this->arrayMapNames = NULL;
	
	this->plistWaypoints->removeAllObjects();
	this->plistWaypoints->release();
	this->plistWaypoints = NULL;
	
	this->arrayLines->removeAllObjects();
	this->arrayLines->release();
	this->arrayLines = NULL;
	
	this->arrayActualSteps->removeAllObjects();
	this->arrayActualSteps->release();
	this->arrayActualSteps = NULL;
    
	//this->arrayPoints->removeAllObjects();
	//this->arrayPoints->release();
	//this->arrayPoints = NULL;
	
	Building::clearArray();
	Elevator::clearArray();
	Floor::clearArray();
	Waypoint::clearArray();
	Escalator::clearArray();
	
	this->removeAllChildrenWithCleanup(true);
	this->loading->getParent()->removeChild(this->loading, true);
	
	//delete(this->loading);
	
}

void Pathfinder::start(int startID, int endID){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	//startID = 6;
    
    //endID = 40; //Transfer
    //endID = 5; //Elevator
    //endID = 22; //Alternative Elevator

	//findWC(startID);
	//findPath(startID, endID);
    
	if(startID > 0 && endID > 0){
		CCLOG("pfkmsfksdmfkodsmfksf");
		this->findPath(startID, endID);
	}else if(startID > 0){
		this->findWC(startID);
	}else{
		return;
	}
	
	
	this->valueI = 0;
	this->stepsCount = 0;
	try {
	this->calculateTotalSteps();
	} catch(std::exception& e) {
		std::cout<<"Exception at "<<__LINE__<<" "<<__FILE__<<"\n";
		std::terminate();
	}
	
	//this->actualMapIndex = 0;
	//this->nextMap();
	
	//ADD LOADING
	this->white->setAnchorPoint(ccp(0.5f, 0.5f));
	this->white->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 - 30));
	this->addChild(this->white);
	
	this->loading->runAnimation("anim",true, false);
	this->loading->setAnchorPoint(ccp(0.5f, 0.5f));
	this->loading->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 - 30));
	//this->addChild(this->loading);
	
}

void Pathfinder::releaseActualMap(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	CCTMXTiledMap *actualMap = (CCTMXTiledMap *)this->getChildByTag(100);
	CCSprite *actualMapImage = (CCSprite *)this->getChildByTag(999);

	if(actualMap){
		int i;
		
		this->removeChild(actualMap, true);
		this->removeChild(actualMapImage, true);
		//this->actualMap->release();
		//actualMap = NULL;
		
		//this->arrayTiles->release();
		//this->arrayTiles = NULL;
		
		//this->arrayPoints->removeAllObjects();
		
		if(this->arrayIcons){
			for(i = 0; i < this->arrayIcons->count(); i++){
				this->removeChild((CCSprite *) this->arrayIcons->objectAtIndex(i), true);
			}
		
			this->arrayIcons->release();
			this->arrayIcons = NULL;
		}
		
		if(this->getChildByTag(151515)) {
			this->removeChildByTag(151515, true);
		}
		
		if(this->arrayLines){
			for(i = 0; i < this->arrayLines->count(); i++){
				this->removeChild((CCSprite *) this->arrayLines->objectAtIndex(i), true);
			}
		
			this->arrayLines->removeAllObjects();
		}
		
		if(this->arrayActualSteps){
			for(i = 0; i < this->arrayActualSteps->count(); i++){
				this->removeChild((CCSprite *) this->arrayActualSteps->objectAtIndex(i), true);
			}
			
			this->arrayActualSteps->removeAllObjects();
		}
	}
}

void Pathfinder::loadMap(CCString *mapName, bool isVisible){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	const char *actualMapName = mapName->getCString();
	//CCLOG(actualMapName);
	CCTMXTiledMap* actualMap = CCTMXTiledMap::create(actualMapName);
	actualMap->setTag(100);
	//actualMap->retain();
	//this->addChild(actualMap);
	actualMap->setScale(2);
	actualMap->setVisible(isVisible);
	//this->setContentSize(CCSizeMake(actualMap->getMapSize().width * actualMap->getTileSize().width, actualMap->getMapSize().height * actualMap->getTileSize().height));
	
	
	//PERFORMANCE TEST
	CCString *mapImageName = actualMap->propertyNamed("mapName");
	int posX = actualMap->propertyNamed("posX")->intValue() * actualMap->getTileSize().width;
	int posY = actualMap->propertyNamed("posY")->intValue() * actualMap->getTileSize().height;
	
	CCSprite *mapImage = CCSprite::create(mapImageName->getCString());
	this->addChild(mapImage, 0, 999);
	mapImage->setScale(2);
	mapImage->setPosition(ccp(-posX + mapImage->boundingBox().size.width / 2, -posY + mapImage->boundingBox().size.height / 2));
	
	//actualMap->setPosition(ccp(posX , posY));
	this->addChild(actualMap);
	
	this->setContentSize(CCSizeMake(mapImage->boundingBox().size.width, mapImage->boundingBox().size.height));
	
	//LOAD OBJECTS FOR ICONS
	//this->loadIcons();
	
	//LAYER
	CCTMXLayer *layerWall = actualMap->layerNamed("wall");
	layerWall->setVisible(false);
	
	int w = actualMap->getMapSize().width;
	int h = actualMap->getMapSize().height;
	int i = 0;
	
	
	std::vector< std::vector<ASTile> > arrayTiles;
	//this->arrayTiles = CCArray::create();
	//this->arrayTiles->retain();
	
	for(i = 0; i < w; i++){
		std::vector<ASTile> array;
		int yPoint = 0;
		for(int j = h -1; j >= 0; j--){
			ASTile tile;
			tile.setPointX(i);
			tile.setPointY(yPoint);
			
			yPoint++;
			
			int gID = layerWall->tileGIDAt(ccp(i, j));
			if(gID > 0){
				CCDictionary *tileProperties = actualMap->propertiesForGID(gID);
				if(tileProperties){
					if(tileProperties->valueForKey("wall")->intValue() == 1){
						tile.setPassable(false);
					}
				}
			}
			
			array.push_back(tile);
		}
		
		arrayTiles.push_back(array);
	}
	
	//START + END TILE - OBJECTS with IDs
	Floor *actualFloor = (Floor *)this->arrayMaps->objectAtIndex(this->actualMapIndex);
	CCTMXObjectGroup *waypoints = actualMap->objectGroupNamed("waypoint");
	
	ASTile begin;
	ASTile end;
	
	for(i = 0; i < waypoints->getObjects()->count(); i++){
		CCDictionary *object = (CCDictionary *)waypoints->getObjects()->objectAtIndex(i);
		int objX = object->valueForKey("x")->intValue() / actualMap->getTileSize().width;
		int objY = object->valueForKey("y")->intValue() / actualMap->getTileSize().height;
		
		if(object->valueForKey("id")->intValue() == actualFloor->getStartID()){
			begin = arrayTiles.at(objX).at(objY);
			begin.setPassable(true);
		}
		
		if(object->valueForKey("id")->intValue() == actualFloor->getEndID()){
			end = arrayTiles.at(objX).at(objY);
			end.setPassable(true);
		}
	}
	
	//EXECUTE A*
	AStar astar;
	std::vector<ASTile> arrayPath = astar.findBestPath(arrayTiles, begin, end, true);
	
	this->arrayPoints = std::vector<ASTile>();
	
	//POINTS TO DRAW LINE
	for(i = 0; i < arrayPath.size(); i++){
		ASTile path = arrayPath.at(i);
		arrayPoints.push_back(path);
	}
	
	arrayPoints.push_back(begin);
	//this->aStar->clear();
	
	std::cout<<"MAP COUNT AND INDEX: "<<this->arrayMaps->count()<<" "<<this->actualMapIndex<<"\n";
	if(this->arrayMaps->count()-1 > this->actualMapIndex) {
	
		CCSprite *spriteDefault = CCSprite::create("bt_proxMapa.png");
		CCSprite *spriteSelected = CCSprite::create("bt_proxMapa.png");

		CCMenuItemSprite *item = CCMenuItemSprite::create(spriteDefault, spriteSelected, this, menu_selector(Pathfinder::goToNextMap));
		
		CCMenu* next = CCMenu::create();
		next->setAnchorPoint(ccp(0.5,0.5));

		item->setAnchorPoint(ccp(0, 0));
		item->setPosition(ccp(0,0));
		next->setPosition(ccp(arrayPoints.front().getPointX()*10, arrayPoints.front().getPointY()*10));
		next->addChild(item, 0, 0);
		item->setRotation(25);
		this->addChild(next, 0, 151515);
	
	}
	
	/*this->arrayIcons = CCArray::create();
	this->arrayIcons->retain();*/
	
	//this->arrayIcons->addObject(next);
	//this->addChild(next);
	//CCSprite *icon = CCSprite::create("az_btn.png");
	//icon->setPosition(ccp(arrayPoints.front().getPointX()*10, arrayPoints.front().getPointY()*10));
	//this->addChild(icon);
std::cout<<"MAP LOADED: \n";
    if (m_CCFinishDelegate) {
         m_CCFinishDelegate->ccFinishJob();
    }
   
	
}

void Pathfinder::goToNextMap(CCObject* obj = NULL)
{
	this->step(-this->actualStep,false);
}

void Pathfinder::loadIcons(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	this->arrayIcons = CCArray::create();
	this->arrayIcons->retain();
	
	CCTMXTiledMap *actualMap = (CCTMXTiledMap *)this->getChildByTag(100);
	CCTMXObjectGroup *icons = actualMap->objectGroupNamed("icon");
	
	for(int i = 0; i < icons->getObjects()->count(); i++){
		CCDictionary *object = (CCDictionary *)icons->getObjects()->objectAtIndex(i);
		
		int objX = object->valueForKey("x")->intValue() + object->valueForKey("width")->intValue() / 2;
		int objY = object->valueForKey("y")->intValue() + object->valueForKey("height")->intValue() / 2;
		
		CCSprite *icon = NULL;
		
		//GET WAYPOINT ID AND SET AS TAG
		
		switch(object->valueForKey("id")->intValue()){
			case ICON_ACESSO_RESTRITO:	icon = CCSprite::create("iconeAcessoRestrito.png");	break;
			case ICON_ELEVADOR:			icon = CCSprite::create("iconeElevador.png");		break;
			case ICON_ENTRADA:			icon = CCSprite::create("iconeEntrada.png");		break;
			case ICON_SAIDA:			icon = CCSprite::create("iconeSaida.png");			break;
			case ICON_ESTACIONAMENTO:	icon = CCSprite::create("iconeTeste.png");			break;
			case ICON_ESCADA:			icon = CCSprite::create("iconeEscada.png");			break;
			case ICON_ESCADA_ROLANTE:	icon = CCSprite::create("iconeEscadaRolante.png");	break;
			case ICON_CAFE:				icon = CCSprite::create("iconeCafe.png");			break;
			case ICON_RESTAURANTE:		icon = CCSprite::create("iconeRestaurante.png");	break;
			case ICON_CAIXA_ELETRONICA:	icon = CCSprite::create("iconeBanco.png");			break;
			case ICON_FARMACIA:			icon = CCSprite::create("iconeFarmacia.png");		break;
			case ICON_EXAME:			icon = CCSprite::create("iconeExame.png");			break;
			case ICON_CLINICA:			icon = CCSprite::create("iconeClinica.png");		break;
			case ICON_BANHEIRO:			icon = CCSprite::create("iconeBanheiro.png");		break;
			case ICON_PEDIATRIA:		icon = CCSprite::create("iconePediatria.png");		break;
			case ICON_FOTO:				icon = CCSprite::create("iconeFoto.png");			break;
			case ICON_OBRA_DE_ARTE:		icon = CCSprite::create("iconeObraDeArte.png");		break;
			case ICON_RECEPCAO:			icon = CCSprite::create("iconeRecepcao.png");		break;
			case ICON_ESCULTURA:		icon = CCSprite::create("iconeEscultura.png");		break;
			case ICON_APARTAMENTOS:		icon = CCSprite::create("iconeApartamentos.png");	break;
			case ICON_CABELEIREIRO:		icon = CCSprite::create("iconeCabeleireiro.png");	break;
			case ICON_PRESENTES:		icon = CCSprite::create("iconePresentes.png");		break;
			case ICON_JARDIM:			icon = CCSprite::create("iconeJardim.png");			break;
			case ICON_MATERNIDADE:		icon = CCSprite::create("iconeMaternidade.png");	break;
			case ICON_AUDITORIO:		icon = CCSprite::create("iconeAuditorio.png");		break;
			case ICON_SINAGOGA:			icon = CCSprite::create("iconeSinagoga.png");		break;
			default:
				break;
		}
		
		if(icon){
			icon->setPosition(ccp(objX, objY));
			this->arrayIcons->addObject(icon);
			this->addChild(icon);
			//icon->setPosition(ccp(objX-(icon->boundingBox().size.width/2), objY-(icon->boundingBox().size.height/2)));
			icon->setScale(2);
			//icon->setVisible(false);
			icon->setTag(object->valueForKey("waypointID")->intValue());
		}
		
	}
	
}

void Pathfinder::executeAStar(bool smooth){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	/*//START + END TILE - OBJECTS with IDs
	Floor *actualFloor = (Floor *)this->arrayMaps->objectAtIndex(this->actualMapIndex);
	CCTMXObjectGroup *waypoints = this->actualMap->objectGroupNamed("waypoint");
	
	ASTile *begin; 
	ASTile *end;
	int i;
	
	for(i = 0; i < waypoints->getObjects()->count(); i++){
		CCDictionary *object = (CCDictionary *)waypoints->getObjects()->objectAtIndex(i);
		int objX = object->valueForKey("x")->intValue() / this->actualMap->getTileSize().width;
		int objY = object->valueForKey("y")->intValue() / this->actualMap->getTileSize().height;
		
		if(object->valueForKey("id")->intValue() == actualFloor->getStartID()){
			//begin = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(objX))->objectAtIndex(objY);
			begin->setPassable(true);
		}
		
		if(object->valueForKey("id")->intValue() == actualFloor->getEndID()){
			//end = (ASTile *)((CCArray *)this->arrayTiles->objectAtIndex(objX))->objectAtIndex(objY);
			end->setPassable(true);
		}
	}
	
	//EXECUTE A*
	//CCArray *arrayPath = this->aStar->findBestPath(this->arrayTiles, begin, end, true);
	
	//POINTS TO DRAW LINE
	for(i = 0; i < arrayPath->count(); i++){
		ASTile *path = (ASTile *)arrayPath->objectAtIndex(i);
		arrayPoints->addObject(path);
	}
	
	arrayPoints->addObject(begin);
	this->aStar->clear();*/
		
}

void Pathfinder::drawLines(LineType lineType){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	CCTMXTiledMap *actualMap = (CCTMXTiledMap *)this->getChildByTag(100);
	
	ASTile point = arrayPoints.at(0);
	CCPoint lastPoint = ccp((point.getPointX() * actualMap->getTileSize().width + 6), (point.getPointY() * actualMap->getTileSize().height + 6));
	CCPoint lastPointToAngleCalc = ccp((point.getPointX() * actualMap->getTileSize().width + 6) , ((actualMap->getMapSize().height - point.getPointY()) * actualMap->getTileSize().height + 6));
	
	if(lineType == LINE_SHADOW){
	//	lastPoint = ccp(lastPoint.x - 4, lastPoint.y - 4);
	//	lastPointToAngleCalc = ccp(lastPointToAngleCalc.x - 4, lastPointToAngleCalc.y - 4);
	}
	
	CCSprite *arrow;
	if(lineType != LINE_STEP){
		arrow = CCSprite::create(lineType == LINE_SHADOW ? "setaSombra.png" : "seta.png");
		this->addChild(arrow);
		arrow->setPosition(lastPoint);
		//arrow->setScale(2);
		this->arrayLines->addObject(arrow);
		//arrow->setAnchorPoint(ccp(0, 0.5f));
		
		ASTile pointRefRotation = arrayPoints.at(1);
		float angle = atan2f(point.getPointY() - pointRefRotation.getPointY(), point.getPointX() - pointRefRotation.getPointX());
		arrow->setRotation(angle * 180 / M_PI * -1);
	}
	
	for(int i = 1; i < arrayPoints.size(); i++){
		ASTile path = arrayPoints.at(i);
		int posX = (path.getPointX() * actualMap->getTileSize().width + 6);
		int posY = (path.getPointY() * actualMap->getTileSize().height + 6);
		
		if(lineType == LINE_SHADOW){
			//posX -= 4;
			//posY -= 4;
		}
		
		float angle2 = 0;
		CCSprite *arrowStep;
		if(lineType == LINE_STEP){
			arrowStep = CCSprite::create("seta.png");
			this->addChild(arrowStep);
			arrowStep->setPosition(lastPoint);
			//arrow->setScale(2);
			arrowStep->setVisible(false);
			this->arrayActualSteps->addObject(arrowStep);
			//arrowStep->setAnchorPoint(ccp(0, 0.5f));
			
			ASTile pointRefRotation2 = arrayPoints.at(i - 1);
			angle2 = atan2f(pointRefRotation2.getPointY() - path.getPointY(), pointRefRotation2.getPointX() - path.getPointX());
			arrowStep->setRotation(angle2 * 180 / M_PI * -1);
			
		}
		
		int posYtoAngleCalc = ((actualMap->getMapSize().height - path.getPointY()) * actualMap->getTileSize().height + 6);
		
		CCSprite *circle = CCSprite::create(lineType == LINE_SHADOW ? "ligacaoSombra.png" : (lineType == LINE_STEP ? "ligacao.png" : "ligacao.png"));
		this->addChild(circle);
		circle->setPosition(ccp(posX, posY));
		//circle->setScale(2);
		if(lineType == LINE_STEP){
			circle->setVisible(false);
			this->arrayActualSteps->addObject(circle);
		}else{
			this->arrayLines->addObject(circle);
		}
		
		float distance = ccpDistance(lastPoint, ccp(posX, posY));
		if(i == 1){
			distance -= 30;
			CCPoint endPoint;
			endPoint.x = sinf(CC_DEGREES_TO_RADIANS(atan2f(posX - lastPointToAngleCalc.x , posYtoAngleCalc - lastPointToAngleCalc.y) * 180 / M_PI * -1)) * distance;
			endPoint.y = cosf(CC_DEGREES_TO_RADIANS(atan2f(posX - lastPointToAngleCalc.x , posYtoAngleCalc - lastPointToAngleCalc.y) * 180 / M_PI * -1)) * distance;
			arrow->setPosition(ccpAdd(ccp(posX, posY), endPoint));
			if(lineType == LINE_STEP){
				arrowStep->setPosition(ccpAdd(ccp(posX, posY), endPoint));
			}
			
		}
		
		CCSprite *line = CCSprite::create(lineType == LINE_SHADOW ? "linhaSombra.png" : (lineType == LINE_STEP ? "linha.png" : "linha.png"));
		this->addChild(line);
		line->setPosition(ccp(posX, posY));
		//line->setScale(2);
		if(lineType == LINE_STEP){
			line->setVisible(false);
			this->arrayActualSteps->addObject(line);
		}else{
			this->arrayLines->addObject(line);
		}
		
		line->setAnchorPoint(ccp(0.5f, 0));
		line->setScaleY(distance/line->boundingBox().size.height /* * 2*/);
		line->setRotation(atan2f(posX - lastPointToAngleCalc.x , posYtoAngleCalc - lastPointToAngleCalc.y) * 180 / M_PI * -1);
		
		lastPoint = ccp(posX, posY);
		lastPointToAngleCalc = ccp(posX, posYtoAngleCalc);
	}

}

void Pathfinder::drawAllLines(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	this->drawLines(LINE_SHADOW);
	//this->drawLines(LINE_NORMAL);
	this->drawLines(LINE_STEP);
}

void Pathfinder::nextMap(CCObject* pObj = NULL){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	//this->releaseActualMap();
	
	if(this->arrayMaps->count() > this->actualMapIndex){
		this->setRotation(0);
		//loadMap();
		CCString *actualMapName = (CCString *)this->arrayMapNames->objectAtIndex(this->actualMapIndex);
		this->loadMap(actualMapName, true);
		//this->executeAStar(true);
		this->drawAllLines();
		this->loadIcons();
    }

	this->actualStep = this->arrayPoints.size();
	//this->nextStep(true);
	this->stepLock = false;
	this->step(-1, true);
	
}

void Pathfinder::previousMap(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	//this->releaseActualMap();
	
	if(this->actualMapIndex >= 0){
		this->setRotation(0);
		//this->loadMap();
		CCString *actualMapName = (CCString *)this->arrayMapNames->objectAtIndex(this->actualMapIndex);
		this->loadMap(actualMapName, true);
		//this->executeAStar(true);
		this->drawAllLines();
		this->loadIcons();
	}
	
	this->actualStep = -1;
	//this->previousStep(true);
	this->step(1, true);
	
}

void Pathfinder::showStepLine(bool firstTime){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	int step = firstTime ? actualStep + 1 : actualStep;
	
	if(step * 3 - 1 >= 0){
		
		/*
		for(int i = 0; i < this->arrayActualSteps->count(); i++){
			((CCSprite *)this->arrayActualSteps->objectAtIndex(i))->setVisible(false);
		}

		((CCSprite *)this->arrayActualSteps->objectAtIndex(step * 3 - 1))->setVisible(true);
		((CCSprite *)this->arrayActualSteps->objectAtIndex(step * 3 - 2))->setVisible(true);
		((CCSprite *)this->arrayActualSteps->objectAtIndex(step * 3 - 3))->setVisible(true);
		*/
		
		int i;
		for(i = 0; i < this->arrayActualSteps->count(); i++){
			((CCSprite *)this->arrayActualSteps->objectAtIndex(i))->setVisible(false);
		}
		
		for(i = this->arrayActualSteps->count() - 1; i >= 0; i = i - 2){
			if(step * 3 - 2 > i){
				((CCSprite *)this->arrayActualSteps->objectAtIndex(i+1))->setVisible(true);
				break;
			}
			
			((CCSprite *)this->arrayActualSteps->objectAtIndex(i))->setVisible(true);
			((CCSprite *)this->arrayActualSteps->objectAtIndex(--i))->setVisible(true);
		}
		
		if(actualStep <= 1){
			((CCSprite *)this->arrayActualSteps->objectAtIndex(0))->setVisible(true);
		}
		
	}
}

void Pathfinder::step(int nextValue, bool firstTime, bool animate){
	std::cout<<this->actualStep<< " " << this->stepActual << " " << this->getTotalStep() << " " << this->stepsCount << "\n";

	if(!this->stepLock){
		//if(this->arrayPoints){
			if(this->arrayPoints.size() > 0){
				if((nextValue == -1 && this->actualStep > 0) || (nextValue == 1 && this->actualStep+1 < this->arrayPoints.size())){
					this->stepLock = true;
					
					CCTMXTiledMap *actualMap = (CCTMXTiledMap *)this->getChildByTag(100);
					CCSize winSize = CCDirector::sharedDirector()->getWinSize();
					CCSize tileSize = actualMap->getTileSize();
					
					this->actualStep += nextValue;
					this->stepActual += nextValue;
					
					//ACTUAL POINT
					ASTile actual = (ASTile)arrayPoints.at(this->actualStep);
					CCPoint actualPoint = ccp(actual.getPointX() * tileSize.width  + 6, actual.getPointY() * tileSize.height  + 6);
					
					//NEXT POINT
					ASTile next;
					CCPoint nextPoint;
					if(this->actualStep - 1 >= 0){
						next = (ASTile)arrayPoints.at(this->actualStep - 1);
						nextPoint = ccp(next.getPointX() * tileSize.width  + 6, next.getPointY() * tileSize.height  + 6);
					}
					
					//ANCHOR CALC
					CCPoint newPoint = ccpRotateByAngle(actualPoint, this->getAnchorPointInPoints(), 0);
					
					//ANGLE CALC HERE
					float previousAngle = angle;
					if(next.getPointY() > -1){
						angle = atan2f(nextPoint.x - actualPoint.x, nextPoint.y - actualPoint.y) * 180 / M_PI * -1;
					}
					
					float xAngle = roundf(angle / 90);
					angle = xAngle * 90;
					
					//int i = 0;
					if(!firstTime){
						this->showStepLine(false);
						
						//LOG DE DIRECAO
						if((int)angle < (int)previousAngle){
							if(nextValue == -1){
								descriptionStep = "Vire a direita";
								CCLog("VIRE A DIREITA");
							}else{
								descriptionStep = "Vire a esquerda";
								CCLog("VIRE A ESQUERDA");
							}
						}else if((int)angle > (int)previousAngle){
							if(nextValue == -1){
								descriptionStep = "Vire a esquerda";
								CCLog("VIRE A ESQUERDA");
							}else{
								descriptionStep = "Vire a direita";
								CCLog("VIRE A DIREITA");
							}
							
						}else{
							if(this->actualMapIndex == this->arrayMaps->count() - 1){
								if(this->actualStep == 0){
									descriptionStep = "Você chegou ao seu destino";
									CCLog("VOCÊ CHEGOU AO SEU DESTINO");
								}else{
									descriptionStep = "Siga em frente";
									CCLog("SIGA EM FRENTE");
								}
							}else{
								if(this->actualStep == 0){
									CCString *info = this->getNextMapInfo();
									descriptionStep = info->getCString();
									CCLog(info->getCString());
								}else{
									descriptionStep = "Siga em frente";
									CCLog("SIGA EM FRENTE");
								}
							}
						}
						
						//CHANGE ANCHOR
						ChangeAnchorPoint *change = ChangeAnchorPoint::createWithAnchorPoint(newPoint, true);
						
						//MOVE TO
						CCMoveTo *move = CCMoveTo::create(0.6f, ccp(winSize.width/2,winSize.height/2));
						
						//ROTATE TO
						CCRotateTo *rotate = CCRotateTo::create(0.6f, angle);
						
						//UNLOCK STEP
						CCCallFuncN *callfunc = CCCallFuncN::create(this, callfuncN_selector(Pathfinder::unlockStep));
						
						CCFiniteTimeAction *sequence;
						if(nextValue == -1){
							//NEXT
							
							
							if((int)angle == (int)previousAngle){
								sequence = CCSequence::create(change, move, callfunc, NULL);
							}else{
								sequence = CCSequence::create(change, move, rotate, callfunc, NULL);
							}
						}else{
							//PREVIOUS
				
							
							if((int)angle == (int)previousAngle){
								sequence = CCSequence::create(change, move, callfunc, NULL);
							}else{
								sequence = CCSequence::create(rotate, change, move, callfunc, NULL);
							}
						}
						
						this->runAction(sequence);
						
						//ROTATE ICONS
						for(int i = 0; i < this->arrayIcons->count(); i++){
							CCSprite *icon = (CCSprite *)this->arrayIcons->objectAtIndex(i);
							
							CCActionInterval *interval = CCActionInterval::create(0.6f);
							CCRotateTo *iconRotate = CCRotateTo::create(0.6f, -angle);
							CCFiniteTimeAction *iconSequence;
							if(nextValue == -1){
								iconSequence = CCSequence::create(interval, iconRotate, NULL);
							}else{
								iconSequence = CCSequence::create(iconRotate, interval, NULL);
							}
							
							icon->runAction(iconSequence);
						}
						{
							CCActionInterval *interval = CCActionInterval::create(0.6f);
							CCRotateTo *iconRotate = CCRotateTo::create(0.6f, -angle);
							CCFiniteTimeAction *iconSequence;
							if(nextValue == -1){
								iconSequence = CCSequence::create(interval, iconRotate, NULL);
							}else{
								iconSequence = CCSequence::create(iconRotate, interval, NULL);
							}
							
							if(this->getChildByTag(151515))
								this->getChildByTag(151515)->getChildByTag(0)->runAction(iconSequence);
						}
						
						
					}else{
						if(nextValue == -1){
							this->showStepLine(false);
						}else{
							this->showStepLine(true);
						}
						
						//FIRST TIME
						ChangeAnchorPoint *change = ChangeAnchorPoint::createWithAnchorPoint(newPoint, true);
						this->runAction(change);
						this->setPosition(ccp(winSize.width/2, winSize.height/2));
						this->setRotation(nextValue == -1 ? angle : angle - 180);
						//this->setRotation(angle);
						stepLock = false;
						
						//ROTATE ICONS
						for(int i = 0; i < this->arrayIcons->count(); i++){
							CCSprite *icon = (CCSprite *)this->arrayIcons->objectAtIndex(i);
							icon->setRotation(nextValue == -1 ? -angle : angle);
						}
						if(this->getChildByTag(151515))
							this->getChildByTag(151515)->getChildByTag(0)->setRotation(nextValue == -1 ? -angle : angle);
						
						//LOG DE DIRECAO
						if(nextValue == -1){
							descriptionStep = "Siga em frente";
							CCLog("SIGA EM FRENTE");
						}else{
							CCString *info = this->getNextMapInfo();
							descriptionStep = info->getCString();
							CCLog(info->getCString());
						}
					}
				}else{
					
					
					if(nextValue < 0){
						if(this->actualMapIndex < this->arrayMaps->count() - 1){
							this->releaseActualMap();
							this->actualMapIndex++;
							
							//this->actualStep += (nextValue+1); << o next map muda esse
							this->stepActual += (nextValue+1);
						
							CCActionInterval *interval1 = CCActionInterval::create(3.0f);
							CCCallFuncN *callfunc1 = CCCallFuncN::create(this, callfuncN_selector(Pathfinder::nextMapInterval));
							CCFiniteTimeAction *sequence1 = CCSequence::create(interval1, callfunc1, NULL);
							this->runAction(sequence1);

							//this->nextMap();
							
							this->stepLock = true;
							
							//ADD LOADING
							this->loading->runAnimation("anim",true, false);
							CCPoint converted = this->convertToNodeSpace(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 + 30));
								this->loading->setVisible(true);
							
							this->white->setPosition(converted);
							this->setRotation(-angle);
							this->white->setVisible(true);
						}
					}else{
						if(this->actualMapIndex > 0){
							this->releaseActualMap();
							this->actualMapIndex--;
						
							CCActionInterval *interval2 = CCActionInterval::create(3.0f);
							CCCallFuncN *callfunc2 = CCCallFuncN::create(this, callfuncN_selector(Pathfinder::previousMapInterval));
							CCFiniteTimeAction *sequence2 = CCSequence::create(interval2, callfunc2, NULL);
							this->runAction(sequence2);
						
							//this->previousMap();
							
							//ADD LOADING
							this->loading->runAnimation("anim",true, false);
							CCPoint converted = this->convertToNodeSpace(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 + 30));
								this->loading->setVisible(true);
							
							this->white->setPosition(converted);
							this->setRotation(-angle);
							this->white->setVisible(true);
						}
					}
					
					
				}
			}
		//}
	}
}

CCString *Pathfinder::getNextMapInfo(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	int buildingActual = this->mapBuildings.at(this->actualMapIndex);
	
	int buildingNext = this->mapBuildings.at(this->actualMapIndex + 1);
	int floorNext = this->mapFloors.at(this->actualMapIndex + 1);
	int floorNumber = floorNext <= 0 ? floorNext * -1 + 1 : floorNext;
	
	
	CCString *info;
	if(buildingActual == buildingNext){
		if(floorNext <= 0){
			info = CCString::createWithFormat("Pegue o elevador até o Intermediário %d", floorNumber);
		}else{
			info = CCString::createWithFormat("Pegue o elevador até o %dº Andar", floorNumber);
		}
	}else{
		switch(buildingNext){
			case 1:
				info = CCString::create("Siga para o Bloco A1");
				break;
			case 2:
				info = CCString::create("Siga para o Bloco A");
				break;
			case 3:
				info = CCString::create("Siga para o Bloco B");
				break;
			case 4:
				info = CCString::create("Siga para o Bloco D");
				break;
			case 5:
				info = CCString::create("Siga para o Bloco E");
				break;
		}
	}
	
	return info;
}

void Pathfinder::nextMapInterval(CCNode *sender){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	//REMOVE LOADING
	//this->removeChild(this->loading, false);
	this->loading->setVisible(false);
	this->white->setVisible(false);
	
	this->nextMap();
}

void Pathfinder::previousMapInterval(CCNode *sender){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	//REMOVE LOADING
	//this->removeChild(this->loading, false);
	this->loading->setVisible(false);
	this->white->setVisible(false);
	
	this->previousMap();
}

void Pathfinder::unlockStep(CCNode *sender){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	this->stepLock = false;
}

void Pathfinder::findPath(int idBegin, int idEnd){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
    this->arrayMaps->removeAllObjects();
    this->arrayMapNames->removeAllObjects();
    
    Waypoint *begin = Waypoint::get(idBegin);
    Waypoint *end = Waypoint::get(idEnd);
    
    Floor *actualFloor = (Floor *)begin->getFather();
	actualFloor->setStartID(idBegin);
    Floor *finalFloor = (Floor *)end->getFather();
	finalFloor->setEndID(idEnd);

    Building *actualBuilding = (Building *)actualFloor->getFather();
    Building *finalBuilding = (Building *)finalFloor->getFather();
    
    int floor = actualFloor->getFloorNumber() <= 0 ? actualFloor->getFloorNumber() * -1 + 1 : actualFloor->getFloorNumber();
    
    //CCLOG(" ");
    //CCLOG("INICIO ------------------------------------------");
    //CCLOG(actualFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", actualBuilding->getID(), floor);
    
    CCString *mapName = CCString::createWithFormat(actualFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", actualBuilding->getID(), floor);
    this->arrayMaps->addObject(actualFloor);
    this->arrayMapNames->addObject(mapName);
    CCLOG(mapName->getCString());
	
	
	this->mapBuildings.push_back(actualBuilding->getID());
	this->mapFloors.push_back(actualFloor->getFloorNumber());
	
    while(actualFloor != finalFloor){
        if(actualBuilding == finalBuilding){
			bool hasEscalator = false;
			if(actualFloor->getEscalator() && finalFloor->getEscalator()){
				if(actualFloor->getEscalator()->getRefID() == finalFloor->getEscalator()->getRefID()){
					actualFloor->setEndID(actualFloor->getEscalator()->getWaypoint()->getID());
					actualFloor = finalFloor;
					actualFloor->setStartID(actualFloor->getEscalator()->getWaypoint()->getID());
					hasEscalator = true;
				}
			}
			
			if(!hasEscalator){
				Elevator *elevator = end->getElevatorException();
				if(!elevator){
					elevator = actualBuilding->getDefaultElevator();
				}
		
				actualFloor->setEndID(elevator->getWaypoint()->getID());
				actualFloor = finalFloor;
				actualFloor->setStartID(elevator->getWaypoint()->getID());
			}
		}else{
            if(actualBuilding->getID() > finalBuilding->getID()){
                if(actualFloor->getTransferLeft()){
                    actualBuilding = Building::get(actualBuilding->getID()-1);
                    actualFloor->setEndID(actualFloor->getTransferLeftWaypointID());
					actualFloor = actualBuilding->getFloor(actualFloor->getFloorNumber());
					actualFloor->setStartID(actualFloor->getTransferRightWaypointID());
				}else{
					actualFloor->setEndID(actualBuilding->getDefaultElevator()->getWaypoint()->getID());
					actualFloor = actualBuilding->findNearestTransferLeftFloor(actualFloor->getFloorNumber());
                    actualFloor->setStartID(actualBuilding->getDefaultElevator()->getWaypoint()->getID());
				}
            }else{
                if(actualFloor->getTransferRight()){
                    actualBuilding = Building::get(actualBuilding->getID()+1);
					actualFloor->setEndID(actualFloor->getTransferRightWaypointID());
					actualFloor = actualBuilding->getFloor(actualFloor->getFloorNumber());
					actualFloor->setStartID(actualFloor->getTransferLeftWaypointID());
				}else{
			        actualFloor->setEndID(actualBuilding->getDefaultElevator()->getWaypoint()->getID());
					actualFloor = actualBuilding->findNearestTransferRightFloor(actualFloor->getFloorNumber());
                    actualFloor->setStartID(actualBuilding->getDefaultElevator()->getWaypoint()->getID());
				}
            }
        }
		
        floor = actualFloor->getFloorNumber() <= 0 ? actualFloor->getFloorNumber() * -1 + 1 : actualFloor->getFloorNumber();
        //CCLOG(actualFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", actualBuilding->getID(), floor);
        
        CCString *newMapName = CCString::createWithFormat(actualFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", actualBuilding->getID(), floor);
        this->arrayMaps->addObject(actualFloor);
        this->arrayMapNames->addObject(newMapName);
		CCLOG(newMapName->getCString());
		
		this->mapBuildings.push_back(actualBuilding->getID());
		this->mapFloors.push_back(actualFloor->getFloorNumber());
    }
    
     //CCLOG("FIM ------------------------------------------");
    
}

int Pathfinder::getStep(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
    return arrayPoints.size()-actualStep;
}


int Pathfinder::getTotalStep(){
    return arrayPoints.size();
}


void Pathfinder::findWC(int idBegin){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	int i, j, meters, metersToWC, nearestWC;
	Waypoint *wc;
	bool wcFounded = false;

	CCTMXTiledMap *actualMap;// = (CCTMXTiledMap *)this->getChildByTag(100);
	
	ASTile actualTile;
	CCPoint actualTilePoint;
	ASTile nextTile;
	CCPoint nextTilePoint;
	
	nearestWC = 0;
	metersToWC = 99999999;
	
	this->arrayMaps->removeAllObjects();
    this->arrayMapNames->removeAllObjects();
    
    Waypoint *begin = Waypoint::get(idBegin);
	Floor *actualFloor = (Floor *)begin->getFather();
	actualFloor->setStartID(idBegin);
	
	Building *actualBuilding = (Building *)actualFloor->getFather();
    int floor = actualFloor->getFloorNumber() <= 0 ? actualFloor->getFloorNumber() * -1 + 1 : actualFloor->getFloorNumber();
	
	CCString *mapName = CCString::createWithFormat(actualFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", actualBuilding->getID(), floor);
    this->arrayMaps->addObject(actualFloor);
    this->arrayMapNames->addObject(mapName);
	
	actualFloor->setStartID(idBegin);
	
	//METERS TO WC IN SAME FLOOR
	CCArray *arrayWCs = actualFloor->getWCs();
	for(i = 0; i < arrayWCs->count(); i++){
		wc = (Waypoint *)arrayWCs->objectAtIndex(i);
		actualFloor->setEndID(wc->getID());
		
		this->releaseActualMap();
		this->actualMapIndex = 0;
		this->loadMap(mapName, false);
		//this->executeAStar(true);
		actualMap = (CCTMXTiledMap *)this->getChildByTag(100);

		meters = 0;
		for(j = 0; j < this->arrayPoints.size()-1; j++){
			actualTile = (ASTile)this->arrayPoints.at(j);
			actualTilePoint = ccp(actualTile.getPointX() * actualMap->getTileSize().width + 6, actualTile.getPointY() * actualMap->getTileSize().height + 6);
			
			nextTile = (ASTile)this->arrayPoints.at(j+1);
			nextTilePoint = ccp(nextTile.getPointX() * actualMap->getTileSize().width + 6, nextTile.getPointY() * actualMap->getTileSize().height + 6);
			
			meters += ccpDistance(actualTilePoint, nextTilePoint);
		}
		
		if(meters < metersToWC){
			metersToWC = meters;
			nearestWC = wc->getID();
			wcFounded = true;
		}
	}
	
	Building *otherBuilding = NULL;
	Floor *otherFloor = NULL;
	CCString *otherMapName;
	
	//VALIDATE LEFT FLOOR
	if(actualFloor->getTransferLeft() && !wcFounded){
		//actualFloor->setEndID(actualFloor->getTransferLeftWaypointID());
		
		otherBuilding = Building::get(actualBuilding->getID() - 1);
		otherFloor = otherBuilding->getFloor(actualFloor->getFloorNumber());
		arrayWCs = otherFloor->getWCs();
		
		if(arrayWCs->count() > 0){
			wc = (Waypoint *)arrayWCs->objectAtIndex(0);
			nearestWC = wc->getID();
			wcFounded = true;
		}
		
		floor = otherFloor->getFloorNumber() <= 0 ? otherFloor->getFloorNumber() * -1 + 1 : otherFloor->getFloorNumber();
		otherMapName = CCString::createWithFormat(otherFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", otherBuilding->getID(), floor);
		this->arrayMaps->addObject(otherFloor);
		this->arrayMapNames->addObject(otherMapName);
		
	}
	
	//VALIDATE RIGHT FLOOR
	if(actualFloor->getTransferRight() && !wcFounded){
		//actualFloor->setEndID(actualFloor->getTransferRightWaypointID());
		
		otherBuilding = Building::get(actualBuilding->getID() + 1);
		otherFloor = otherBuilding->getFloor(actualFloor->getFloorNumber());
		arrayWCs = otherFloor->getWCs();
		
		if(arrayWCs->count() > 0){
			wc = (Waypoint *)arrayWCs->objectAtIndex(0);
			nearestWC = wc->getID();
			wcFounded = true;
		}
		
		floor = otherFloor->getFloorNumber() <= 0 ? otherFloor->getFloorNumber() * -1 + 1 : otherFloor->getFloorNumber();
		otherMapName = CCString::createWithFormat(otherFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", otherBuilding->getID(), floor);
		this->arrayMaps->addObject(otherFloor);
		this->arrayMapNames->addObject(otherMapName);
	}
	
	
	int countFloor;
	//VALIDATE FLOOR ABOVE
	if(actualFloor->getFloorNumber() < actualBuilding->getMaxFloor() && !wcFounded){
		//actualFloor->setEndID(actualBuilding->getDefaultElevator()->getWaypoint()->getID());
		
		countFloor = actualFloor->getFloorNumber() + 1;
		while(!(otherFloor = otherBuilding->getFloor(countFloor))){
			countFloor++;
		}
		
		arrayWCs = otherFloor->getWCs();
		if(arrayWCs->count() > 0){
			wc = (Waypoint *)arrayWCs->objectAtIndex(0);
			nearestWC = wc->getID();
			wcFounded = true;
		}
		
		floor = otherFloor->getFloorNumber() <= 0 ? otherFloor->getFloorNumber() * -1 + 1 : otherFloor->getFloorNumber();
		otherMapName = CCString::createWithFormat(otherFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", otherBuilding->getID(), floor);
		this->arrayMaps->addObject(otherFloor);
		this->arrayMapNames->addObject(otherMapName);
	}
	
	//VALIDATE FLOOR BELOW
	if(actualFloor->getFloorNumber() > actualBuilding->getMinFloor() && !wcFounded){
		//actualFloor->setEndID(actualBuilding->getDefaultElevator()->getWaypoint()->getID());
		
		countFloor = actualFloor->getFloorNumber() - 1;
		while(!(otherFloor = otherBuilding->getFloor(countFloor))){
			countFloor--;
		}
		
		arrayWCs = otherFloor->getWCs();
		if(arrayWCs->count() > 0){
			wc = (Waypoint *)arrayWCs->objectAtIndex(0);
			nearestWC = wc->getID();
			wcFounded = true;
		}
		
		floor = otherFloor->getFloorNumber() <= 0 ? otherFloor->getFloorNumber() * -1 + 1 : otherFloor->getFloorNumber();
		otherMapName = CCString::createWithFormat(otherFloor->getFloorNumber() <= 0 ? "building_%d_floor_i%d.tmx" : "building_%d_floor_%d.tmx", otherBuilding->getID(), floor);
		this->arrayMaps->addObject(otherFloor);
		this->arrayMapNames->addObject(otherMapName);
	}
	
	this->releaseActualMap();
	if(wcFounded){
		this->actualMapIndex = 0;
		this->findPath(idBegin, nearestWC);
	}
}

void Pathfinder::init(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
    CCDictElement *element = NULL;
	CCString *key;
	int iKey;
	
	//BUILDINGS
	CCDictionary *buildings = (CCDictionary *)this->plistWaypoints->valueForKey("buildings");
	CCDICT_FOREACH(buildings, element){
		Building::create(CCString::create(element->getStrKey())->intValue(), (CCString *)element->getObject());
	}
	
	//FLOORS
	element = NULL;
	CCDictionary *floors = (CCDictionary *)this->plistWaypoints->valueForKey("floors");
	CCDICT_FOREACH(floors, element){
		key = CCString::create(element->getStrKey());
		iKey = key->intValue();
		
		CCDictionary *floor = (CCDictionary *)element->getObject();
		Building::get(floor->valueForKey("buildingID")->intValue())->addFloor(Floor::create(iKey, (CCString *)floor->valueForKey("name"),
																							floor->valueForKey("floorNumber")->intValue(), 
																							floor->valueForKey("transferRight")->boolValue(), 
																							floor->valueForKey("transferLeft")->boolValue()));
	}
	
	//WAYPOINTS
	element = NULL;
	CCDictionary *waypoints = (CCDictionary *)this->plistWaypoints->valueForKey("waypoints");
	CCDICT_FOREACH(waypoints, element){
		key = CCString::create(element->getStrKey());
		iKey = key->intValue();
		
		CCDictionary *waypoint = (CCDictionary *)element->getObject();
		switch (waypoint->count()){
			case 1: //ELEVATOR
				Waypoint::create(iKey, (CCString *)waypoint->valueForKey("name"), true, false, false);
				break;
			case 2: //WAYPOINT
				Floor::get(waypoint->valueForKey("floorID")->intValue())->addWaypoint(Waypoint::create(iKey, (CCString *)waypoint->valueForKey("name")));
				
				break;
			case 4: //TRANSFER
				Floor::get(waypoint->valueForKey("floorID")->intValue())->addWaypoint(Waypoint::create(iKey, (CCString *)waypoint->valueForKey("name"), 
																									   waypoint->valueForKey("transferRight")->boolValue(),
																									   waypoint->valueForKey("transferLeft")->boolValue()));
				break;
			case 5://ELEVATOR, ESCALATOR, WC
				if(!waypoint->valueForKey("elevator")->boolValue()){
					Floor::get(waypoint->valueForKey("floorID")->intValue())->addWaypoint(Waypoint::create(iKey, (CCString *)waypoint->valueForKey("name"), 
																										   waypoint->valueForKey("elevator")->boolValue(), 
																										   waypoint->valueForKey("escalator")->boolValue(), 
																										   waypoint->valueForKey("wc")->boolValue()));
				}else{
					Waypoint::create(iKey, (CCString *)waypoint->valueForKey("name"), 
									 waypoint->valueForKey("elevator")->boolValue(), 
									 waypoint->valueForKey("escalator")->boolValue(), 
									 waypoint->valueForKey("wc")->boolValue());
				}
				break;
			default:
				break;
		}
	}
	
	//ELEVATORS
	element = NULL;
	CCDictionary *elevators = (CCDictionary *)this->plistWaypoints->valueForKey("elevators");
	CCDICT_FOREACH(elevators, element){
		key = CCString::create(element->getStrKey());
		iKey = key->intValue();
		
		CCDictionary *elevator = (CCDictionary *)element->getObject();
		Building::get(elevator->valueForKey("buildingID")->intValue())->addElevator(Elevator::create(iKey, (CCString *)elevator->valueForKey("name"), Waypoint::get(elevator->valueForKey("waypointID")->intValue())));
	}
	
	//ESCALATOR
	element = NULL;
	CCDictionary *escalators = (CCDictionary *)this->plistWaypoints->valueForKey("escalators");
	CCDICT_FOREACH(escalators, element){
		key = CCString::create(element->getStrKey());
		iKey = key->intValue();
		
		CCDictionary *escalator = (CCDictionary *)element->getObject();
		Floor::get(escalator->valueForKey("floorID")->intValue())->setEscalator(Escalator::create(iKey, (CCString *)escalator->valueForKey("name"), Waypoint::get(escalator->valueForKey("waypointID")->intValue()), 
																																					escalator->valueForKey("refID")->intValue()));
	}
	
	//ALTERNATIVES
	element = NULL;
	CCDictionary *alternatives = (CCDictionary *)this->plistWaypoints->valueForKey("alternatives");
	CCDICT_FOREACH(alternatives, element){
		CCDictionary *alternative = (CCDictionary *)element->getObject();
		Waypoint::get(alternative->valueForKey("waypointID")->intValue())->setElevatorException(Elevator::get(alternative->valueForKey("elevatorID")->intValue()));
	}
}

//CCArray *Pathfinder::getArrayTiles(){
    //return this->arrayTiles;
//}


CCArray *Pathfinder::getArrayIcons(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	return this->arrayIcons;
}


void Pathfinder::calculateTotalSteps(){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	AStar astar;
	std::vector<ASTile> arrayPath;
	std::vector< std::vector<ASTile> > arrayTiles;
	
	//for(i; i < this->arrayMapNames->count(); i++){
	if(this->valueI < this->arrayMapNames->count()){
		const char *mName = ((CCString *)this->arrayMapNames->objectAtIndex(this->valueI))->getCString();
		std::cout<<"MNAME: "<<mName<<"\n";
		CCTMXTiledMap *mTiled = CCTMXTiledMap::create(mName);
		CCTMXLayer *layerWall = mTiled->layerNamed("wall");
		
		int w = mTiled->getMapSize().width;
		int h = mTiled->getMapSize().height;
		int j = 0;
		
		for(j = 0; j < w; j++){
			std::vector<ASTile> array;
			int yPoint = 0;
			for(int k = h -1; k >= 0; k--){
				ASTile tile;
				tile.setPointX(j);
				tile.setPointY(yPoint);
				
				yPoint++;
				
				int gID = layerWall->tileGIDAt(ccp(j, k));
				if(gID > 0){
					CCDictionary *tileProperties = mTiled->propertiesForGID(gID);
					if(tileProperties){
						if(tileProperties->valueForKey("wall")->intValue() == 1){
							tile.setPassable(false);
						}
					}
				}
				array.push_back(tile);
			}
			arrayTiles.push_back(array);
		}
		
		Floor *actualFloor = (Floor *)this->arrayMaps->objectAtIndex(this->valueI);
		CCTMXObjectGroup *waypoints = mTiled->objectGroupNamed("waypoint");
		
		ASTile begin;
		ASTile end;
		
		for(j = 0; j < waypoints->getObjects()->count(); j++){
			CCDictionary *object = (CCDictionary *)waypoints->getObjects()->objectAtIndex(j);
			int objX = object->valueForKey("x")->intValue() / mTiled->getTileSize().width;
			int objY = object->valueForKey("y")->intValue() / mTiled->getTileSize().height;
			
			CCLOG("ID: %d", object->valueForKey("id")->intValue());
			CCLOG("startID %d", actualFloor->getStartID());
			CCLOG("endID: %d", actualFloor->getEndID());
			
			std::cout<<object->valueForKey("id")->intValue()<<" ";
			
			if(object->valueForKey("id")->intValue() == actualFloor->getStartID()){
				
				begin = arrayTiles.at(objX).at(objY);
				begin.setPassable(true);
			}
			
			if(object->valueForKey("id")->intValue() == actualFloor->getEndID()){
				end = arrayTiles.at(objX).at(objY);
				end.setPassable(true);
			}
		}
		
		std::cout<<" - "<<mName<<" "<< actualFloor->getStartID() << " " << actualFloor->getEndID() <<"\n";
		arrayPath = astar.findBestPath(arrayTiles, begin, end, true);
		
		stepsCount += arrayPath.size();
		
		arrayPath.clear();
		astar.clear();
		arrayTiles.clear();
		
		CCActionInterval *interval1 = CCActionInterval::create(2.0f);
		CCCallFuncN *callfunc1 = CCCallFuncN::create(this, callfuncN_selector(Pathfinder::nextStepCount));
		CCFiniteTimeAction *sequence1 = CCSequence::create(interval1, callfunc1, NULL);
		this->runAction(sequence1);
		
	}else{
		//REMOVE LOADING
//		this->removeChild(this->loading, false);
		this->loading->setVisible(false);
		this->white->setVisible(false);
		
		this->stepsCount += this->arrayMapNames->count();
		this->stepActual = 0;
		this->actualMapIndex = 0;
		this->nextMap();
	}
}

void Pathfinder::nextStepCount(CCNode *sender){
	std::cout<<__PRETTY_FUNCTION__<<"\n";
	CCLog("%d", stepsCount);
	
	this->valueI++;
	//this->calculateTotalSteps();
	try {
	this->calculateTotalSteps();
	} catch(std::exception& e) {
		std::cout<<"Exception at "<<__LINE__<<" "<<__FILE__<<"\n";
		std::terminate();
	}
	
}

int Pathfinder::getStepsCount(){

	return this->stepsCount;
}

int Pathfinder::getStepActual(){

	return this->stepActual;
}
