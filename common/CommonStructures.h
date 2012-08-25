//
//  CommonStructures.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//
//

#ifndef COMMONTRUCTURES_H
#define COMMONTRUCTURES_H

#include <stdlib.h>
#include <math.h>
#include "Precision.h"

/***************************** Define *****************************/

#define CHANNEL_COLOR 4
#define COUNT_COORD 3


/***************************** Enumations *****************************/
enum TypeObject {
    CIRCLE,
    SQUARE,
    TRIANGLE,
    POLYGON_OPEN,
    POLYGON_CLOSE,
    SPRINGS,
    SPRING,
    ROPE,
    ENGINE,
    PLAN,
    PARTICLE
};


/***************************** Color *****************************/
class Color {
public:
    unsigned char r, g, b, a;
    
    static Color * MakeColor(real _r, real _g, real _b, real _a);
    static Color * MakeRandonColor();
};


/***************************** Vector3 *****************************/
class Vector3 {
private:
    real pad;
public:
    real x, y, z;
    
    Vector3();
    Vector3(const real _x, const real _y, const real _z);
    ~Vector3();
    
    void clear();
    void operator=(const Vector3 * _vector);
    void operator+=(const Vector3 * _vector);
    Vector3 * operator+(const Vector3 * _vector) const;
    void operator-=(const Vector3 * _vector);
    Vector3 * operator-(const Vector3 * _vector) const;
    void addScaledVector(const Vector3 * _vector, real _scale);
    Vector3 * componentProduct(const Vector3 * _vector) const;
    void componentProductUpdate(const Vector3 * _vector);
    real scalarProduct(const Vector3 * _vector) const;
    real operator *(const Vector3 * _vector) const;
    void operator *=(const real _value);
    Vector3 * operator*(const real _value) const;
    Vector3 * vectorProduct(const Vector3 * _vector) const;
    void operator%=(const Vector3 *_vector);
    Vector3 * operator%(const Vector3 * _vector) const;
    
    static Vector3 * MakeVector3(const Vector3 * _vector);
    static Vector3 * MakeVector3(real _x, real _y);
    static Vector3 * MakeVector3(real _x, real _y, real _z);
    
    void invert();
    real magnitude() const;
    real squareMagnitude() const;
    void normalize();
};


/***************************** BBox *****************************/
static const real colorBBox[16] = {
    255, 255, 255, 1,
    255, 255, 255, 1,
    255, 255, 255, 1,
    255, 255, 255, 1,
};

class BBox {
public:
    Vector3 * min;
    Vector3 * max;
    real * ptr;
    const real * color;
    
    BBox();
    ~BBox();
};


/***************************** Quaternion *****************************/
class Quaternion {
public:
    union {
        struct {
            real r, i, j, k;
        };
        real data[4];
    };
    
    Quaternion(real _r, real _i, real _j, real _k);
    
    void normalize();
    void operator *=(const Quaternion * _quaternion);
    void rotateByVector(const Vector3 * _vector);
    void addScaledVector(const Vector3 * _vector, real _scale);
};


/***************************** Matrix3 *****************************/
class Matrix3 {
public:
    real data[9];
    
    Matrix3();
    Matrix3(real _c0, real _c1, real _c2, real _c3, real _c4, real _c5, real _c6, real _c7, real _c8);
    Vector3 * operator*(const Vector3 * _vector) const;
    Matrix3 * operator*(const Matrix3 * _matrix) const;
    void setInverse(const Matrix3 * _matrix);
    Matrix3 * inverse() const;
    void invert();
    Vector3 * transform(const Vector3 * _vector) const;
    void setTranspose(const Matrix3 * _matrix);
    Matrix3 * transpose() const;
    void setOrientation(const Quaternion * _quaternion);
};


/***************************** Matrix4 *****************************/
class Matrix4 {
private:
public:
    real data[12];

    Matrix4 * operator*(const Matrix4 * _matrix) const;
    Vector3 * operator*(const Vector3 * _vector) const;
    real getDeterminant() const;
    void setInverse(const Matrix4 * _matrix);
    Matrix4 * inverse() const;
    void invert();
    Vector3 * transform(const Vector3 * _vector) const;
    Vector3 * transformInverse(const Vector3 * _vector) const;
    Vector3 * localToWorld(const Vector3 * _local, const Matrix4 * _transform);
    Vector3 * worldToLocal(const Vector3 * _world, const Matrix4 * _transform);
    Vector3 * transformDirection(const Vector3 * _vector) const;
    Vector3 * transformInverseDirection(const Vector3 * _vector) const;
    Vector3 * localToWorldDirn(const Vector3 * _local, const Matrix4 * _transform);
    Vector3 * worldToLocalDirn(const Vector3 * _world, const Matrix4 * _transform);
    void setOrientationAndPos(const Quaternion * _quaternion, const Vector3 * _pos);
};


/***************************** Matrix Transform *****************************/
static inline real * MatrixMakeIdentity()
{
    real * matrix = new real[16];
    for (int i = 0; i < 16; i++) {
		matrix[i] = 0.0f;
	}
	
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
    
    return matrix;
}

