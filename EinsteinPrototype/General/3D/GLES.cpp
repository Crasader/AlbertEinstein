#include "GLES.h"

#include "glUtil.h"

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
}
