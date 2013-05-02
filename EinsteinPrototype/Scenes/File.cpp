//
//  File.cpp
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 28/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#include "File.h"
 
TestGL::TestGL()
{ 
    init();
    initFixedMenu();
}

void TestGL::init()
{
    CCLOG("Init..");
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor);
    setShaderProgram(program);
    
    o.loadOBJ();

}


void TestGL::draw()
{

    glClearColor(255, 255, 255, 0);
 
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    
	getShaderProgram()->use();
     
    
    CCDirector::sharedDirector()->setDepthTest(true);
    
    
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    
	float scale = size.width/480.0f;
    
	kmMat4 matrixP;
    kmMat4 matrixMV;
    kmMat4 matrixMVP;
    
    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
    
	kmQuaternion quat;
	kmQuaternionRotationYawPitchRoll(&quat, 0, 0, 0);
    
	kmMat3 rotation;
	kmMat3RotationQuaternion(&rotation, &quat);
    
	kmVec3 translation;
	kmVec3Fill(&translation, size.width/2-40, size.height/2.2f, 10000);
    
	kmMat4 rotationAndMove;
	kmMat4RotationTranslation(&rotationAndMove, &rotation, &translation);
    
    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);				// apply rotation and translation to the matrix
    
	GLuint matrixId = glGetUniformLocation(getShaderProgram()->getProgram(), "u_MVPMatrix");
    getShaderProgram()->setUniformLocationwithMatrix4fv(matrixId, matrixMVP.mat, 1);
    
 /*
    
	ccVertex3F vertices[4];
	ccVertex2F uv[4];
    
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uv);
    
	float x = 0;
	float y = 0;
	float len = 40*scale;
    
	/////////// front
	vertices[0] = vertex3(x-len,y-len,len);
	vertices[1] = vertex3(x-len,y+len,len);
	vertices[2] = vertex3(x+len,y-len,len);
	vertices[3] = vertex3(x+len,y+len,len);
    
	uv[0] = vertex2(0, 1);
	uv[1] = vertex2(0, 0);
	uv[2] = vertex2(1, 1);
	uv[3] = vertex2(1, 0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/
    
    o.Render();
     
    
    CC_INCREMENT_GL_DRAWS(1);
    
}