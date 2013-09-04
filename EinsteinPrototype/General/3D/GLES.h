#ifndef GLES_H_
#define GLES_H_

#include "cocos2d.h"
#include <fstream>
#include "Enumerations.h"

typedef struct
{
	float vX, vY, vZ;
	float nX, nY, nZ;

	float ar, ag, ab;
	float dr, dg, db;
	float sr, sg, sb;

	float comp;
} VERTEXDATA;

typedef struct
{
	char name[64];
	unsigned first, count;

	int selected;
	unsigned char pickColor[3];
} OBJECT;

typedef struct
{
	OBJECT **objects;
	unsigned count;
} SCENE;

typedef struct
{
	kmMat4 modelview;
	float rX;
} STATE;

class Building3DLayer : /*public IFixedMenu//,*/ public cocos2d::CCLayer
{
private:
	//GLuint _bufferObject;
	//GLuint _attrib[11];
	enum {
		VERTEX = 0, NORMAL, AMBIENT_COLOR, DIFFUSE_COLOR, MV, MVP, LIGHT_COLOR, SELECTED, PICK_VERTEX, PICK_MVP, PICK_COLOR
	};
	//cocos2d::CCGLProgram* _renderProgram;
	//cocos2d::CCGLProgram* _pickProgram;
public:
    virtual bool init();
	virtual void draw() override;
	std::string readBinaryFile(const std::string& filename);
	GLuint createBuffer(const std::string& filename);
	static void loadDefaultState();
	static void move(float x, float y);
	static void rotate(float x, float y);
	static void zoom(float sc);
	static void pan(int fingerNum, int x, int y)
	{
		static int lastFinger = 0;
		static cocos2d::CCPoint last_position(0,0);
		
		if(fingerNum != lastFinger)
		{
			last_position = cocos2d::CCPoint(0,0);
			lastFinger = fingerNum;
		}
		
		if(last_position.x == 0 && last_position.y == 0)
			last_position = cocos2d::CCPoint(x,y);
		
		if(fingerNum == 1)
		{
			cocos2d::CCPoint delta;
			delta.x = x - last_position.x;
			delta.y = y - last_position.y;
			
			last_position = cocos2d::CCPoint(x,y);
			
			rotate(delta.x,delta.y);
		}
		else if(fingerNum == 2)
		{
			cocos2d::CCPoint delta;
			delta.x = x - last_position.x;
			delta.y = y - last_position.y;
			
			last_position = cocos2d::CCPoint(x,y);
			
			move(delta.x,delta.y);
		}
	}
	static void pinch(float velocity)
	{
		zoom(velocity*2);
	}
	static void tap(int x, int y);
	static void pick(int x,  int y);
    static cocos2d::CCScene* scene();
	static Building3DLayer* create()
	{
		Building3DLayer *pRet = new Building3DLayer();
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
private:
	cocos2d::CCTexture2D * mTexture;
};

/*#include "fileUtil.h"
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
};*/

#endif
