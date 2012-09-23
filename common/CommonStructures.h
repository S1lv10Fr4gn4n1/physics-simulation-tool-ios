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
#include "Defines.h"

/***************************** Enumations *****************************/
enum TypeObject {
    PLAN,
    
    SPHERE,
    BOX,
    TRIANGLE_TRIANGULAR_BASE,
    TRIANGLE_SQUARE_BASE,
    CONE,
    CYLINDER,

    CIRCLE,
    SQUARE,
    TRIANGLE,
    POLYGON_OPEN,
    POLYGON_CLOSE,
    SPRINGS,
    SPRING,
    ROPE,
    ENGINE,
    PARTICLE,
    
    NONE
};

// TODO revise
enum GLtype {
    POINTS         = 0x0000,
    LINES          = 0x0001,
    LINE_LOOP      = 0x0002,
    LINE_STRIP     = 0x0003,
    TRIANGLES      = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN   = 0x0006,
};


/***************************** Color *****************************/
class Color {
public:
    unsigned char r, g, b, a;
    
    static Color * MakeRandonColor();
};


/***************************** Vector3 *****************************/
class Vector3 {
private:
    real pad;
public:
    real x, y, z;
    
    Vector3();
    Vector3(const Vector3 &_vector);
    Vector3(const real _x, const real _y);
    Vector3(const real _x, const real _y, const real _z);
    ~Vector3();
    
    void clear();
    void operator=(const Vector3 &_vector);
    void operator+=(const Vector3 &_vector);
    Vector3 operator+(const Vector3 &_vector) const;
    void operator-=(const Vector3 &_vector);
    Vector3 operator-(const Vector3 &_vector) const;
    void addScaledVector(const Vector3 &_vector, real _scale);
    Vector3 componentProduct(const Vector3 &_vector) const;
    void componentProductUpdate(const Vector3 &_vector);
    real scalarProduct(const Vector3 &_vector) const;
    real operator *(const Vector3 &_vector) const;
    void operator *=(const real _value);
    Vector3 operator*(const real _value) const;
    Vector3 vectorProduct(const Vector3 &_vector) const;
    void operator%=(const Vector3 &_vector);
    Vector3 operator%(const Vector3 &_vector) const;
    real operator[](unsigned _index) const;
    real& operator[](unsigned _index);
    
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
    Vector3 min;
    Vector3 max;
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
    
    Quaternion();
    Quaternion(real _r, real _i, real _j, real _k);
    
    void normalize();
    void operator *=(const Quaternion &_quaternion);
    void rotateByVector(const Vector3 &_vector);
    void addScaledVector(const Vector3 &_vector, real _scale);
};


/***************************** Matrix3 *****************************/
class Matrix3 {
public:
    real data[9];
    
    Matrix3();
    Matrix3(real _c0, real _c1, real _c2, real _c3, real _c4, real _c5, real _c6, real _c7, real _c8);
    Vector3 operator*(const Vector3 &_vector) const;
    Matrix3 operator*(const Matrix3 &_matrix) const;
    void operator*=(const Matrix3 &_matrix);
    void operator*=(const real _scalar);
    void operator+=(const Matrix3 &_matrix);
    void setInverse(const Matrix3 &_matrix);
    Matrix3 inverse() const;
    void invert();
    Vector3 transform(const Vector3 &_vector) const;
    void setTranspose(const Matrix3 &_matrix);
    Matrix3 transpose() const;
    void setOrientation(const Quaternion &_quaternion);
    void setComponents(const Vector3 &_compOne, const Vector3 &_compTwo, const Vector3 &_compThree);
    Vector3 transformTranspose(const Vector3 &_vector) const;
    void setSkewSymmetric(const Vector3 &_vector);
    void setBlockInertiaTensor(const Vector3 &_halfSizes, real _mass);
    void setInertiaTensorCoeffs(real _ix, real _iy, real _iz, real _ixy=0, real _ixz=0, real _iyz=0);
};


