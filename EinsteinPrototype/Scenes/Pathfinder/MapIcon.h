#include "cocos2d.h"

using namespace cocos2d;

class MapIcon : public cocos2d::CCSprite, public CCTargetedTouchDelegate {

private:
	CCObject* target;
    SEL_MenuHandler selector;
	bool touched;
	int iconID;
	
public:
	
	MapIcon();
	
	void setTouchEvent(CCObject *t, SEL_MenuHandler s);
	static MapIcon *create(const char *fileName, int ID);
	int getIconID();
	void setIconID(int ID);
	
	virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

};