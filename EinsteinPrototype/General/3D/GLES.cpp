#include "GLES.h"

//#include "Building3DLayer.h"
#include "SimpleAudioEngine.h"
//#include "OpenGL_Internal.h"
//#include "CCPanGestureRecognizer.h"
//#include "CCPinchGestureRecognizer.h"
//#include <Array>
#include "BlockListScene.h"
#include <algorithm>

using namespace cocos2d;
using namespace CocosDenshion;

SCENE _scene;
STATE _state;
kmMat4 _projection, _mvp;

GLuint _bufferObject = 0;
GLuint _attrib[11];

cocos2d::CCGLProgram* _renderProgram = NULL;
cocos2d::CCGLProgram* _pickProgram = NULL;

bool shouldPick = false;
CCPoint pickPoint(0,0);

#define DIST(x, y) (sqrtf(((x)*(x)) + ((y)*(y))))
#define numActiveTouches() numberOfActiveTouches(active_touch_list)

bool active = false;

CCScene* Building3DLayer::scene()
{
    CCScene *scene = CCScene::create();
    /*Building3DLayer *layer = Building3DLayer::create();
    scene->addChild(layer);*/

    return scene;
}

bool Building3DLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	active = true;
	
	kmMat4 testMat;
	kmMat4Identity(&testMat);
	
	mTexture = CCTextureCache::sharedTextureCache()->addImage("HelloWorld.png");

	this->setTouchEnabled(true);
	
	if(!_renderProgram){
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
	}
	
	if(!_pickProgram) {
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
	}
	
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

std::string Building3DLayer::readBinaryFile(const std::string& filename)
{
	std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(filename.c_str());
	
	size_t filesize;
	unsigned char* data = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &filesize);
	
	return std::string(data, data+filesize);
}

GLuint Building3DLayer::createBuffer(const std::string& filename)
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

void Building3DLayer::loadDefaultState()
{
	active = true;
	//kmMat4Translation(&_state.modelview, -50, -50.f, -250.f);
	kmMat4Translation(&_state.modelview, 0, 0.f, -600.f);
	_state.rX = 120.f;
	
	kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
	
	shouldPick = false;
	
	 float cpymvp[] = {2.59177,-0.0653749,-0.0588968,-0.058885,-2.72216e-09,1.45824,-0.53974,-0.539632,-0.18172,-0.9324,-0.840007,-0.839839,-5.19626,17.2396,334.354,336.287};
	 std::copy(cpymvp,cpymvp+16,_mvp.mat);
	 
	 float cpyproj[] = {2.59813,0,0,0,0,1.73209,0,0,0,0,-1.0002,-1,0,0,-2.0002,0};
	 std::copy(cpyproj,cpyproj+16,_projection.mat);
	 
	 float cpymv[] = {0.997551,-0.0377434,0.058885,0,-1.04774e-09,0.841901,0.539632,0,-0.0699428,-0.538311,0.839839,0,-2,9.95307,-336.287,1};
	 std::copy(cpymv,cpymv+16,_state.modelview.mat);
}
void Building3DLayer::move(float x, float y)
{
	if(!active) return;
   
	kmMat4 copy, trans;
	
	y *= -1;
	
	kmMat4Assign(&copy, &_state.modelview);
	kmMat4Translation(&trans, x, 0, 0);
	kmMat4Multiply(&_state.modelview, &trans, &copy);
	
	kmMat4Assign(&copy, &_state.modelview);
	kmMat4RotationX(&trans, _state.rX);
	//kmMat4Translation(&trans, trans.mat[3*4+1] * y, trans.mat[3*4+2] * y, trans.mat[3*4+3] * y);
	kmMat4Translation(&trans, trans.mat[2] * y, trans.mat[6] * y, trans.mat[10] * y);
	kmMat4Multiply(&_state.modelview, &trans, &copy);
	
	kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
	
	std::cout<<"\nMVP: {";
	for(int i = 0; i < 16; i++) {
		std::cout<<_mvp.mat[i]<<",";
	}
	std::cout<<"}\nProjection: {";
	for(int i = 0; i < 16; i++) {
		std::cout<<_projection.mat[i]<<",";
	}
	std::cout<<"}\nModel View: {";
	for(int i = 0; i < 16; i++) {
		std::cout<<_state.modelview.mat[i]<<",";
	}
}

void Building3DLayer::rotate(float x, float y)
{
	if(!active) return;
	kmMat4 copy, rot;
	
	x /= 100.f;
	y /= 100.f;
	
	//float piover2 = 3.14/2.f;
	
	kmMat4Assign(&copy, &_state.modelview);
	kmMat4RotationY(&rot, x);
	kmMat4Multiply(&_state.modelview, &copy, &rot);
	
	kmMat4Assign(&copy, &_state.modelview);
	_state.rX += y;
	kmVec3 axis = {copy.mat[0],copy.mat[4],copy.mat[8]};
	kmMat4RotationAxisAngle(&rot, &axis, y);
	
	kmMat4Multiply(&_state.modelview, &copy, &rot);
	
	kmMat4Multiply(&_mvp, &_projection, &_state.modelview);

}

