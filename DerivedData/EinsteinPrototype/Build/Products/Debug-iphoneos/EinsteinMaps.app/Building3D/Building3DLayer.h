#ifndef __BUILDING3DLAYER_SCENE_H__
#define __BUILDING3DLAYER_SCENE_H__

#include "cocos2d.h"
#include <fstream>

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

class Building3DLayer : public cocos2d::CCLayer
{
private:
	SCENE _scene;
	STATE _state;
	kmMat4 _projection, _mvp;
	GLuint _bufferObject;
	GLuint _attrib[11];
	enum {
		VERTEX = 0, NORMAL, AMBIENT_COLOR, DIFFUSE_COLOR, MV, MVP, LIGHT_COLOR, SELECTED, PICK_VERTEX, PICK_MVP, PICK_COLOR
	};
	cocos2d::CCGLProgram* _renderProgram;
	cocos2d::CCGLProgram* _pickProgram;
public:
    virtual bool init();
	virtual void draw() override;
	std::string readBinaryFile(const std::string& filename)
	{
		std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str());
		size_t filesize;
		unsigned char* data = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &filesize);
		return std::string(data, data+filesize);
	}
	GLuint createBuffer(const std::string& filename)
	{
		std::string model = readBinaryFile(filename);
		
		const char* p = model.c_str();
		
		unsigned vertexCount;
		memcpy(&vertexCount, p, sizeof(unsigned));
		p += sizeof(unsigned);

		memcpy(&_scene.count, p, sizeof(unsigned));
		p += sizeof(unsigned);

		_scene.objects = (OBJECT**)malloc(_scene.count * sizeof(OBJECT*));
		
		int i;
		for (i = 0; i < _scene.count; i++)
		{
			_scene.objects[i] = (OBJECT*)malloc(sizeof(OBJECT));
			memcpy(_scene.objects[i], p, sizeof(OBJECT));
			p += sizeof(OBJECT);
		}
		
		GLuint bufferObject;
		
		glGenBuffers(1, &bufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VERTEXDATA), p, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0u);
		
		_bufferObject = bufferObject;
		
		return bufferObject;
	}
	void loadDefaultState()
	{
		kmMat4Translation(&_state.modelview, 0, -20.f, -200.f);
		_state.rX = 0.f;
		
		kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
	}
	void move(float x, float y)
	{
		kmMat4 copy, trans;
		
		kmMat4Assign(&copy, &_state.modelview);
		kmMat4Translation(&trans, x, 0, 0);
		kmMat4Multiply(&_state.modelview, &trans, &copy);
		
		kmMat4Assign(&copy, &_state.modelview);
		kmMat4RotationX(&trans, _state.rX);
		kmMat4Translation(&trans, trans.mat[3*4+1] * y, trans.mat[3*4+2] * y, trans.mat[3*4+3] * y);
		kmMat4Multiply(&_state.modelview, &trans, &copy);
		
		kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
		
	}
	void rotate(float x, float y)
	{
		kmMat4 copy, rot;
		
		x /= 33;
		y /= 33;
		
		kmMat4Assign(&copy, &_state.modelview);
		kmMat4RotationY(&rot, x);
		kmMat4Multiply(&_state.modelview, &copy, &rot);
		
		kmMat4Assign(&copy, &_state.modelview);
		_state.rX += y;
		kmVec3 axis = {copy.mat[1*4+1],copy.mat[2*4+1],copy.mat[3*4+1]};
		kmMat4RotationAxisAngle(&rot, &axis, y);
		
		/*kmMat4Identity(&rot);
		float degrees = kmDegreesToRadians(y);
		
		float s = sinf(degrees);
		float c = cosf(degrees);
		float ce = 1 - c;
		
		{
			float x, y, z;
			x = copy.mat[1*4+1];
			y = copy.mat[2*4+1];
			z = copy.mat[3*4+1];
			
			rot.mat[1*4+1] = x * x * ce + c;
			rot.mat[1*4+2] = y * x * ce + z * s;
			rot.mat[1*4+3] = x * z * ce - y * s;

			rot.mat[2*4+1] = x * y * ce - z * s;
			rot.mat[2*4+2] = y * y * ce + c;
			rot.mat[2*4+3] = y * z * ce + x * s;

			rot.mat[3*4+1] = x * z * ce + y * s;
			rot.mat[3*4+2] = y * z * ce - x * s;
			rot.mat[3*4+3] = z * z * ce + c;
		}*/
		
		kmMat4Multiply(&_state.modelview, &copy, &rot);
		
		kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
	}
	void zoom(float sc)
	{
		sc *= 2;
		
		kmMat4 copy, trans;

		kmMat4Assign(&copy, &_state.modelview);
		kmMat4Translation(&trans, 0.f, 0.f, sc);
		kmMat4Multiply(&_state.modelview, &trans, &copy);

		kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
	}
	void didPan(CCObject* obj);
	void didPinch(CCObject* obj);
	void ccTouchesBegan(cocos2d::CCSet *touch, cocos2d::CCEvent *events) override;
	void ccTouchesMoved(cocos2d::CCSet *touch, cocos2d::CCEvent *events) override;
	void ccTouchesEnded(cocos2d::CCSet *touch, cocos2d::CCEvent *events) override;
    static cocos2d::CCScene* scene();
    CREATE_FUNC(Building3DLayer);
private:
	cocos2d::CCTexture2D * mTexture;
};

#endif
