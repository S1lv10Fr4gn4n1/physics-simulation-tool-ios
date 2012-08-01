//
//  Transform.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 29/07/12.
//
//

#ifndef TRANSFORM_H
#define TRANSFORM_H

static inline float * MatrixMakeIdentity()
{
    float * matrix = new float[16];
    for (int i = 0; i < 16; i++) {
		matrix[i] = 0.0f;
	}
	
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
    
    return matrix;
}

static float * MATRIX_IDENTITY = MatrixMakeIdentity();

static inline void MatrixTransformIdentity(float * matrix) {
    for (int i = 0; i < 16; i++) {
		matrix[i] = 0.0f;
	}
	
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
}

static inline float * MatrixMultiplay(float * _matrixLeft, float * _matrixRight)
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
	//matrix[15] = _w;
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

static inline float * MatrixRotate(float _x)
{
    float * matrix = MatrixMakeIdentity();
    
	// totation in Z
	float teta = (M_PI * _x * 10) / 180.0;
	matrix[0] =  cosf(teta);
	matrix[1] =  sinf(teta);
	matrix[4] = -sinf(teta);
	matrix[5] =  cosf(teta);
    
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

// method responsible for transforming an original point to a point resulting from the transformation
static inline Pointer * MatrixTransformPoint(float * matrix, Pointer * _point)
{
	float x = matrix[0] * _point->x + matrix[4] * _point->y + matrix[8]  * _point->z + matrix[12] * _point->w;
	float y = matrix[1] * _point->x + matrix[5] * _point->y + matrix[9]  * _point->z + matrix[13] * _point->w;
	float z = matrix[2] * _point->x + matrix[6] * _point->y + matrix[10] * _point->z + matrix[14] * _point->w;
	float w = matrix[3] * _point->x + matrix[7] * _point->y + matrix[11] * _point->z + matrix[15] * _point->w;

	return MakePointer(x, y, z, w);
}
 
#endif
