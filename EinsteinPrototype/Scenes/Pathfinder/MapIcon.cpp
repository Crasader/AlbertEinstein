#include "MapIcon.h"

MapIcon::MapIcon(){
	this->target = NULL;
	this->touched = false;
}

MapIcon *MapIcon::create(const char *fileName, int ID){
	MapIcon *ts = new MapIcon();
	
	if(ts->initWithFile(fileName)){
		ts->autorelease();
		ts->setIconID(ID);
		return ts;
		
	}
	CC_SAFE_DELETE(ts);
	return NULL;
}

int MapIcon::getIconID(){
	return this->iconID;
}

void MapIcon::setIconID(int ID){
	this->iconID = ID;
}


void MapIcon::setTouchEvent(CCObject *t, SEL_MenuHandler s){
	this->target = t;
	this->selector = s;
}


void MapIcon::onEnter(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void MapIcon::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

bool MapIcon::ccTouchBegan(CCTouch* touch, CCEvent* event){
	
	CCPoint location = touch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
	
	if(location.x > this->getPosition().x && location.y > this->getPosition().y &&
	   location.x < this->getPosition().x + this->boundingBox().size.width &&
	   location.y < this->getPosition().y + this->boundingBox().size.height){
		
	
	
	CCLOG("%f, %f", location.x, location.y);
	
	this->touched = true;
	
	}
		
	return true;
}

void MapIcon::ccTouchMoved(CCTouch* touch, CCEvent* event){

	
}

void MapIcon::ccTouchEnded(CCTouch* touch, CCEvent* event){
	if(touched){
		touched = false;
		
		if(target){
			(target->*selector)(this);
		}
	}
}