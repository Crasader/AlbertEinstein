#include "Building3DLayer.h"
#include "SimpleAudioEngine.h"
#include "OpenGL_Internal.h"
#include "CCPanGestureRecognizer.h"
#include "CCPinchGestureRecognizer.h"
#include <Array>

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Building3DLayer::scene()
{
    CCScene *scene = CCScene::create();
    Building3DLayer *layer = Building3DLayer::create();
    scene->addChild(layer);

    return scene;
}

const unsigned VERTEX_NUMBER = 3;
const unsigned ELEMENT_NUMBER = 3;

GLuint vertex_buffer_id;
GLuint element_buffer_id;

CCSprite* pSprite;

bool Building3DLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	mTexture = CCTextureCache::sharedTextureCache()->addImage("HelloWorld.png");

	this->setTouchEnabled(true);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
	
	const size_t mult = 1;
	
	GLfloat vertex_list[VERTEX_NUMBER*3] = {1.f * mult,1.f * mult,0.f , -1.f * mult,1.f * mult,0.f , 1.f * mult,-1.f * mult,0.f};
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*VERTEX_NUMBER, vertex_list, GL_STATIC_DRAW);
	
	GLuint element_list[ELEMENT_NUMBER] = {0,1,2};
	glGenBuffers(1, &element_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*ELEMENT_NUMBER, element_list, GL_STATIC_DRAW);
	
	_renderProgram = new CCGLProgram();
	_renderProgram->initWithVertexShaderByteArray(
		R"(
			attribute vec3 vertex;
			attribute vec3 normal;
			attribute vec3 ambientColor;
			attribute vec3 diffuseColor;

			varying vec4 color;

			uniform mat4 mv;
			uniform mat4 mvp;
			uniform vec3 lightColor;
			uniform bool selected;

			void main(void)
			{
				vec4 lightPosition = vec4(0.0, 1000.0, 3000.0, 1.0);

				vec4 vertexPosition = mv * vec4(vertex, 1.0);
				vec4 mvNormal = mv * vec4(normal, 0.0);

				vec4 lightDirection = normalize(lightPosition - vertexPosition);

				float factor = max(dot(mvNormal, lightDirection), 0.0);
				
				vec3 bColor;

				if (selected)
				{
					bColor = ambientColor * lightColor + diffuseColor * lightColor * factor * 0.5;
				}
				else
				{
					bColor = ambientColor * lightColor + diffuseColor * lightColor * factor;
				}
				
				color = vec4(bColor, 1.0);

				gl_Position = mvp * vec4(vertex, 1.0);
			}
		)",
		R"(
			#ifdef GL_ES
				precision highp float;
			#endif

			varying vec4 color;

			void main(void)
			{
				gl_FragColor = color;
			}
		)"
	);
	_renderProgram->link();
	
	_pickProgram = new CCGLProgram;
	_pickProgram->initWithVertexShaderByteArray(
		R"(
			attribute vec3 vertex;

			varying vec4 color;

			uniform mat4 mvp;
			uniform vec3 pickColor;

			void main(void)
			{
				color = vec4(pickColor, 1.0);

				gl_Position = mvp * vec4(vertex, 1.0);
			}
		)",
		R"(
			#ifdef GL_ES
				precision highp float;
			#endif

			varying vec4 color;

			void main(void)
			{
				gl_FragColor = color;
			}
		)"
	);
	_pickProgram->link();
	
	_attrib[VERTEX] = glGetAttribLocation(_renderProgram->getProgram(), "vertex");
	_attrib[NORMAL] = glGetAttribLocation(_renderProgram->getProgram(), "normal");
	_attrib[AMBIENT_COLOR] = glGetAttribLocation(_renderProgram->getProgram(), "ambientColor");
	_attrib[DIFFUSE_COLOR] = glGetAttribLocation(_renderProgram->getProgram(), "diffuseColor");
	_attrib[MV] = glGetUniformLocation(_renderProgram->getProgram(), "mv");
	_attrib[MVP] = glGetUniformLocation(_renderProgram->getProgram(), "mvp");
	_attrib[LIGHT_COLOR] = glGetUniformLocation(_renderProgram->getProgram(), "lightColor");
	_attrib[SELECTED] = glGetUniformLocation(_renderProgram->getProgram(), "selected");
	
	_attrib[PICK_VERTEX] = glGetAttribLocation(_pickProgram->getProgram(), "vertex");
	_attrib[PICK_MVP] = glGetUniformLocation(_pickProgram->getProgram(), "mvp");
	_attrib[PICK_COLOR] = glGetUniformLocation(_pickProgram->getProgram(), "pickColor");
	
	createBuffer("model04.b");
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	kmMat4PerspectiveProjection(&_projection, 60.f, (float)winSize.width/winSize.height, 1.f, 10000.f);
	
	loadDefaultState();
	
	/*pSprite = CCSprite::create("HelloWorld.png");
	this->addChild(pSprite);
	pSprite->setPosition(ccp(100,0));*/
	
	/*CCPanGestureRecognizer* pan = CCPanGestureRecognizer::create();
	pan->setTarget(this, callfuncO_selector(Building3DLayer::didPan));
	pan->setCancelsTouchesInView(true);
	this->addChild(pan);
	
	CCPinchGestureRecognizer* pinch = CCPinchGestureRecognizer::create();
	pinch->setTarget(this, callfuncO_selector(Building3DLayer::didPinch));
	pinch->setCancelsTouchesInView(true);
	this->addChild(pinch);*/
	
    return true;
}

