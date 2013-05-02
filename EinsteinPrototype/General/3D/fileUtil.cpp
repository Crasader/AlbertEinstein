    #include "fileUtil.h"

#include "cocos2d.h"
using namespace cocos2d;

void createScene(SCENE *scene, void **outdata)
{
	unsigned long fileSize;
    unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("model.b"), "rb", &fileSize);
	void *fdata = data;
    scene->count = 0;
	memcpy(&scene->count, data, sizeof(unsigned));
	 
    
    data+= sizeof(unsigned);
	scene->objects = (OBJECT**)malloc(sizeof(OBJECT*) * scene->count);
    for (int i = 0; i < scene->count; i++)
	{
		scene->objects[i] = (OBJECT*)malloc(sizeof(OBJECT));
		memcpy(scene->objects[i],  data, sizeof(OBJECT));
		data += sizeof(OBJECT);
	}

	*outdata = malloc(sizeof(VERTEXDATA) * 118083);
	memcpy(*outdata, data, sizeof(VERTEXDATA) * 118083);
	free(fdata);
}

void clearScene(SCENE *scene)
{
	unsigned i;
	for (i = 0; i < scene->count; i++)
	{
		free(scene->objects[i]);
	}
	free(scene->objects);
}

char* readTextFile(char const *fileName)
{
	unsigned long fileSize;
	char *text = (char*)CCFileUtils::sharedFileUtils()->getFileData(fileName, "r", &fileSize);

	if (text == NULL) return NULL;

	text = (char*)realloc(text, fileSize + 1);
	text[fileSize] = '\0';

	return text;
}
