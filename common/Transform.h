//
//  Transform.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 29/07/12.
//
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <math.h>

static inline float * MatrixMakeIdentity()
{
    float * matrix = new float[16];
    for (int i = 0; i < 16; i++) {
		matrix[i] = 0.0f;
	}
	
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
    
    return matrix;
}

//static float * MatrixIdentity = MatrixMakeIdentity();

static inline void MatrixTransformIdentity(float ** matrix) {
    
    for (int i = 0; i < 16; i++) {
		*(*matrix+i) = 0.0f;
	}
	
	*(*matrix+0) = *(*matrix+5) = *(*matrix+10) = *(*matrix+15) = 1.0;
}

static inline float * MatrixMultiply(float * _matrixLeft, float * _matrixRight)
{
	float * matrixResult = new float[16];
	
    for (int i = 0; i < 16; i++) {
        matrixResult[i] = _matrixLeft[i % 4] * _matrixRight[i / 4 * 4] +
                          _matrixLeft[(i % 4) + 4] * _matrixRight[i / 4 * 4 + 1] +
                          _matrixLeft[(i % 4) + 8] * _matrixRight[i / 4 * 4 + 2] +
                          _matrixLeft[(i % 4) + 12] * _matrixRight[i / 4 * 4 + 3];
    }
    
    
	return matrixResult;
}

static inline void MatrixTranslate(float * matrix, float _x, float _y, float _z)
{
	// translate (tx, ty, tz)  matrix identity
	matrix[12] = _x;
	matrix[13] = _y;
	matrix[14] = _z;
	//matrix[15] = _w;
}

static inline void MatrixTranslate(float * matrix, float _x, float _y)
{
    MatrixTranslate(matrix, _x, _y, 0.0f);
}

static inline void MatrixTranslate(float * matrix, Pointer * _pointer)
{
    MatrixTranslate(matrix, _pointer->x, _pointer->y, _pointer->z);
}

static inline float * MatrixMakeTranslate(float _x, float _y)
{
    float * matrix = MatrixMakeIdentity();
    MatrixTranslate(matrix, _x, _y);
    return matrix;
}

static inline void MatrixScale(float * matrix, float _x, float _y, float _z)
{
	// scale (sx, sy, sz) matrix identidade
	matrix[0]  = _x; //(_x / 100);
	matrix[5]  = _y; //(_y / 100);
	matrix[10] = _z;
}

static inline void MatrixScale(float * matrix, float _x, float _y)
{
    MatrixScale(matrix, _x, _y, 0.0f);
}

static inline float * MatrixMakeScale(float _x, float _y)
{
    float * matrix = MatrixMakeIdentity();
    MatrixScale(matrix, _x, _y, 0.0f);
    return matrix;
}

static inline float * MatrixMakeXRotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);
    
    float * matrix = MatrixMakeIdentity();
    matrix[5] = cos;
    matrix[6] = sin;
    matrix[9] = -sin;
    matrix[10] = cos;
    
    return matrix;
}

static inline float * MatrixMakeYRotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);
    
    float * matrix = MatrixMakeIdentity();
    matrix[0] = cos;
    matrix[2] = -sin;
    matrix[8] = sin;
    matrix[10] = cos;
    
    return matrix;
}

static inline float * MatrixMakeZRotation(float radians)
{
    float cos = cosf(radians);
    float sin = sinf(radians);
    
    float * matrix = MatrixMakeIdentity();
    matrix[0] = cos;
    matrix[1] = sin;
    matrix[4] = -sin;
    matrix[5] = cos;
    
    return matrix;
}

static inline Pointer * MatrixTransformPoint(const float * matrix, const Pointer * _pointer)
{
	float x = matrix[0] * _pointer->x + matrix[4] * _pointer->y + matrix[8]  * _pointer->z + matrix[12] * _pointer->w;
	float y = matrix[1] * _pointer->x + matrix[5] * _pointer->y + matrix[9]  * _pointer->z + matrix[13] * _pointer->w;
	float z = matrix[2] * _pointer->x + matrix[6] * _pointer->y + matrix[10] * _pointer->z + matrix[14] * _pointer->w;
	float w = matrix[3] * _pointer->x + matrix[7] * _pointer->y + matrix[11] * _pointer->z + matrix[15] * _pointer->w;
    
	return MakePointer(x, y, z, w);
}

static inline void MatrixOrtho(float * matrix, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    MatrixTransformIdentity(&matrix);
    
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = farZ + nearZ;
    float fsn = farZ - nearZ;
    
    matrix[0] = 2.0f / rsl;
    matrix[5] = 2.0f / tsb;
    matrix[10] = -2.0f / fsn;
    matrix[12] = -ral / rsl;
    matrix[13] = -tab / tsb;
    matrix[14] = -fan / fsn;
}

static inline float * MatrixMakeOrtho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float * matrix = new float(16);
    MatrixOrtho(matrix, left, right, bottom, top, nearZ, farZ);
    return matrix;
}
 
#endif
