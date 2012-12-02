//
//  CommonStructures.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#include "CommonStructures.h"

/***************************** Vector3 *****************************/
Vector3::Vector3()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->pad = 0.0f;
}

Vector3::Vector3(const Vector3 &_vector)
{
    this->x = _vector.x;
    this->y = _vector.y;
    this->z = _vector.z;
    this->pad = 0.0f;
}

Vector3::Vector3(const real _x, const real _y, const real _z)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
    this->pad = 0.0f;
}

Vector3::Vector3(const real _x, const real _y)
{
    this->x = _x;
    this->y = _y;
    this->z = 0.0f;
    this->pad = 0.0f;
}

Vector3::~Vector3()
{
    // TODO put your code here
}

void Vector3::clear()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->pad = 0.0f;
}

real Vector3::operator[](unsigned _index) const
{
    if (_index == 0) {
        return this->x;
    }
    if (_index == 1) {
        return this->y;
    }
    return this->z;
}

real& Vector3::operator[](unsigned _index)
{
    if (_index == 0) {
        return this->x;
    } else if (_index == 1) {
        return this->y;
    }
    return this->z;
}

void Vector3::operator+=(const Vector3 &_vector)
{
    this->x += _vector.x;
    this->y += _vector.y;
    this->z += _vector.z;
}

void Vector3::operator=(const Vector3 &_vector)
{
    this->x = _vector.x;
    this->y = _vector.y;
    this->z = _vector.z;
}

Vector3 Vector3::operator+(const Vector3 &_vector) const
{
    return Vector3(this->x+_vector.x, this->y+_vector.y, this->z+_vector.z);
}

void Vector3::operator-=(const Vector3 &_vector)
{
    this->x -= _vector.x;
    this->y -= _vector.y;
    this->z -= _vector.z;
}

Vector3 Vector3::operator-(const Vector3 &_vector) const
{
    return Vector3(this->x-_vector.x, this->y-_vector.y, this->z-_vector.z);
}

void Vector3::addScaledVector(const Vector3 &_vector, real _scale)
{
    this->x += _vector.x * _scale;
    this->y += _vector.y * _scale;
    this->z += _vector.z * _scale;
}

Vector3 Vector3::componentProduct(const Vector3 &_vector) const
{
    return Vector3(this->x * _vector.x, this->y * _vector.y, this->z * _vector.z);
}
void Vector3::componentProductUpdate(const Vector3 &_vector)
{
    this->x *= _vector.x;
    this->y *= _vector.y;
    this->z *= _vector.z;
}

real Vector3::scalarProduct(const Vector3 &_vector) const
{
    return this->x*_vector.x + this->y*_vector.y + this->z*_vector.z;
}

real Vector3::operator *(const Vector3 &_vector) const
{
    return this->x*_vector.x + this->y*_vector.y + this->z*_vector.z;
}

void Vector3::operator*=(const real _value)
{
    this->x *= _value;
    this->y *= _value;
    this->z *= _value;
}

Vector3 Vector3::operator*(const real _value) const
{
    return Vector3(this->x*_value, this->y*_value, this->z*_value);
}

Vector3 Vector3::vectorProduct(const Vector3 &_vector) const
{
    return Vector3(this->y*_vector.z - this->z*_vector.y,
                   this->z*_vector.x - this->x*_vector.z,
                   this->x*_vector.y - this->y*_vector.x);
}

void Vector3::operator%=(const Vector3 &_vector)
{
    *this = this->vectorProduct(_vector);
}

Vector3 Vector3::operator%(const Vector3 &_vector) const
{
    return Vector3(this->y*_vector.z - this->z*_vector.y,
                   this->z*_vector.x - this->x*_vector.z,
                   this->x*_vector.y - this->y*_vector.x);
}

void Vector3::invert()
{
    this->x = -this->x;
    this->y = -this->y;
    this->z = -this->z;
}