void Building3DLayer::zoom(float sc)
{
	sc *= 2;
	
	kmMat4 copy, trans;

	kmMat4Assign(&copy, &_state.modelview);
	kmMat4Translation(&trans, 0.f, 0.f, sc);
	kmMat4Multiply(&_state.modelview, &trans, &copy);

	kmMat4Multiply(&_mvp, &_projection, &_state.modelview);
}

void Building3DLayer::pick(int x, int y)
{
     return;
	//if(!active) return;
    
	int ix, iy;

	ix = x;
	iy = y;

	static OBJECT *selectedObject = NULL;
	unsigned i;

	//glClearColor(0.f, 0.f, 0.f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(_pickProgram->getProgram());

	glUniformMatrix4fv(_attrib[PICK_MVP], 1, GL_FALSE, (GLfloat*)&_mvp);

	glEnableVertexAttribArray(_attrib[PICK_VERTEX]);

	glBindBuffer(GL_ARRAY_BUFFER, _bufferObject);

	glVertexAttribPointer(_attrib[PICK_VERTEX], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEXDATA), (void*)offsetof(VERTEXDATA, vX));

	for (i = 0; i < _scene.count; i++)
	{
		glUniform3f(_attrib[PICK_COLOR],
			_scene.objects[i]->pickColor[0] / 255.f, _scene.objects[i]->pickColor[1] / 255.f, _scene.objects[i]->pickColor[2] / 255.f);
		glDrawArrays(GL_TRIANGLES, _scene.objects[i]->first, _scene.objects[i]->count);
	}

	glDisableVertexAttribArray(_attrib[PICK_VERTEX]);

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glUseProgram(0u);

	GLubyte color[4];
	CCSize windowsize = CCDirector::sharedDirector()->getWinSize();
	
	int scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
	
	glReadPixels(std::max(0,ix * scaleFactor - 1), std::max<int>(0,(windowsize.height - iy)*scaleFactor-1), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

	if (color[0] == 0 && selectedObject != NULL)
	{
		selectedObject->selected = 0;
		selectedObject = NULL;
	}
	else
	{
		for (i = 0; i < _scene.count; i++)
		{
			if (_scene.objects[i]->pickColor[0] == color[0])
			{
				if (selectedObject != _scene.objects[i] && selectedObject != NULL)
				{
					selectedObject->selected = 0;
				}

				selectedObject = _scene.objects[i];
				
				if (strcmp(selectedObject->name, "Mesh6_Group5_Model") == 0 ||
					strcmp(selectedObject->name, "Mesh3_Group2_Model") == 0 ||
					strcmp(selectedObject->name, "Mesh1_Group1_Model") == 0 ||
					strcmp(selectedObject->name, "Mesh4_Group4_Model") == 0 ||
					strcmp(selectedObject->name, "Mesh25_base_implanta__o_REV12_0_A_C1F7F7448_1_Model") == 0)
				{
					selectedObject->selected = 1;
					/*
						Mesh6_Group5_Model = A1
						Mesh3_Group2_Model = A
						Mesh1_Group1_Model = B
						Mesh4_Group4_Model = D
						Mesh25_base_implanta__o_REV12_0_A_C1F7F7448_1_Model = E
					*/
					
					int blockID;
					std::string selectedObjectName(selectedObject->name);
					
					if(selectedObjectName == "Mesh6_Group5_Model") blockID = 1;
					if(selectedObjectName == "Mesh3_Group2_Model") blockID = 2;
					if(selectedObjectName == "Mesh1_Group1_Model") blockID = 3;
					if(selectedObjectName == "Mesh4_Group4_Model") blockID = 4;
					if(selectedObjectName == "Mesh25_base_implanta__o_REV12_0_A_C1F7F7448_1_Model") blockID = 5;
					
					bool e = false;
					active = false;
                    BlockListScene::createLayerChild(blockID,&e,&active);
					
				}
			}
		}
	}
}

void Building3DLayer::tap(int x, int y)
{
	shouldPick = true;
	pickPoint = CCPoint(x,y);
}

void Building3DLayer::draw()
{
	if(!active) return;

	glEnable(GL_DEPTH_TEST);
	
	if(shouldPick)
	{
		pick(pickPoint.x, pickPoint.y);
		shouldPick = false;
	}
	
	glEnable(GL_CULL_FACE);
	glClearColor(1.f,1.f,1.f,1.f);
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

#undef DIST
#undef numActiveTouches

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