//static real * MatrixIdentity = MatrixMakeIdentity();

static inline void MatrixTransformIdentity(real ** matrix)
{
    
    for (int i = 0; i < 16; i++) {
		*(*matrix+i) = 0.0f;
	}
	
	*(*matrix+0) = *(*matrix+5) = *(*matrix+10) = *(*matrix+15) = 1.0;
}

static inline real * MatrixMultiply(real * _matrixLeft, real * _matrixRight)
{
	real * matrixResult = new real[16];
	
    for (int i = 0; i < 16; i++) {
        matrixResult[i] = _matrixLeft[i % 4] * _matrixRight[i / 4 * 4] +
        _matrixLeft[(i % 4) + 4] * _matrixRight[i / 4 * 4 + 1] +
        _matrixLeft[(i % 4) + 8] * _matrixRight[i / 4 * 4 + 2] +
        _matrixLeft[(i % 4) + 12] * _matrixRight[i / 4 * 4 + 3];
    }
    
    
	return matrixResult;
}

static inline void MatrixTranslate(real * matrix, real _x, real _y, real _z)
{
	// translate (tx, ty, tz)  matrix identity
	matrix[12] = _x;
	matrix[13] = _y;
	matrix[14] = _z;
	//matrix[15] = _w;
}

static inline void MatrixTranslate(real * matrix, real _x, real _y)
{
    MatrixTranslate(matrix, _x, _y, 0.0f);
}

static inline void MatrixTranslate(real * matrix, Vector3 * _vector)
{
    MatrixTranslate(matrix, _vector->x, _vector->y, _vector->z);
}

static inline real * MatrixMakeTranslate(real _x, real _y)
{
    real * matrix = MatrixMakeIdentity();
    MatrixTranslate(matrix, _x, _y);
    return matrix;
}

static inline void MatrixScale(real * matrix, real _x, real _y, real _z)
{
	// scale (sx, sy, sz) matrix identidade
	matrix[0]  = _x; //(_x / 100);
	matrix[5]  = _y; //(_y / 100);
	matrix[10] = _z;
}

static inline void MatrixScale(real * matrix, real _x, real _y)
{
    MatrixScale(matrix, _x, _y, 0.0f);
}

static inline real * MatrixMakeScale(real _x, real _y)
{
    real * matrix = MatrixMakeIdentity();
    MatrixScale(matrix, _x, _y, 0.0f);
    return matrix;
}

static inline real * MatrixMakeXRotation(real radians)
{
    real cos = cosf(radians);
    real sin = sinf(radians);
    
    real * matrix = MatrixMakeIdentity();
    matrix[5] = cos;
    matrix[6] = sin;
    matrix[9] = -sin;
    matrix[10] = cos;
    
    return matrix;
}

static inline real * MatrixMakeYRotation(real radians)
{
    real cos = cosf(radians);
    real sin = sinf(radians);
    
    real * matrix = MatrixMakeIdentity();
    matrix[0] = cos;
    matrix[2] = -sin;
    matrix[8] = sin;
    matrix[10] = cos;
    
    return matrix;
}

static inline real * MatrixMakeZRotation(real radians)
{
    real cos = cosf(radians);
    real sin = sinf(radians);
    
    real * matrix = MatrixMakeIdentity();
    matrix[0] = cos;
    matrix[1] = sin;
    matrix[4] = -sin;
    matrix[5] = cos;
    
    return matrix;
}

static inline Vector3 * MatrixTransformPoint(const real * matrix, const Vector3 * _vector)
{
	real x = matrix[0] * _vector->x + matrix[4] * _vector->y + matrix[8]  * _vector->z + matrix[12] * 1;//vector->w;
	real y = matrix[1] * _vector->x + matrix[5] * _vector->y + matrix[9]  * _vector->z + matrix[13] * 1;//vector->w;
	real z = matrix[2] * _vector->x + matrix[6] * _vector->y + matrix[10] * _vector->z + matrix[14] * 1;//vector->w;
	//real w = matrix[3] * vector->x + matrix[7] * vector->y + matrix[11] * vector->z + matrix[15] * vector->w;
    
	return Vector3::MakeVector3(x, y, z);
}

static inline void MatrixOrtho(real * matrix, real left, real right, real bottom, real top, real nearZ, real farZ)
{
    MatrixTransformIdentity(&matrix);
    
    real ral = right + left;
    real rsl = right - left;
    real tab = top + bottom;
    real tsb = top - bottom;
    real fan = farZ + nearZ;
    real fsn = farZ - nearZ;
    
    matrix[0] = 2.0f / rsl;
    matrix[5] = 2.0f / tsb;
    matrix[10] = -2.0f / fsn;
    matrix[12] = -ral / rsl;
    matrix[13] = -tab / tsb;
    matrix[14] = -fan / fsn;
}

static inline real * MatrixMakeOrtho(real left, real right, real bottom, real top, real nearZ, real farZ)
{
    real * matrix = new real(16);
    MatrixOrtho(matrix, left, right, bottom, top, nearZ, farZ);
    return matrix;
}


#endif
