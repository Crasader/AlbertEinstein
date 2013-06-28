#include "GLES.h"

//#include "Building3DLayer.h"
#include "SimpleAudioEngine.h"
#include "OpenGL_Internal.h"
//#include "CCPanGestureRecognizer.h"
//#include "CCPinchGestureRecognizer.h"
//#include <Array>

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
		"\
			attribute vec3 vertex;\
			attribute vec3 normal;\
			attribute vec3 ambientColor;\
			attribute vec3 diffuseColor;\
			\
			varying vec4 color;\
			\
			uniform mat4 mv;\
			uniform mat4 mvp;\
			uniform vec3 lightColor;\
			uniform bool selected;\
			\
			void main(void)\
			{\
				vec4 lightPosition = vec4(0.0, 1000.0, 3000.0, 1.0);\
				\
				vec4 vertexPosition = mv * vec4(vertex, 1.0);\
				vec4 mvNormal = mv * vec4(normal, 0.0);\
				\
				vec4 lightDirection = normalize(lightPosition - vertexPosition);\
				\
				float factor = max(dot(mvNormal, lightDirection), 0.0);\
				\
				vec3 bColor;\
				\
				if (selected)\
				{\
					bColor = ambientColor * lightColor + diffuseColor * lightColor * factor * 0.5;\
				}\
				else\
				{\
					bColor = ambientColor * lightColor + diffuseColor * lightColor * factor;\
				}\
				\
				color = vec4(bColor, 1.0);\
				\
				gl_Position = mvp * vec4(vertex, 1.0);\
			}",
			"#ifdef GL_ES\n\
				precision highp float;\n\
			#endif\n\
			\
			varying vec4 color;\
			\
			void main(void)\
			{\
				gl_FragColor = color;\
			}"
	);
	_renderProgram->link();
	
	_pickProgram = new CCGLProgram;
	_pickProgram->initWithVertexShaderByteArray(
		"\
			attribute vec3 vertex;\
			\
			varying vec4 color;\
			\
			uniform mat4 mvp;\
			uniform vec3 pickColor;\
			\
			void main(void)\
			{\
				color = vec4(pickColor, 1.0);\
				\
				gl_Position = mvp * vec4(vertex, 1.0);\
			}",
			"\
			#ifdef GL_ES\n\
				precision highp float;\n\
			#endif\n\
			\
			varying vec4 color;\
			\
			void main(void)\
			{\
				gl_FragColor = color;\
			}"
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
	
    return true;
}

void Building3DLayer::didPan(CCObject* obj)
{
	
}

void Building3DLayer::didPinch(CCObject* obj)
{

}

std::vector<int> active_touch_list;

CCPoint last_location;