void Building3DLayer::didPan(CCObject* obj)
{
	/*if(num_touches > 1) return;

	CCPan* pan = static_cast<CCPan*>(obj);
	
	if(last_pan.x == 0 && last_pan.y == 0)
	{
		last_pan = pan->location;
	}
	
	move(pan->location.x - last_pan.x,pan->location.y - last_pan.y);
	last_pan = pan->location;*/
}

void Building3DLayer::didPinch(CCObject* obj)
{
	/*CCPinch* pinch = static_cast<CCPinch*>(obj);
	
	if(pinch->type == kPinchGestureRecognizerTypeClose)
	{
		zoom(-1);
	}
	else
	{
		zoom(1);
	}*/
}

std::array<int, 5> active_touch_list{{0,0,0,0,0}};

CCPoint last_location;

int numberOfActiveTouches(const std::array<int, 5>& arr)
{
	int cnt = 0;
	for(int i = 0; i < 5; i++)
		arr[i] ? cnt++ : cnt;
		
	return cnt;
}

#define numActiveTouches() numberOfActiveTouches(active_touch_list)

void Building3DLayer::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *events)
{
	CCSetIterator it = touches->begin();
	
	for(;it != touches->end();++it)
	{
		CCTouch* touch = static_cast<CCTouch*>(*it);
		active_touch_list[touch->getID()] = 1;
	}
}

#define DIST(x, y) (sqrtf(((x)*(x)) + ((y)*(y))))

void Building3DLayer::ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent)
{
	CCSetIterator it = touches->begin();

	if(numActiveTouches() == 1)
	{
		CCTouch* touch = (CCTouch*)(*it);
		
		CCPoint delta = touch->getLocation();
		delta.x -= last_location.x;
		delta.y -= last_location.y;
		last_location = touch->getLocation();
		
		rotate(delta.x,delta.y);
	}
	if(numActiveTouches() == 2)
	{
		static float last_distance = 40;
		std::array<CCTouch*,2> twotouches;
		int i = 0;
		for(;it!=touches->end();++it)
		{
			twotouches[i++] = static_cast<CCTouch*>(*it);
		}
		
		CCPoint d1 = twotouches[0]->getLocation();
		CCPoint d2 = twotouches[1]->getLocation();
		CCPoint diff(d1.x - d2.x, d1.y - d2.y);
		
		if(DIST(diff.x,diff.y) > last_distance + 50)
		{
			//PINCH
			if(last_distance < DIST(diff.x,diff.y))
			{
				zoom(1);
			}
			else
			{
				zoom(-1);
			}
			
			last_distance = 50;
		}
		else
		{
			//TWO FINGER PAN
		}
		
	}
}

void Building3DLayer::ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *events)
{
	CCSetIterator it = touches->begin();
	
	for(;it != touches->end();++it)
	{
		CCTouch* touch = static_cast<CCTouch*>(*it);
		active_touch_list[touch->getID()] = 0;
	}
}

void Building3DLayer::draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.05f, 0.25f, 0.45f, 1.f);
	_renderProgram->use();
	
	_renderProgram->setUniformLocationWith3f(_attrib[LIGHT_COLOR], 1.f, 1.f, 1.f);
	_renderProgram->setUniformLocationWithMatrix4fv(_attrib[MV], (GLfloat*)&_state.modelview, 1);
	_renderProgram->setUniformLocationWithMatrix4fv(_attrib[MVP], (GLfloat*)&_mvp, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, _bufferObject);

	glEnableVertexAttribArray(_attrib[VERTEX]);
	glEnableVertexAttribArray(_attrib[NORMAL]);
	glEnableVertexAttribArray(_attrib[AMBIENT_COLOR]);
	glEnableVertexAttribArray(_attrib[DIFFUSE_COLOR]);
	
	glVertexAttribPointer(_attrib[VERTEX], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, vX));
	glVertexAttribPointer(_attrib[NORMAL], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, nX));
	glVertexAttribPointer(_attrib[AMBIENT_COLOR], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, ar));
	glVertexAttribPointer(_attrib[DIFFUSE_COLOR], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, dr));

	unsigned i;
	for (i = 0; i < _scene.count-1; i++)
	{
		_renderProgram->setUniformLocationWith1i(_attrib[SELECTED], _scene.objects[i]->selected);
		glDrawArrays(GL_TRIANGLES, _scene.objects[i]->first, _scene.objects[i]->count);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	
	CCLayer::draw();
}
