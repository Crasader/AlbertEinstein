#include "mat.h"

#include <string.h>
#include <math.h>

#define DEGREEStoRAD 3.14159265f/180.f

void identity(Mat4f *mat)
{
	memset(mat, 0, sizeof(Mat4f));

	mat->e11 = mat->e22 = mat->e33 = mat->e44 = 1.f;
}

void translate(Mat4f *mat, float x, float y, float z)
{
	identity(mat);

	mat->e14 = x;
	mat->e24 = y;
	mat->e34 = z;
}

void scale(Mat4f *mat, float x, float y, float z)
{
	identity(mat);

	mat->e11 = x;
	mat->e22 = y;
	mat->e33 = z;
}

void rotate(Mat4f *mat, float degrees, float x, float y, float z)
{
	identity(mat);

	degrees *= DEGREEStoRAD;

	float s = sin(degrees);
	float c = cos(degrees);
	float ce = 1 - c;

	mat->e11 = x * x * ce + c;
	mat->e21 = y * x * ce + z * s;
	mat->e31 = x * z * ce - y * s;

	mat->e12 = x * y * ce - z * s;
	mat->e22 = y * y * ce + c;
	mat->e32 = y * z * ce + x * s;

	mat->e13 = x * z * ce + y * s;
	mat->e23 = y * z * ce - x * s;
	mat->e33 = z * z * ce + c;
}

void rotateX(Mat4f *mat, float degrees)
{
	identity(mat);

	degrees *= DEGREEStoRAD;

	mat->e22 = mat->e33 = cosf(degrees);
	mat->e32 = sinf(degrees);
	mat->e23 = -mat->e32;
}

void rotateY(Mat4f *mat, float degrees)
{
	identity(mat);

	degrees *= DEGREEStoRAD;

	mat->e11 = mat->e33 = cosf(degrees);
	mat->e13 = sinf(degrees);
	mat->e31 = -mat->e13;
}

void rotateZ(Mat4f *mat, float degrees)
{
	identity(mat);

	degrees *= DEGREEStoRAD;

	mat->e11 = mat->e22 = cosf(degrees);
	mat->e21 = sinf(degrees);
	mat->e12 = -mat->e21;
}

void perspective(Mat4f *mat, float fovy, float aspect, float zNear, float zFar)
{
	identity(mat);

	fovy *= DEGREEStoRAD;

	float cot = 1.f / tanf(fovy/2);
	
	mat->e11 = cot/aspect;
	mat->e22 = cot;
	mat->e33 = (zNear + zFar) / (zNear - zFar);
	mat->e34 = 2 * zNear * zFar / (zNear - zFar);
	mat->e43 = -1.f;
	mat->e44 = 0.f;
}

void multiply(Mat4f *result, Mat4f const *a, Mat4f const *b)
{
	result->e11 = a->e11 * b->e11 + a->e12 * b->e21 + a->e13 * b->e31 + a->e14 * b->e41;
	result->e12 = a->e11 * b->e12 + a->e12 * b->e22 + a->e13 * b->e32 + a->e14 * b->e42;
	result->e13 = a->e11 * b->e13 + a->e12 * b->e23 + a->e13 * b->e33 + a->e14 * b->e43;
	result->e14 = a->e11 * b->e14 + a->e12 * b->e24 + a->e13 * b->e34 + a->e14 * b->e44;

	result->e21 = a->e21 * b->e11 + a->e22 * b->e21 + a->e23 * b->e31 + a->e24 * b->e41;
	result->e22 = a->e21 * b->e12 + a->e22 * b->e22 + a->e23 * b->e32 + a->e24 * b->e42;
	result->e23 = a->e21 * b->e13 + a->e22 * b->e23 + a->e23 * b->e33 + a->e24 * b->e43;
	result->e24 = a->e21 * b->e14 + a->e22 * b->e24 + a->e23 * b->e34 + a->e24 * b->e44;

	result->e31 = a->e31 * b->e11 + a->e32 * b->e21 + a->e33 * b->e31 + a->e34 * b->e41;
	result->e32 = a->e31 * b->e12 + a->e32 * b->e22 + a->e33 * b->e32 + a->e34 * b->e42;
	result->e33 = a->e31 * b->e13 + a->e32 * b->e23 + a->e33 * b->e33 + a->e34 * b->e43;
	result->e34 = a->e31 * b->e14 + a->e32 * b->e24 + a->e33 * b->e34 + a->e34 * b->e44;

	result->e41 = a->e41 * b->e11 + a->e42 * b->e21 + a->e43 * b->e31 + a->e44 * b->e41;
	result->e42 = a->e41 * b->e12 + a->e42 * b->e22 + a->e43 * b->e32 + a->e44 * b->e42;
	result->e43 = a->e41 * b->e13 + a->e42 * b->e23 + a->e43 * b->e33 + a->e44 * b->e43;
	result->e44 = a->e41 * b->e14 + a->e42 * b->e24 + a->e43 * b->e34 + a->e44 * b->e44;
}