/***************************** Matrix4 *****************************/
class Matrix4 {
private:
public:
    real data[12];
    
    Matrix4();
    Matrix4(real _c0, real _c1, real _c2, real _c3, real _c4, real _c5,
            real _c6, real _c7, real _c8, real _c9, real _c10, real _c11);
    Matrix4 operator*(const Matrix4 &_matrix) const;
    Vector3 operator*(const Vector3 &_vector) const;
    real getDeterminant() const;
    void setInverse(const Matrix4 &_matrix);
    Matrix4 inverse() const;
    void invert();
    Vector3 transform(const Vector3 &_vector) const;
    Vector3 transformInverse(const Vector3 &_vector) const;
    Vector3 localToWorld(const Vector3 &_local, const Matrix4 &_transform);
    Vector3 worldToLocal(const Vector3 &_world, const Matrix4 &_transform);
    Vector3 transformDirection(const Vector3 &_vector) const;
    Vector3 transformInverseDirection(const Vector3 &_vector) const;
    Vector3 localToWorldDirn(const Vector3 &_local, const Matrix4 &_transform);
    Vector3 worldToLocalDirn(const Vector3 &_world, const Matrix4 &_transform);
    void setOrientationAndPos(const Quaternion &_quaternion, const Vector3 &_pos);
    Vector3 getAxisVector(int _i) const;
};


/***************************** Matrix 4x4 *****************************/

