#ifndef MAT_H_
#define MAT_H_

typedef struct Vec3f
{
	float x, y, z;
} Vec3f;


typedef struct Vec4f
{
	float x, y, z, w;
} Vec4f;

typedef struct Mat4f
{
	float e11, e21, e31, e41,
	e12, e22, e32, e42,
	e13, e23, e33, e43,
	e14, e24, e34, e44;
} Mat4f;

void identity(Mat4f *mat);

void translate(Mat4f *mat, float x, float y, float z);
void scale(Mat4f *mat, float x, float y, float z);

void rotate(Mat4f *mat, float degrees, float x, float y, float z);
void rotateX(Mat4f *mat, float degrees);
void rotateY(Mat4f *mat, float degrees);
void rotateZ(Mat4f *mat, float degrees);

void perspective(Mat4f *mat, float fovy, float aspect, float zNear, float zFar);

void multiply(Mat4f *result, Mat4f const *a, Mat4f const *b);

#endif
