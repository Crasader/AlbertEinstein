#ifndef FILEUTIL_H_
#define FILTEUTIL_H_

typedef struct
{
	float vX, vY, vZ;
	float nX, nY, nZ;
	float tU, tV;
} VERTEXDATA;

typedef struct
{
	char name[64];
	unsigned first, count;

	unsigned char ambColor[3], difColor[3];
	unsigned char pickColor[3];
} OBJECT;

typedef struct
{
	OBJECT **objects;
	unsigned count;
} SCENE;


void createScene(SCENE *scene, void **outdata);
void clearScene(SCENE *scene);

char* readTextFile(char const *fileName);


#endif