real Vector3::magnitude() const
{
    return real_sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

real Vector3::squareMagnitude() const
{
    return this->x*this->x + this->y*this->y + this->z*this->z;
}

void Vector3::normalize()
{
    real l = this->magnitude();
    if (l > 0.0f) {
        (*this)*=((real)1)/l;
    }
}

/***************************** Color *****************************/
Color * Color::MakeRandonColor()
{
    Color * color = new Color();
    color->r = static_cast<unsigned char>(rand() % 256);
    color->g = static_cast<unsigned char>(rand() % 256);
    color->b = static_cast<unsigned char>(rand() % 256);
    color->a = 1;
    
    return color;
}


/***************************** BBox *****************************/
BBox::BBox()
{
    this->ptr = NULL;
    this->color = colorBBox;
}

BBox::~BBox()
{
    delete [] this->ptr;
    this->ptr = NULL;
}


/***************************** Matrix3 *****************************/
Matrix3::Matrix3()
{
    this->data[0] = this->data[1] = this->data[2] = this->data[3] = this->data[4] =
    this->data[5] = this->data[6] = this->data[7] = this->data[8] = 0;
}

Matrix3::Matrix3(real _c0, real _c1, real _c2, real _c3, real _c4, real _c5, real _c6, real _c7, real _c8)
{
    this->data[0] = _c0;
    this->data[1] = _c1;
    this->data[2] = _c2;
    this->data[3] = _c3;
    this->data[4] = _c4;
    this->data[5] = _c5;
    this->data[6] = _c6;
    this->data[7] = _c7;
    this->data[8] = _c8;
}

Vector3 Matrix3::operator*(const Vector3 &_vector) const
{
    return Vector3(_vector.x * this->data[0] + _vector.y * this->data[1] + _vector.z * this->data[2],
                   _vector.x * this->data[3] + _vector.y * this->data[4] + _vector.z * this->data[5],
                   _vector.x * this->data[6] + _vector.y * this->data[7] + _vector.z * this->data[8]
                   );
}

Matrix3 Matrix3::operator*(const Matrix3 &_matrix) const
{
    return Matrix3(this->data[0]*_matrix.data[0] + this->data[1]*_matrix.data[3] + this->data[2]*_matrix.data[6],
                   this->data[0]*_matrix.data[1] + this->data[1]*_matrix.data[4] + this->data[2]*_matrix.data[7],
                   this->data[0]*_matrix.data[2] + this->data[1]*_matrix.data[5] + this->data[2]*_matrix.data[8],
                   this->data[3]*_matrix.data[0] + this->data[4]*_matrix.data[3] + this->data[5]*_matrix.data[6],
                   this->data[3]*_matrix.data[1] + this->data[4]*_matrix.data[4] + this->data[5]*_matrix.data[7],
                   this->data[3]*_matrix.data[2] + this->data[4]*_matrix.data[5] + this->data[5]*_matrix.data[8],
                   this->data[6]*_matrix.data[0] + this->data[7]*_matrix.data[3] + this->data[8]*_matrix.data[6],
                   this->data[6]*_matrix.data[1] + this->data[7]*_matrix.data[4] + this->data[8]*_matrix.data[7],
                   this->data[6]*_matrix.data[2] + this->data[7]*_matrix.data[5] + this->data[8]*_matrix.data[8]
                   );
}

void Matrix3::operator*=(const Matrix3 &_matrix)
{
    real t1;
    real t2;
    real t3;
    
    t1 = this->data[0]*_matrix.data[0] + this->data[1]*_matrix.data[3] + this->data[2]*_matrix.data[6];
    t2 = this->data[0]*_matrix.data[1] + this->data[1]*_matrix.data[4] + this->data[2]*_matrix.data[7];
    t3 = this->data[0]*_matrix.data[2] + this->data[1]*_matrix.data[5] + this->data[2]*_matrix.data[8];
    this->data[0] = t1;
    this->data[1] = t2;
    this->data[2] = t3;
    
    t1 = this->data[3]*_matrix.data[0] + this->data[4]*_matrix.data[3] + this->data[5]*_matrix.data[6];
    t2 = this->data[3]*_matrix.data[1] + this->data[4]*_matrix.data[4] + this->data[5]*_matrix.data[7];
    t3 = this->data[3]*_matrix.data[2] + this->data[4]*_matrix.data[5] + this->data[5]*_matrix.data[8];
    data[3] = t1;
    data[4] = t2;
    data[5] = t3;
    
    t1 = this->data[6]*_matrix.data[0] + this->data[7]*_matrix.data[3] + this->data[8]*_matrix.data[6];
    t2 = this->data[6]*_matrix.data[1] + this->data[7]*_matrix.data[4] + this->data[8]*_matrix.data[7];
    t3 = this->data[6]*_matrix.data[2] + this->data[7]*_matrix.data[5] + this->data[8]*_matrix.data[8];
    this->data[6] = t1;
    this->data[7] = t2;
    this->data[8] = t3;
}

void Matrix3::operator+=(const Matrix3 &_matrix)
{
    this->data[0] += _matrix.data[0];
    this->data[1] += _matrix.data[1];
    this->data[2] += _matrix.data[2];
    this->data[3] += _matrix.data[3];
    this->data[4] += _matrix.data[4];
    this->data[5] += _matrix.data[5];
    this->data[6] += _matrix.data[6];
    this->data[7] += _matrix.data[7];
    this->data[8] += _matrix.data[8];
}

void Matrix3::operator*=(const real _scalar)
{
    this->data[0] *= _scalar;
    this->data[1] *= _scalar;
    this->data[2] *= _scalar;
    this->data[3] *= _scalar;
    this->data[4] *= _scalar;
    this->data[5] *= _scalar;
    this->data[6] *= _scalar;
    this->data[7] *= _scalar;
    this->data[8] *= _scalar;
}

void Matrix3::setInverse(const Matrix3 &_matrix)
{
    real t4 = _matrix.data[0]*_matrix.data[4];
    real t6 = _matrix.data[0]*_matrix.data[5];
    real t8 = _matrix.data[1]*_matrix.data[3];
    real t10 = _matrix.data[2]*_matrix.data[3];
    real t12 = _matrix.data[1]*_matrix.data[6];
    real t14 = _matrix.data[2]*_matrix.data[6];
    
    // calculate the determinant.
    real t16 = (t4*_matrix.data[8] - t6*_matrix.data[7] - t8*_matrix.data[8] +
                t10*_matrix.data[7] + t12*_matrix.data[5] - t14*_matrix.data[4]);
    
    // make sure the determinant is non-zer_matrix
    if (t16 == (real)0.0f) {
        return;
    }
    
    real t17 = 1/t16;
    this->data[0] = (_matrix.data[4]*_matrix.data[8]-_matrix.data[5]*_matrix.data[7])*t17;
    this->data[1] = -(_matrix.data[1]*_matrix.data[8]-_matrix.data[2]*_matrix.data[7])*t17;
    this->data[2] = (_matrix.data[1]*_matrix.data[5]-_matrix.data[2]*_matrix.data[4])*t17;
    this->data[3] = -(_matrix.data[3]*_matrix.data[8]-_matrix.data[5]*_matrix.data[6])*t17;
    this->data[4] = (_matrix.data[0]*_matrix.data[8]-t14)*t17;
    this->data[5] = -(t6-t10)*t17;
    this->data[6] = (_matrix.data[3]*_matrix.data[7]-_matrix.data[4]*_matrix.data[6])*t17;
    this->data[7] = -(_matrix.data[0]*_matrix.data[7]-t12)*t17;
    this->data[8] = (t4-t8)*t17;
}

Matrix3 Matrix3::inverse() const
{
    Matrix3 matrix;
    matrix.setInverse(*this);
    
    return matrix;
}

void Matrix3::invert()
{
    this->setInverse(*this);
}

void Matrix3::setTranspose(const Matrix3 &_matrix)
{
    this->data[0] = _matrix.data[0];
    this->data[1] = _matrix.data[3];
    this->data[2] = _matrix.data[6];
    this->data[3] = _matrix.data[1];
    this->data[4] = _matrix.data[4];
    this->data[5] = _matrix.data[7];
    this->data[6] = _matrix.data[2];
    this->data[7] = _matrix.data[5];
    this->data[8] = _matrix.data[8];
}

Matrix3 Matrix3::transpose() const
{
    Matrix3 matrix;
    matrix.setTranspose(*this);
    
    return matrix;
}

void Matrix3::setOrientation(const Quaternion &_quaternion)
{
    this->data[0] = 1 - (2*_quaternion.j*_quaternion.j + 2*_quaternion.k*_quaternion.k);
    this->data[1] = 2*_quaternion.i*_quaternion.j + 2*_quaternion.k*_quaternion.r;
    this->data[2] = 2*_quaternion.i*_quaternion.k - 2*_quaternion.j*_quaternion.r;
    this->data[3] = 2*_quaternion.i*_quaternion.j - 2*_quaternion.k*_quaternion.r;
    this->data[4] = 1 - (2*_quaternion.i*_quaternion.i  + 2*_quaternion.k*_quaternion.k);
    this->data[5] = 2*_quaternion.j*_quaternion.k + 2*_quaternion.i*_quaternion.r;
    this->data[6] = 2*_quaternion.i*_quaternion.k + 2*_quaternion.j*_quaternion.r;
    this->data[7] = 2*_quaternion.j*_quaternion.k - 2*_quaternion.i*_quaternion.r;
    this->data[8] = 1 - (2*_quaternion.i*_quaternion.i  + 2*_quaternion.j*_quaternion.j);
}

Vector3 Matrix3::transform(const Vector3 &_vector) const
{
    return (*this) * _vector;
}

void Matrix3::setComponents(const Vector3 &_compOne, const Vector3 &_compTwo, const Vector3 &_compThree)
{
    this->data[0] = _compOne.x;
    this->data[1] = _compTwo.x;
    this->data[2] = _compThree.x;
    this->data[3] = _compOne.y;
    this->data[4] = _compTwo.y;
    this->data[5] = _compThree.y;
    this->data[6] = _compOne.z;
    this->data[7] = _compTwo.z;
    this->data[8] = _compThree.z;
};

Vector3 Matrix3::transformTranspose(const Vector3 & _vector) const
{
    return Vector3(_vector.x * this->data[0] + _vector.y * this->data[3] + _vector.z * this->data[6],
                   _vector.x * this->data[1] + _vector.y * this->data[4] + _vector.z * this->data[7],
                   _vector.x * this->data[2] + _vector.y * this->data[5] + _vector.z * this->data[8]
                   );
}

void Matrix3::setSkewSymmetric(const Vector3 &_vector)
{
    this->data[0] = this->data[4] = this->data[8] = 0;
    this->data[1] = -_vector.z;
    this->data[2] = _vector.y;
    this->data[3] = _vector.z;
    this->data[5] = -_vector.x;
    this->data[6] = -_vector.y;
    this->data[7] = _vector.x;
}



// Sets the value of the matrix as an inertia tensor of
// a rectangular block aligned with the body's coordinate
// system with the given axis half-sizes and mass.
void Matrix3::setBlockInertiaTensor(const Vector3 &_halfSizes, real _mass)
{
    Vector3 squares = _halfSizes.componentProduct(_halfSizes);
    this->setInertiaTensorCoeffs(0.3f*_mass*(squares.y + squares.z),
                                 0.3f*_mass*(squares.x + squares.z),
                                 0.3f*_mass*(squares.x + squares.y));
}

// sets the value of the matrix from inertia tensor values
void Matrix3::setInertiaTensorCoeffs(real _ix, real _iy, real _iz, real _ixy, real _ixz, real _iyz)
{
    this->data[0] = _ix;
    this->data[1] = this->data[3] = -_ixy;
    this->data[2] = this->data[6] = -_ixz;
    this->data[4] = _iy;
    this->data[5] = this->data[7] = -_iyz;
    this->data[8] = _iz;
}

/***************************** Matrix4 *****************************/

Matrix4::Matrix4()
{
    this->data[1] = this->data[2] = this->data[3] =
    this->data[4] = this->data[6] = this->data[7] =
    this->data[8] = this->data[9] = this->data[11] = 0;
    this->data[0] = this->data[5] = this->data[10] = 1;
}

Matrix4::Matrix4(real _c0, real _c1, real _c2, real _c3, real _c4, real _c5,
                 real _c6, real _c7, real _c8, real _c9, real _c10, real _c11)
{
    this->data[0] = _c0;
    this->data[1] = _c1;
    this->data[2] = _c2;
    this->data[3] = _c3;
    this->data[4] = _c4;
    this->data[5] = _c5;
    this->data[6] = _c6;
    this->data[7] = _c7;
    this->data[8] = _c8;
    this->data[9] = _c9;
    this->data[10] = _c10;
    this->data[11] = _c11;
}

real Matrix4::getDeterminant() const
{
    return this->data[8]*this->data[5]*this->data[2]+
           this->data[4]*this->data[9]*this->data[2]+
           this->data[8]*this->data[1]*this->data[6]-
           this->data[0]*this->data[9]*this->data[6]-
           this->data[4]*this->data[1]*this->data[10]+
           this->data[0]*this->data[5]*this->data[10];
}


void Matrix4::setOrientationAndPos(const Quaternion &_quaternion, const Vector3 &_pos)
{
    this->data[0] = 1 - (2*_quaternion.j*_quaternion.j + 2*_quaternion.k*_quaternion.k);
    this->data[1] = 2*_quaternion.i*_quaternion.j + 2*_quaternion.k*_quaternion.r;
    this->data[2] = 2*_quaternion.i*_quaternion.k - 2*_quaternion.j*_quaternion.r;
    this->data[3] = _pos.x;
    this->data[4] = 2*_quaternion.i*_quaternion.j - 2*_quaternion.k*_quaternion.r;
    this->data[5] = 1 - (2*_quaternion.i*_quaternion.i  + 2*_quaternion.k*_quaternion.k);
    this->data[6] = 2*_quaternion.j*_quaternion.k + 2*_quaternion.i*_quaternion.r;
    this->data[7] = _pos.y;
    this->data[8] = 2*_quaternion.i*_quaternion.k + 2*_quaternion.j*_quaternion.r;
    this->data[9] = 2*_quaternion.j*_quaternion.k - 2*_quaternion.i*_quaternion.r;
    this->data[10] = 1 - (2*_quaternion.i*_quaternion.i  + 2*_quaternion.j*_quaternion.j);
    this->data[11] = _pos.z;
}

void Matrix4::setInverse(const Matrix4 &_matrix)
{
    // make sure the determinant is non-zero.
    real det = this->getDeterminant();
    
    if (det == 0) {
        return;
    }
    
    det = ((real)1.0)/det;
    
    this->data[0] = (-_matrix.data[9]*_matrix.data[6]+_matrix.data[5]*_matrix.data[10])*det;
    this->data[4] = (_matrix.data[8]*_matrix.data[6]-_matrix.data[4]*_matrix.data[10])*det;
    this->data[8] = (-_matrix.data[8]*_matrix.data[5]+_matrix.data[4]*_matrix.data[9])*det;
    
    this->data[1] = (_matrix.data[9]*_matrix.data[2]-_matrix.data[1]*_matrix.data[10])*det;
    this->data[5] = (-_matrix.data[8]*_matrix.data[2]+_matrix.data[0]*_matrix.data[10])*det;
    this->data[9] = (_matrix.data[8]*_matrix.data[1]-_matrix.data[0]*_matrix.data[9])*det;
    
    this->data[2] = (-_matrix.data[5]*_matrix.data[2]+_matrix.data[1]*_matrix.data[6])*det;
    this->data[6] = (+_matrix.data[4]*_matrix.data[2]-_matrix.data[0]*_matrix.data[6])*det;
    this->data[10] = (-_matrix.data[4]*_matrix.data[1]+_matrix.data[0]*_matrix.data[5])*det;
    
    this->data[3] = (_matrix.data[9]*_matrix.data[6]*_matrix.data[3]
                     -_matrix.data[5]*_matrix.data[10]*_matrix.data[3]
                     -_matrix.data[9]*_matrix.data[2]*_matrix.data[7]
                     +_matrix.data[1]*_matrix.data[10]*_matrix.data[7]
                     +_matrix.data[5]*_matrix.data[2]*_matrix.data[11]
                     -_matrix.data[1]*_matrix.data[6]*_matrix.data[11])*det;
    this->data[7] = (-_matrix.data[8]*_matrix.data[6]*_matrix.data[3]
                     +_matrix.data[4]*_matrix.data[10]*_matrix.data[3]
                     +_matrix.data[8]*_matrix.data[2]*_matrix.data[7]
                     -_matrix.data[0]*_matrix.data[10]*_matrix.data[7]
                     -_matrix.data[4]*_matrix.data[2]*_matrix.data[11]
                     +_matrix.data[0]*_matrix.data[6]*_matrix.data[11])*det;
    this->data[11] =(_matrix.data[8]*_matrix.data[5]*_matrix.data[3]
                     -_matrix.data[4]*_matrix.data[9]*_matrix.data[3]
                     -_matrix.data[8]*_matrix.data[1]*_matrix.data[7]
                     +_matrix.data[0]*_matrix.data[9]*_matrix.data[7]
                     +_matrix.data[4]*_matrix.data[1]*_matrix.data[11]
                     -_matrix.data[0]*_matrix.data[5]*_matrix.data[11])*det;}

Matrix4 Matrix4::inverse() const
{
    Matrix4 matrix;
    matrix.setInverse(*this);
    
    return matrix;
}

void Matrix4::invert()
{
    this->setInverse(*this);
}

Matrix4 Matrix4::operator*(const Matrix4 &_matrix) const
{
    Matrix4 matrix;
    matrix.data[0] = (_matrix.data[0]*this->data[0]) + (_matrix.data[4]*this->data[1]) + (_matrix.data[8]*this->data[2]);
    matrix.data[4] = (_matrix.data[0]*this->data[4]) + (_matrix.data[4]*this->data[5]) + (_matrix.data[8]*this->data[6]);
    matrix.data[8] = (_matrix.data[0]*this->data[8]) + (_matrix.data[4]*this->data[9]) + (_matrix.data[8]*this->data[10]);
    
    matrix.data[1] = (_matrix.data[1]*this->data[0]) + (_matrix.data[5]*this->data[1]) + (_matrix.data[9]*this->data[2]);
    matrix.data[5] = (_matrix.data[1]*this->data[4]) + (_matrix.data[5]*this->data[5]) + (_matrix.data[9]*this->data[6]);
    matrix.data[9] = (_matrix.data[1]*this->data[8]) + (_matrix.data[5]*this->data[9]) + (_matrix.data[9]*this->data[10]);
    
    matrix.data[2] = (_matrix.data[2]*this->data[0]) + (_matrix.data[6]*this->data[1]) + (_matrix.data[10]*this->data[2]);
    matrix.data[6] = (_matrix.data[2]*this->data[4]) + (_matrix.data[6]*this->data[5]) + (_matrix.data[10]*this->data[6]);
    matrix.data[10] = (_matrix.data[2]*this->data[8]) + (_matrix.data[6]*this->data[9]) + (_matrix.data[10]*this->data[10]);
    
    matrix.data[3] = (_matrix.data[3]*this->data[0]) + (_matrix.data[7]*this->data[1]) + (_matrix.data[11]*this->data[2]) + this->data[3];
    matrix.data[7] = (_matrix.data[3]*this->data[4]) + (_matrix.data[7]*this->data[5]) + (_matrix.data[11]*this->data[6]) + this->data[7];
    matrix.data[11] = (_matrix.data[3]*this->data[8]) + (_matrix.data[7]*this->data[9]) + (_matrix.data[11]*this->data[10]) + this->data[11];

    return matrix;
}

Vector3 Matrix4::operator*(const Vector3 &_vector) const
{
    return Vector3(_vector.x * this->data[0] +
                   _vector.y * this->data[1] +
                   _vector.z * this->data[2] + this->data[3],
                   
                   _vector.x * this->data[4] +
                   _vector.y * this->data[5] +
                   _vector.z * this->data[6] + this->data[7],
                       
                   _vector.x * this->data[8] +
                   _vector.y * this->data[9] +
                   _vector.z * this->data[10] + this->data[11]
                   );
}

Vector3 Matrix4::transformInverse(const Vector3 &_vector) const
{
    Vector3 tmp(_vector.x, _vector.y, _vector.z);
    tmp.x -= this->data[3];
    tmp.y -= this->data[7];
    tmp.z -= this->data[11];

    return Vector3(tmp.x * this->data[0] +
                   tmp.y * this->data[4] +
                   tmp.z * this->data[8],
                   tmp.x * this->data[1] +
                   tmp.y * this->data[5] +
                   tmp.z * this->data[9],
                   tmp.x * this->data[2] +
                   tmp.y * this->data[6] +
                   tmp.z * this->data[10]
                   );
}

Vector3 Matrix4::localToWorld(const Vector3 &_local, const Matrix4 &_transform)
{
    return _transform.transform(_local);
}

Vector3 Matrix4::transform(const Vector3 &_vector) const
{
    return (*this) * _vector;
}

Vector3 Matrix4::worldToLocal(const Vector3 &_world, const Matrix4 &_transform)
{
    return _transform.transformInverse(_world);
}

Vector3 Matrix4::transformDirection(const Vector3 &_vector) const
{
    return Vector3(_vector.x * this->data[0] +
                   _vector.y * this->data[1] +
                   _vector.z * this->data[2],
                   _vector.x * this->data[4] +
                   _vector.y * this->data[5] +
                   _vector.z * this->data[6],
                   _vector.x * this->data[8] +
                   _vector.y * this->data[9] +
                   _vector.z * this->data[10]
                   );
}

Vector3 Matrix4::transformInverseDirection(const Vector3 &_vector) const
{
    return Vector3(_vector.x * this->data[0] +
                   _vector.y * this->data[4] +
                   _vector.z * this->data[8],
                   _vector.x * this->data[1] +
                   _vector.y * this->data[5] +
                   _vector.z * this->data[9],
                   _vector.x * this->data[2] +
                   _vector.y * this->data[6] +
                   _vector.z * this->data[10]
                   );
}

Vector3 Matrix4::localToWorldDirn(const Vector3 &_local, const Matrix4 &_transform)
{
    return _transform.transformDirection(_local);
}

Vector3 Matrix4::worldToLocalDirn(const Vector3 &_world, const Matrix4 &_transform)
{
    return _transform.transformInverseDirection(_world);
}

// gets a vector representing one axis, one column, in matrix.
// row 3 corresponds to the position of the transform matrix.
Vector3 Matrix4::getAxisVector(int _i) const
{
    return Vector3(this->data[_i], this->data[_i+4], this->data[_i+8]);
}


/***************************** Matrix4x4 *****************************/
Matrix4x4::Matrix4x4()
{
    this->data[1] = this->data[2] = this->data[3] = this->data[4] =
    this->data[6] = this->data[7] = this->data[8] = this->data[9] =
    this->data[11] = this->data[12] = this->data[13] = this->data[14] = 0;
    
    this->data[0] = this->data[5] = this->data[10] = this->data[15] = 1;
}

Matrix4x4::Matrix4x4(real _matrix[16])
{
    for (int i=0; i<16; i++) {
        this->data[i] = _matrix[i];
    }
}

/***************************** Quaternion *****************************/
Quaternion::Quaternion()
{
    this->r = 0.0f;
    this->i = 0.0f;
    this->j = 0.0f;
    this->k = 0.0f;
}

Quaternion::Quaternion(real _r, real _i, real _j, real _k)
{
    this->r = _r;
    this->i = _i;
    this->j = _j;
    this->k = _k;
}

void Quaternion::normalize()
{
    real d = this->r*this->r +
             this->i*this->i +
             this->j*this->j +
             this->k*this->k;
    
    // check for zero length quaternion, and use the no-rotation quaternion in that case.
    if (d == 0) {
        this->r = 1;
        return;
    }
    
    d = ((real)1.0)/real_sqrt(d);
    this->r *= d;
    this->i *= d;
    this->j *= d;
    this->k *= d;
}

void Quaternion::operator*=(const Quaternion &_quaternion)
{
    this->r = this->r*_quaternion.r - this->i*_quaternion.i -
              this->j*_quaternion.j - this->k*_quaternion.k;
    this->i = this->r*_quaternion.i + this->i*_quaternion.r +
              this->j*_quaternion.k - this->k*_quaternion.j;
    this->j = this->r*_quaternion.j + this->j*_quaternion.r +
              this->k*_quaternion.i - this->i*_quaternion.k;
    this->k = this->r*_quaternion.k + this->k*_quaternion.r +
              this->i*_quaternion.j - this->j*_quaternion.i;
}

void Quaternion::rotateByVector(const Vector3 &_vector)
{
    Quaternion quaternion(0, _vector.x, _vector.y, _vector.z);
    (*this) *= quaternion;
}

void Quaternion::addScaledVector(const Vector3 &_vector, real _scale)
{
    Quaternion quaternion(0 ,_vector.x * _scale, _vector.y * _scale, _vector.z * _scale);
    quaternion *= (*this);
    this->r += quaternion.r * ((real)0.5);
    this->i += quaternion.i * ((real)0.5);
    this->j += quaternion.j * ((real)0.5);
    this->k += quaternion.k * ((real)0.5);
}

/***************************** Camera *****************************/
Camera::Camera()
{
    this->eyeX = 0.0f;
    this->eyeY = 1.0f;
    this->eyeZ = 2.0f;

    this->centerX = 0.0f;
    this->centerY = 0.0f;
    this->centerZ = 0.0f;

    this->upX = 0.0f;
    this->upY = 1.0f;
    this->upZ = 0.0f;

    this->fovyRadians = DEGREES_TO_RADIANS(60.0f);

    this->nearZ = 0.1f;
    this->farZ = 10.0f;
}

//!resets the properties of the camera
void Camera::resetCamera()
{
    this->eyeX = 0.0f;
    this->eyeY = 1.0f;
    this->eyeZ = 2.0f;

    this->centerX = 0.0f;
    this->centerY = 0.0f;
    this->centerZ = 0.0f;

    this->lookAtMatrix = MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                          this->centerX, this->centerY, this->centerZ,
                                          this->upX, this->upY, this->upZ);
}

