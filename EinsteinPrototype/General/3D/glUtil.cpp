#include "glUtil.h"
#include "fileUtil.h"

GLuint createShader(const char *shaderSource, GLenum shaderType);
 

GLuint createProgram(char const *vshFileName, char const *fshFileName)
{
	GLint info;
	char *log;

	GLuint vsh, fsh, program;
	char *vshCode, *fshCode;

	vshCode = readTextFile(vshFileName);
	if (vshCode == NULL)
	{
		//fprintf(stderr, "Nao foi possivel ler o arquivo \"%s\"\n", vshFileName);
		return 0u;
	}

	fshCode = readTextFile(fshFileName);
	if (fshCode == NULL)
	{
		//fprintf(stderr, "Nao foi possivel ler o arquivo \"%s\"\n", fshFileName);
		return 0u;
	}

	vsh = createShader(vshCode, GL_VERTEX_SHADER);
	free(vshCode);

	if (vsh == 0u)
	{
		free(fshCode);
		return 0u;
	}

	fsh = createShader(fshCode, GL_FRAGMENT_SHADER);
	free(fshCode);

	if (fsh == 0u)
	{
		return 0u;
	}

	program = glCreateProgram();
	if (program == 0u)
	{
		//fprintf(stderr, "Nao foi possivel criar um objeto programa (shader).\n");
		glDeleteShader(vsh);
		glDeleteShader(fsh);
		return 0u;
	}

	glAttachShader(program, vsh);
	glAttachShader(program, fsh);
	glLinkProgram(program);
	glDeleteShader(vsh);
	glDeleteShader(fsh);

	glGetProgramiv(program, GL_LINK_STATUS, &info);
	if (info == GL_FALSE)
	{
		//fprintf(stderr, "Nao foi possivel linkar um programa:\n");

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info);

		log = (char*)malloc(info);

		glGetProgramInfoLog(program, info, &info, log);
		printf("%s\n", log);

		glDeleteProgram(program);
		return 0u;
	}

	return program;
}

GLuint createShader(const char *shaderSource, GLenum shaderType)
{
	GLuint obj;
	GLint info;
	char *log;

	obj = glCreateShader(shaderType);
	if (obj == 0u)
	{
		//fprintf(stderr, "Nao foi possivel criar um objeto shader.\n");
		return 0u;
	}

	glShaderSource(obj, 1, &shaderSource, NULL);
	glCompileShader(obj);

	glGetShaderiv(obj, GL_COMPILE_STATUS, &info);
	if (info == GL_FALSE)
	{
		//fprintf(stderr, "Nao foi possivel compilar um shader:\n");
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &info);
		log = (char*)malloc(info);
		glGetShaderInfoLog(obj, info, &info, log);
		free(log);
		glDeleteShader(obj);
		return 0u;
	}
	return obj;
}
