//
//  File.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 28/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__File__
#define __EinsteinPrototype__File__

#include <iostream>
#include "Enumerations.h"
#include "Shader.h"
#include "ReaderOBJFiles.h"
class TestGL : public IFixedMenu
{
public:
    void init();
 
    TestGL();
 
    
    ObjReader o;
    //locations for the vertex and color attribute in the shader
    int m_positionLocation, m_colorLocation;
    
     virtual void draw(void);
   
    GLuint vertexbuffer;
    
    GLuint VertexArrayID;
    
};
#endif /* defined(__EinsteinPrototype__File__) */