//!the camera is rotating as _radians, adjusting the matrix lookAt
void Camera::rotateCamera(real _radians)
{
    static int ang = 0;
    const static float radius = 3;

    this->eyeX = (radius * cos(M_PI * ang / 180.0f));
    this->centerX = -this->eyeX;
    this->eyeZ = (radius * sin(M_PI * ang / 180.0f));
    this->centerZ = -this->eyeZ;

    if (_radians < 0) {
        ang-=1;
    } else {
        ang+=1;
    }

    this->lookAtMatrix = MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                          this->centerX, this->centerY, this->centerZ,
                                          this->upX, this->upY, this->upZ);
}

//!adjusts the camera's perspective confome _aspect, adjusting matrices perspective and lookAt
void Camera::updatePerspective(real _aspect)
{
#if defined (_3D_)
    this->perspectiveMatrix = MatrixMakePerspective(this->fovyRadians, _aspect, this->nearZ, this->farZ);
    this->lookAtMatrix = MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                          this->centerX, this->centerY, this->centerZ,
                                          this->upX, this->upY, this->upZ);
#else
    MatrixOrtho(this->orthoMatrix, -_aspect, _aspect, -1, 1, -1, 1);
#endif
}

//!djusts the camera's zoom and confome _scale _value, adjusting the matrix lookAt
void Camera::zoom(real _scale, real _value)
{
#if defined (_3D_)
    this->eyeZ = _scale;

    this->lookAtMatrix = MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                          this->centerX, this->centerY, this->centerZ,
                                          this->upX, this->upY, this->upZ);
#else
    MatrixOrtho(this->orthoMatrix, -_value, _value, _scale, _scale, -1, 1);
#endif
}

void Camera::pan(real _scaleX, real _scaleY, real _aspect)
{
    this->eyeX = _scaleX;
    this->eyeY = _scaleY;

#if defined (_3D_)
    this->lookAtMatrix = MatrixMakeLookAt(this->eyeX, this->eyeY, this->eyeZ,
                                          this->centerX, this->centerY, this->centerZ,
                                          this->upX, this->upY, this->upZ);
#else
    MatrixOrtho(this->orthoMatrix, (-_aspect - _scaleX), (_aspect - _scaleX), (-1 -_scaleY), (1 - _scaleY), -1, 1);
#endif

}