#ifndef GLES_H_
#define GLES_H_

#include "fileUtil.h"
#include "mat.h"
#include "cocos2d.h"

using namespace cocos2d;
class GLES : public CCLayer
{
	GLuint bufferObject;
	GLuint lightProgram, lVertex, lNormal, lMV, lMVP, lAmbientColor, lDiffuseColor, lLightColor;
	GLuint pickProgram, lPickVertex, lPickMVP, lPickColor;
	GLenum glError;
    
	Mat4f m1, m2, m3, m4, view, projection, mvp;
	SCENE scene;
	OBJECT *selectedObject;
    
	float aX, aY;
	float pX, pZ;
    
	unsigned touchCount;
	bool moved;
    
public:
	virtual ~GLES();
    
	bool init();
	void draw();
    
	void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
	void selectObject(int x, int y);
    
	//static CCScene* getScene();
	//CREATE_FUNC(GLES);
    
    
    GLES();
};

#endif
