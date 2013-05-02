//
//  ReaderOBJFiles.h
//  EinsteinPrototype
//
//  Created by Diego Mucciolo on 28/02/13.
//  Copyright (c) 2013 Farofa Studios. All rights reserved.
//

#ifndef __EinsteinPrototype__ReaderOBJFiles__
#define __EinsteinPrototype__ReaderOBJFiles__

#include <iostream>
#include <locale>
#include <algorithm>

#include "Enumerations.h"
struct Vertex
{
    float x, y, z;
};


struct UV
{
    float u, v;
};

class ObjReader
{
public:
    
    GLuint vertexbuffer;
    GLuint uvbuffer;
    std::vector<int> vertexIndices, uvIndices, normalIndices;
    std::vector<Vertex> lstVertices;
    std::vector<UV> lstUV;
    std::vector<Vertex> lstNormal;


    bool loadOBJ();
    
    void SetupBuffers();
    void Render();
};

 

#endif /* defined(__EinsteinPrototype__ReaderOBJFiles__) */