class Matrix4x4 {
public:
    real data[16];
    Matrix4x4();
    Matrix4x4(real _matrix[16]);
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

const static real * MatrixIdentity = MatrixMakeIdentity();

static inline void MatrixTransformIdentity(real ** matrix)
{
    
    for (int i = 0; i < 16; i++) {
		*(*matrix+i) = 0.0f;
	}
	
	*(*matrix+0) = *(*matrix+5) = *(*matrix+10) = *(*matrix+15) = 1.0f;
}

static inline void MatrixTransformIdentity(real * matrix)
{
    
    for (int i = 0; i < 16; i++) {
		*(matrix+i) = 0.0f;
	}
	
	*(matrix+0) = *(matrix+5) = *(matrix+10) = *(matrix+15) = 1.0f;
}

static inline real * MatrixMultiplyRigidBody(real * _matrixLeft, real * _matrixRight)
{
	real * matrixResult = new real[12];
	
    for (int i = 0; i < 12; i++) {
        matrixResult[i] = _matrixLeft[i % 4] *        _matrixRight[i / 4 * 4] +
                          _matrixLeft[(i % 4) + 4]  * _matrixRight[i / 4 * 4 + 1] +
                          _matrixLeft[(i % 4) + 8]  * _matrixRight[i / 4 * 4 + 2] +
                          _matrixLeft[(i % 4) + 12] * _matrixRight[i / 4 * 4 + 3];
    }
    
    
	return matrixResult;
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

static inline void MatrixTranslateRigidBody(real * matrix, real _x, real _y, real _z)
{
	// translate (tx, ty, tz)  matrix rigidbody
	matrix[3] = _x;
	matrix[7] = _y;
	matrix[11] = _z;
	//matrix[15] = _w;
}

static inline void MatrixTranslateRigidBody(real * matrix, Vector3 &_vector)
{
    MatrixTranslateRigidBody(matrix, _vector.x, _vector.y, _vector.z);
}


//static inline void MatrixTranslate(real * matrix, real _x, real _y)
//{
//    MatrixTranslate(matrix, _x, _y, 0.0f);
//}

static inline void MatrixTranslate(real * matrix, const Vector3 &_vector)
{
    MatrixTranslate(matrix, _vector.x, _vector.y, _vector.z);
}

//static inline real * MatrixMakeTranslate(real _x, real _y)
//{
//    real * matrix = MatrixMakeIdentity();
//    MatrixTranslate(matrix, _x, _y);
//    return matrix;
//}

static inline void MatrixScale(real * matrix, real _x, real _y, real _z)
{
	// scale (sx, sy, sz) matrix identidade
	matrix[0]  = _x;
	matrix[5]  = _y;
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

//static inline real * MatrixMakeScale(real _x, real _y, real _z)
//{
//    real * matrix = MatrixMakeIdentity();
//    MatrixScale(matrix, _x, _y, _z);
//    return matrix;
//}

//static inline real * MatrixMakeXRotation(real radians)
//{
//    real cos = cosf(radians);
//    real sin = sinf(radians);
//    
//    real * matrix = MatrixMakeIdentity();
//    matrix[5] = cos;
//    matrix[6] = sin;
//    matrix[9] = -sin;
//    matrix[10] = cos;
//    
//    return matrix;
//}

//static inline real * MatrixMakeYRotation(real radians)
//{
//    real cos = cosf(radians);
//    real sin = sinf(radians);
//    
//    real * matrix = MatrixMakeIdentity();
//    matrix[0] = cos;
//    matrix[2] = -sin;
//    matrix[8] = sin;
//    matrix[10] = cos;
//    
//    return matrix;
//}

static inline real * MatrixMakeZRotation(real _radians)
{
    real cos = cosf(_radians);
    real sin = sinf(_radians);
    
    real * matrix = MatrixMakeIdentity();
    matrix[0] = cos;
    matrix[1] = sin;
    matrix[4] = -sin;
    matrix[5] = cos;
    
    return matrix;
}


static inline real * MatrixMakeZRotationRigidBody(real _radians)
{
    real cos = cosf(_radians);
    real sin = sinf(_radians);
    
    real * matrix = MatrixMakeIdentity();
    matrix[0] = cos;
    matrix[4] = sin;
    matrix[1] = -sin;
    matrix[5] = cos;
    
    return matrix;
}


static inline Vector3 MatrixTransformPoint(const real * matrix, const Vector3 &_vector)
{
	real x = matrix[0] * _vector.x + matrix[4] * _vector.y + matrix[8]  * _vector.z + matrix[12] * 1;//vector->w;
	real y = matrix[1] * _vector.x + matrix[5] * _vector.y + matrix[9]  * _vector.z + matrix[13] * 1;//vector->w;
	real z = matrix[2] * _vector.x + matrix[6] * _vector.y + matrix[10] * _vector.z + matrix[14] * 1;//vector->w;
	//real w = matrix[3] * vector->x + matrix[7] * vector->y + matrix[11] * vector->z + matrix[15] * vector->w;
    
	return Vector3(x, y, z);
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

static inline void MatrixMakePerspective(Matrix4x4 &_matrix, real _fovyRadians, real _aspect, real _nearZ, real _farZ)
{
    real cotan = 1.0f / real_tan(_fovyRadians / 2.0f);
    
    _matrix.data[0] = cotan / _aspect;
    _matrix.data[1] = 0.0f;
    _matrix.data[2] = 0.0f;
    _matrix.data[3] = 0.0f;
    _matrix.data[4] = 0.0f;
    _matrix.data[5] = cotan;
    _matrix.data[6] = 0.0f;
    _matrix.data[7] = 0.0f;
    _matrix.data[8] = 0.0f;
    _matrix.data[9] = 0.0f;
    _matrix.data[10] = (_farZ + _nearZ) / (_nearZ - _farZ);
    _matrix.data[11] = -1.0f;
    _matrix.data[12] = 0.0f;
    _matrix.data[13] = 0.0f;
    _matrix.data[14] = (2.0f * _farZ * _nearZ) / (_nearZ - _farZ);
    _matrix.data[15] = 0.0f;
}

static inline Matrix4x4 MatrixMakePerspective(real _fovyRadians, real _aspect, real _nearZ, real _farZ)
{
    Matrix4x4 matrix;
    MatrixMakePerspective(matrix, _fovyRadians, _aspect, _nearZ, _farZ);
    return matrix;
}

static inline void MatrixMakeLookAt(Matrix4x4 &_matrix, float _eyeX, float _eyeY, float _eyeZ,
                                    float _centerX, float _centerY, float _centerZ,
                                    float _upX, float _upY, float _upZ)
{
    Vector3 ev( _eyeX, _eyeY, _eyeZ);
    Vector3 cv(_centerX, _centerY, _centerZ);
    Vector3 uv(_upX, _upY, _upZ);
    
    cv.invert();
    Vector3 n = ev + cv;
    n.normalize();
    
    Vector3 u =  uv.vectorProduct(n);
    u.normalize();
    Vector3 v = n.vectorProduct(u);
    
    _matrix.data[0] = u.x;
    _matrix.data[1] = v.x;
    _matrix.data[2] = n.x;
    _matrix.data[3] = 0.0f;
    _matrix.data[4] = u.y;
    _matrix.data[5] = v.y;
    _matrix.data[6] = n.y;
    _matrix.data[7] = 0.0f;
    _matrix.data[8] = u.z;
    _matrix.data[9] = v.z;
    _matrix.data[10] = n.z;
    
    u.invert();
    v.invert();
    n.invert();
    real a30 = u.scalarProduct(ev);
    real a31 = v.scalarProduct(ev);
    real a32 = n.scalarProduct(ev);
    
    _matrix.data[11] = 0.0f;
    _matrix.data[12] = a30;
    _matrix.data[13] = a31;
    _matrix.data[14] = a32;
    _matrix.data[15] = 1.0f;
}

static inline Matrix4x4 MatrixMakeLookAt(float _eyeX, float _eyeY, float _eyeZ,
                                         float _centerX, float _centerY, float _centerZ,
                                         float _upX, float _upY, float _upZ)
{
    Matrix4x4 matrix;
    MatrixMakeLookAt(matrix, _eyeX, _eyeY, _eyeZ, _centerX, _centerY, _centerZ, _upX, _upY, _upZ);
    return matrix;
}

static inline void calculateTransformMatrix(Matrix4 &_transformMatrix, const Vector3 &_position,
                                            const Quaternion &_orientation)
{
    _transformMatrix.data[0] = 1 - 2*_orientation.j*_orientation.j - 2*_orientation.k*_orientation.k;
    _transformMatrix.data[1] = 2*_orientation.i*_orientation.j - 2*_orientation.r*_orientation.k;
    _transformMatrix.data[2] = 2*_orientation.i*_orientation.k + 2*_orientation.r*_orientation.j;
    _transformMatrix.data[3] = _position.x;
    _transformMatrix.data[4] = 2*_orientation.i*_orientation.j + 2*_orientation.r*_orientation.k;
    _transformMatrix.data[5] = 1 - 2*_orientation.i*_orientation.i - 2*_orientation.k*_orientation.k;
    _transformMatrix.data[6] = 2*_orientation.j*_orientation.k - 2*_orientation.r*_orientation.i;
    _transformMatrix.data[7] = _position.y;
    _transformMatrix.data[8] = 2*_orientation.i*_orientation.k - 2*_orientation.r*_orientation.j;
    _transformMatrix.data[9] = 2*_orientation.j*_orientation.k + 2*_orientation.r*_orientation.i;
    _transformMatrix.data[10] = 1 - 2*_orientation.i*_orientation.i-2*_orientation.j*_orientation.j;
    _transformMatrix.data[11] = _position.z;
}

static inline void transformInertiaTensor(Matrix3 &_iitWorld, const Quaternion &_quaternion,
                                          const Matrix3 &_iitBody, const Matrix4 &_rotmat)
{
    real t4 = _rotmat.data[0]*_iitBody.data[0]+
              _rotmat.data[1]*_iitBody.data[3]+
              _rotmat.data[2]*_iitBody.data[6];
    real t9 = _rotmat.data[0]*_iitBody.data[1]+
              _rotmat.data[1]*_iitBody.data[4]+
              _rotmat.data[2]*_iitBody.data[7];
    real t14 = _rotmat.data[0]*_iitBody.data[2]+
               _rotmat.data[1]*_iitBody.data[5]+
               _rotmat.data[2]*_iitBody.data[8];
    real t28 = _rotmat.data[4]*_iitBody.data[0]+
               _rotmat.data[5]*_iitBody.data[3]+
               _rotmat.data[6]*_iitBody.data[6];
    real t33 = _rotmat.data[4]*_iitBody.data[1]+
               _rotmat.data[5]*_iitBody.data[4]+
               _rotmat.data[6]*_iitBody.data[7];
    real t38 = _rotmat.data[4]*_iitBody.data[2]+
               _rotmat.data[5]*_iitBody.data[5]+
               _rotmat.data[6]*_iitBody.data[8];
    real t52 = _rotmat.data[8]*_iitBody.data[0]+
               _rotmat.data[9]*_iitBody.data[3]+
               _rotmat.data[10]*_iitBody.data[6];
    real t57 = _rotmat.data[8]*_iitBody.data[1]+
               _rotmat.data[9]*_iitBody.data[4]+
               _rotmat.data[10]*_iitBody.data[7];
    real t62 = _rotmat.data[8]*_iitBody.data[2]+
               _rotmat.data[9]*_iitBody.data[5]+
               _rotmat.data[10]*_iitBody.data[8];
    _iitWorld.data[0] = t4*_rotmat.data[0]+
                         t9*_rotmat.data[1]+
                         t14*_rotmat.data[2];
    _iitWorld.data[1] = t4*_rotmat.data[4]+
                         t9*_rotmat.data[5]+
                         t14*_rotmat.data[6];
    _iitWorld.data[2] = t4*_rotmat.data[8]+
                         t9*_rotmat.data[9]+
                         t14*_rotmat.data[10];
    _iitWorld.data[3] = t28*_rotmat.data[0]+
                         t33*_rotmat.data[1]+
                         t38*_rotmat.data[2];
    _iitWorld.data[4] = t28*_rotmat.data[4]+
                         t33*_rotmat.data[5]+
                         t38*_rotmat.data[6];
    _iitWorld.data[5] = t28*_rotmat.data[8]+
                         t33*_rotmat.data[9]+
                         t38*_rotmat.data[10];
    _iitWorld.data[6] = t52*_rotmat.data[0]+
                         t57*_rotmat.data[1]+
                         t62*_rotmat.data[2];
    _iitWorld.data[7] = t52*_rotmat.data[4]+
                         t57*_rotmat.data[5]+
                         t62*_rotmat.data[6];
    _iitWorld.data[8] = t52*_rotmat.data[8]+
                         t57*_rotmat.data[9]+
                         t62*_rotmat.data[10];
}

//static void MakeArrayVector3(Vector3 _array[], unsigned _size)
//{
//    for (int i=0; i<_size; i++) {
//        _array[i] = new Vector3();
//    }
//}
//
//static void DeleteArrayVector3(Vector3 _array[], unsigned _size)
//{
//    for (int i=0; i<_size; i++) {
//        delete _array[i];
//        _array[i] = NULL;
//    }
//    
//    _array = NULL;
//}
//
//static inline void MakeArrayMatrix3(Matrix3 * _array[], int _size)
//{
//    for (int i=0; i<_size; i++) {
//        _array[i] = new Matrix3();
//    }
//}
//
//static inline void DeleteArrayMatrix3(Matrix3 * _array[], int _size)
//{
//    for (int i=0; i<_size; i++) {
//        if (_array[i]) {
//            delete _array[i];
//            _array[i] = NULL;
//        }
//    }
//    
//    _array = NULL;
//}
//
//static inline void MakeArrayMatrix4(Matrix4 * _array[], int _size)
//{
//    for (int i=0; i<_size; i++) {
//        _array[i] = new Matrix4();
//    }
//}
//
//static inline void DeleteArrayMatrix4(Matrix4 * _array[], int _size)
//{
//    for (int i=0; i<_size; i++) {
//        delete _array[i];
//        _array[i] = NULL;
//    }
//    
//    _array = NULL;
//}

#endif