int numberOfActiveTouches(const std::vector<int>& arr)
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

		CCPoint delta = touch->locationInView();
		delta.x -= last_location.x;
		delta.y -= last_location.y;
		last_location = touch->locationInView();
		
		rotate(delta.x,delta.y);
	}
	if(numActiveTouches() == 2)
	{
		static float last_distance = 40;
		std::vector<CCTouch*> twotouches;
		int i = 0;
		for(;it!=touches->end();++it)
		{
			twotouches[i++] = static_cast<CCTouch*>(*it);
		}
		
		CCPoint d1 = twotouches[0]->locationInView();
		CCPoint d2 = twotouches[1]->locationInView();
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
	_renderProgram->setUniformLocationwithMatrix4fv(_attrib[MV], (GLfloat*)&_state.modelview, 1);
	_renderProgram->setUniformLocationwithMatrix4fv(_attrib[MVP], (GLfloat*)&_mvp, 1);
	
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


/*#include "glUtil.h"

#define DISTZ -500.f
#define DISTY -50.f
#define DISTX 0

#define COCOS2D_DEBUG 1


GLES::GLES()
{
    init();
}

GLES::~GLES()
{
	clearScene(&scene);
	glDeleteBuffers(1, &bufferObject);
	glDeleteProgram(lightProgram);
	glDeleteProgram(pickProgram);
}

bool GLES::init()
{
	if (!CCLayer::init()) return false;
	this->setTouchEnabled(true);

	this->touchCount = 0u;
	this->moved = false;
	this->pX = this->pZ = this->aX = this->aY = 0.f;
	this->selectedObject = NULL;
    
    perspective(&projection, 60.f, CCDirector::sharedDirector()->getWinSize().width/CCDirector::sharedDirector()->getWinSize().height, 1.f, 100000.f);

	//lightProgram
	this->lightProgram = createProgram(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("light.vsh"), CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("default.fsh"));
    this->lVertex = glGetAttribLocation(lightProgram, "vertex");
	this->lNormal = glGetAttribLocation(lightProgram, "normal");
    this->lMV = glGetUniformLocation(lightProgram, "mv");
	this->lMVP = glGetUniformLocation(lightProgram, "mvp");
	this->lAmbientColor = glGetUniformLocation(lightProgram, "ambientColor");
	this->lDiffuseColor = glGetUniformLocation(lightProgram, "diffuseColor");
	this->lLightColor = glGetUniformLocation(lightProgram, "lightColor");

	//pickProgram
	this->pickProgram = createProgram(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("pick.vsh"), CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("default.fsh"));
	this->lPickVertex = glGetAttribLocation(pickProgram, "vertex");
	this->lPickMVP    = glGetUniformLocation(pickProgram, "mvp");
	this->lPickColor  = glGetUniformLocation(pickProgram, "pickColor");
	//SCENE
	void *data;
	createScene(&this->scene, &data);
	//bufferObject
	glGenBuffers(1, &this->bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, 118083 * sizeof(VERTEXDATA), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
    free(data);

	
	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.25, 0.25, 0.25, 1.0);

	return true;
}

void GLES::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    this->touchCount += pTouches->count();
}

void GLES::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it;
	CCPoint delta;
	CCTouch *touches[2];

	moved = true;

	if (pTouches->count() == 1)
	{
		touches[0] = (CCTouch*)pTouches->anyObject();
        delta =ccpSub(
                      CCDirector::sharedDirector()->convertToGL(touches[0]->locationInView()),
                      CCDirector::sharedDirector()->convertToGL(touches[0]->previousLocationInView()));
    
		aX -= delta.y / 2.f;
		aY += delta.x / 2.f;
	}
	else if(pTouches->count() == 2)
	{

		touches[0] = (CCTouch*)pTouches->anyObject();
	    delta =ccpSub(
                      CCDirector::sharedDirector()->convertToGL(touches[0]->locationInView()),
                      CCDirector::sharedDirector()->convertToGL(touches[0]->previousLocationInView()));
		CCLog("delta: %f %f", delta.x, delta.y);
		pX += delta.x;
		pZ -= delta.y;
	}
}

void GLES::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCPoint touch;
	this->touchCount -= pTouches->count();

	if (this->touchCount == 0)
	{
		if (moved)
			moved = false;
		else if (pTouches->count() == 1)
		{
			touch = ((CCTouch*)pTouches->anyObject())->locationInView();
			CCLog("point: %f %f", touch.x, touch.y);

			this->selectObject(touch.x, touch.y);
			//pick
		}
	}
}

void GLES::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{

}

void GLES::draw()
{
   	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    translate(&this->m1, this->pX + DISTX, DISTY, this->pZ + DISTZ);
	
    rotateY(&this->m2, this->aY);
	rotateX(&this->m3, this->aX);

	multiply(&this->m4, &this->m3, &this->m2);
	multiply(&this->view, &this->m1, &this->m4);
	multiply(&this->mvp, &this->projection, &this->view);

	glUseProgram(this->lightProgram);
	glUniformMatrix4fv(this->lMV, 1, GL_FALSE, (GLfloat*)&this->view);
	glUniformMatrix4fv(this->lMVP, 1, GL_FALSE, (GLfloat*)&this->mvp);

	glEnableVertexAttribArray(this->lVertex);
	glEnableVertexAttribArray(this->lNormal);

	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObject);

	glVertexAttribPointer(this->lVertex, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, vX));
	glVertexAttribPointer(this->lNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, nX));

	unsigned i;
	for (i = 0; i < scene.count; i++)
	{
		glUniform3f(this->lDiffuseColor,
			this->scene.objects[i]->difColor[0] / 255.f, this->scene.objects[i]->difColor[1] / 255.f, this->scene.objects[i]->difColor[2] / 255.f);
		glUniform3f(this->lAmbientColor,
			this->scene.objects[i]->ambColor[0] / 255.f, this->scene.objects[i]->ambColor[1] / 255.f, this->scene.objects[i]->ambColor[2] / 255.f);
		glUniform3f(this->lLightColor, 1.f, 1.f, 1.f);
        glDrawArrays(GL_TRIANGLES, this->scene.objects[i]->first, this->scene.objects[i]->count);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0u);
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
    CC_NODE_DRAW_SETUP();
    CC_INCREMENT_GL_DRAWS(1);
}

void GLES::selectObject(int x, int y)
{
	unsigned i;
    
	glClearColor(0.f, 0.f, 0.f, 0.f);
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glUseProgram(pickProgram);
    
	glUniformMatrix4fv(lPickMVP, 1, GL_FALSE, (GLfloat*)&mvp);
    
	glEnableVertexAttribArray(lPickVertex);
    
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
    
	glVertexAttribPointer(lPickVertex, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, vX));
    
	for (i = 0; i < scene.count; i++)
	{
        glUniform3f(
                    lPickColor,scene.objects[i]->pickColor[0] / 255.f,
                    scene.objects[i]->pickColor[1] / 255.f,
                    scene.objects[i]->pickColor[2] / 255.f
                    );
		glDrawArrays(GL_TRIANGLES, scene.objects[i]->first, scene.objects[i]->count);
	}
    
    glBindBuffer(GL_ARRAY_BUFFER, 0u);

    GLubyte color[3];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    CCLog("%d %d %d", int(color[0]), int(color[1]), int(color[2]));
    
	if (color[0] == 0 && selectedObject != NULL)
	{
		selectedObject->difColor[0] = selectedObject->difColor[1] = selectedObject->difColor[2] = 220;
		selectedObject = NULL;
	}
	else
	{
		for (i = 0; i < scene.count; i++)
		{
			if (scene.objects[i]->pickColor[0] == color[0])
			{
				if (selectedObject != scene.objects[i] && selectedObject != NULL)
					selectedObject->difColor[0] = selectedObject->difColor[1] = selectedObject->difColor[2] = 220;
                
				selectedObject = scene.objects[i];
				selectedObject->difColor[0] = selectedObject->difColor[1] = selectedObject->difColor[2] = 70;
			}
		}
	} 
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
    CC_NODE_DRAW_SETUP();
}*/
