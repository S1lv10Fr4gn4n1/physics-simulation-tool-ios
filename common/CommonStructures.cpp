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

Vector3::Vector3(const real _x, const real _y, const real _z)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
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

void Vector3::operator+=(const Vector3 * _vector)
{
    this->x += _vector->x;
    this->y += _vector->y;
    this->z += _vector->z;
}

void Vector3::operator=(const Vector3 * _vector)
{
    this->x = _vector->x;
    this->y = _vector->y;
    this->z = _vector->z;
}

Vector3 * Vector3::operator+(const Vector3 * _vector) const
{
    return new Vector3(this->x+_vector->x, this->y+_vector->y, this->z+_vector->z);
}

void Vector3::operator-=(const Vector3 * _vector)
{
    this->x -= _vector->x;
    this->y -= _vector->y;
    this->z -= _vector->z;
}

Vector3 * Vector3::operator-(const Vector3 * _vector) const
{
    return new Vector3(this->x-_vector->x, this->y-_vector->y, this->z-_vector->z);
}

void Vector3::addScaledVector(const Vector3 * _vector, real _scale)
{
    this->x += _vector->x * _scale;
    this->y += _vector->y * _scale;
    this->z += _vector->z * _scale;
}

Vector3 * Vector3::componentProduct(const Vector3 * _vector) const
{
    return new Vector3(this->x * _vector->x, this->y * _vector->y, this->z * _vector->z);
}
void Vector3::componentProductUpdate(const Vector3 * _vector)
{
    this->x *= _vector->x;
    this->y *= _vector->y;
    this->z *= _vector->z;
}

real Vector3::scalarProduct(const Vector3 * _vector) const
{
    return this->x*_vector->x + this->y*_vector->y + this->z*_vector->z;
}

real Vector3::operator *(const Vector3 * _vector) const
{
    return this->x*_vector->x + this->y*_vector->y + this->z*_vector->z;
}

void Vector3::operator*=(const real _value)
{
    this->x *= _value;
    this->y *= _value;
    this->z *= _value;
}

Vector3 * Vector3::operator*(const real _value) const
{
    return new Vector3(this->x*_value, this->y*_value, this->z*_value);
}

Vector3 * Vector3::vectorProduct(const Vector3 * _vector) const
{
    return new Vector3(this->y*_vector->z - this->z*_vector->y,
                       this->z*_vector->x - this->x*_vector->z,
                       this->x*_vector->y - this->y*_vector->x);
}

void Vector3::operator%=(const Vector3 * _vector)
{
    *this = *this->vectorProduct(_vector);
}

Vector3 * Vector3::operator%(const Vector3 * _vector) const
{
    return new Vector3(this->y*_vector->z - this->z*_vector->y,
                       this->z*_vector->x - this->x*_vector->z,
                       this->x*_vector->y - this->y*_vector->x);
}

void Vector3::invert()
{
    this->x = -this->x;
    this->y = -this->y;
    this->x = -this->z;
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

Vector3 * Vector3::MakeVector3(real _x, real _y, real _z)
{
    Vector3 * vector = new Vector3();
    vector->x = _x;
    vector->y = _y;
    vector->z = _z;
    
    return vector;
}

Vector3 * Vector3::MakeVector3(real _x, real _y)
{
    return MakeVector3(_x, _y, 0.0f);
}

Vector3 * Vector3::MakeVector3(const Vector3 * _vector)
{
    return MakeVector3(_vector->x, _vector->y, _vector->z);
}


/***************************** Color *****************************/
Color * Color::MakeColor(real _r, real _g, real _b, real _a)
{
    Color * color = new Color();
    color->r = _r;
    color->g = _g;
    color->b = _b;
    color->a = _a;

    return color;
}

Color * Color::MakeRandonColor()
{
    return Color::MakeColor(static_cast<unsigned char>(rand() % 256),
                            static_cast<unsigned char>(rand() % 256),
                            static_cast<unsigned char>(rand() % 256),
                            1);
}


/***************************** BBox *****************************/
BBox::BBox()
{
    this->min = new Vector3();
    this->max = new Vector3();
    this->ptr = NULL;
    this->color = colorBBox;
}

BBox::~BBox()
{
    delete this->min;
    delete this->max;
    delete [] this->ptr;
    this->min = NULL;
    this->max = NULL;
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

Vector3 * Matrix3::operator*(const Vector3 * _vector) const
{
    return new Vector3(_vector->x * this->data[0] + _vector->y * this->data[1] + _vector->z * this->data[2],
                       _vector->x * this->data[3] + _vector->y * this->data[4] + _vector->z * this->data[5],
                       _vector->x * this->data[6] + _vector->y * this->data[7] + _vector->z * this->data[8]
                       );
}

Matrix3 * Matrix3::operator*(const Matrix3 * _matrix) const
{
    return new Matrix3(this->data[0]*_matrix->data[0] + this->data[1]*_matrix->data[3] + this->data[2]*_matrix->data[6],
                       this->data[0]*_matrix->data[1] + this->data[1]*_matrix->data[4] + this->data[2]*_matrix->data[7],
                       this->data[0]*_matrix->data[2] + this->data[1]*_matrix->data[5] + this->data[2]*_matrix->data[8],
                       this->data[3]*_matrix->data[0] + this->data[4]*_matrix->data[3] + this->data[5]*_matrix->data[6],
                       this->data[3]*_matrix->data[1] + this->data[4]*_matrix->data[4] + this->data[5]*_matrix->data[7],
                       this->data[3]*_matrix->data[2] + this->data[4]*_matrix->data[5] + this->data[5]*_matrix->data[8],
                       this->data[6]*_matrix->data[0] + this->data[7]*_matrix->data[3] + this->data[8]*_matrix->data[6],
                       this->data[6]*_matrix->data[1] + this->data[7]*_matrix->data[4] + this->data[8]*_matrix->data[7],
                       this->data[6]*_matrix->data[2] + this->data[7]*_matrix->data[5] + this->data[8]*_matrix->data[8]
                       );
}

void Matrix3::setInverse(const Matrix3 * _matrix)
{
    real t4 = _matrix->data[0]*_matrix->data[4];
    real t6 = _matrix->data[0]*_matrix->data[5];
    real t8 = _matrix->data[1]*_matrix->data[3];
    real t10 = _matrix->data[2]*_matrix->data[3];
    real t12 = _matrix->data[1]*_matrix->data[6];
    real t14 = _matrix->data[2]*_matrix->data[6];
    
    // calculate the determinant.
    real t16 = (t4*_matrix->data[8] - t6*_matrix->data[7] - t8*_matrix->data[8] +
                t10*_matrix->data[7] + t12*_matrix->data[5] - t14*_matrix->data[4]);
    
    // make sure the determinant is non-zer_matrix
    if (t16 == (real)0.0f) {
        return;
    }
    
    real t17 = 1/t16;
    this->data[0] = (_matrix->data[4]*_matrix->data[8]-_matrix->data[5]*_matrix->data[7])*t17;
    this->data[1] = -(_matrix->data[1]*_matrix->data[8]-_matrix->data[2]*_matrix->data[7])*t17;
    this->data[2] = (_matrix->data[1]*_matrix->data[5]-_matrix->data[2]*_matrix->data[4])*t17;
    this->data[3] = -(_matrix->data[3]*_matrix->data[8]-_matrix->data[5]*_matrix->data[6])*t17;
    this->data[4] = (_matrix->data[0]*_matrix->data[8]-t14)*t17;
    this->data[5] = -(t6-t10)*t17;
    this->data[6] = (_matrix->data[3]*_matrix->data[7]-_matrix->data[4]*_matrix->data[6])*t17;
    this->data[7] = -(_matrix->data[0]*_matrix->data[7]-t12)*t17;
    this->data[8] = (t4-t8)*t17;
}

Matrix3 * Matrix3::inverse() const
{
    Matrix3 * matrix = new Matrix3();
    matrix->setInverse(this);
    
    return matrix;
}

void Matrix3::invert()
{
    this->setInverse(this);
}

void Matrix3::setTranspose(const Matrix3 * _matrix)
{
    this->data[0] = _matrix->data[0];
    this->data[1] = _matrix->data[3];
    this->data[2] = _matrix->data[6];
    this->data[3] = _matrix->data[1];
    this->data[4] = _matrix->data[4];
    this->data[5] = _matrix->data[7];
    this->data[6] = _matrix->data[2];
    this->data[7] = _matrix->data[5];
    this->data[8] = _matrix->data[8];
}

Matrix3 * Matrix3::transpose() const
{
    Matrix3 * matrix = new Matrix3();
    matrix->setTranspose(this);
    
    return matrix;
}

void Matrix3::setOrientation(const Quaternion * _quaternion)
{
    this->data[0] = 1 - (2*_quaternion->j*_quaternion->j + 2*_quaternion->k*_quaternion->k);
    this->data[1] = 2*_quaternion->i*_quaternion->j + 2*_quaternion->k*_quaternion->r;
    this->data[2] = 2*_quaternion->i*_quaternion->k - 2*_quaternion->j*_quaternion->r;
    this->data[3] = 2*_quaternion->i*_quaternion->j - 2*_quaternion->k*_quaternion->r;
    this->data[4] = 1 - (2*_quaternion->i*_quaternion->i  + 2*_quaternion->k*_quaternion->k);
    this->data[5] = 2*_quaternion->j*_quaternion->k + 2*_quaternion->i*_quaternion->r;
    this->data[6] = 2*_quaternion->i*_quaternion->k + 2*_quaternion->j*_quaternion->r;
    this->data[7] = 2*_quaternion->j*_quaternion->k - 2*_quaternion->i*_quaternion->r;
    this->data[8] = 1 - (2*_quaternion->i*_quaternion->i  + 2*_quaternion->j*_quaternion->j);
}

Vector3 * Matrix3::transform(const Vector3 * _vector) const
{
    return *this * _vector;
}

void Matrix3::setComponents(const Vector3 * _compOne, const Vector3 * _compTwo, const Vector3 * _compThree)
{
    this->data[0] = _compOne->x;
    this->data[1] = _compTwo->x;
    this->data[2] = _compThree->x;
    this->data[3] = _compOne->y;
    this->data[4] = _compTwo->y;
    this->data[5] = _compThree->y;
    this->data[6] = _compOne->z;
    this->data[7] = _compTwo->z;
    this->data[8] = _compThree->z;
};

Vector3 * Matrix3::transformTranspose(const Vector3 * _vector) const
{
    return new Vector3(_vector->x * this->data[0] + _vector->y * this->data[3] + _vector->z * this->data[6],
                       _vector->x * this->data[1] + _vector->y * this->data[4] + _vector->z * this->data[7],
                       _vector->x * this->data[2] + _vector->y * this->data[5] + _vector->z * this->data[8]
                       );
}


/***************************** Matrix4 *****************************/

Matrix4::Matrix4()
{
    this->data[0] = this->data[1] = this->data[2] = this->data[3] =
    this->data[4] = this->data[5] = this->data[6] = this->data[7] =
    this->data[8] = this->data[9] = this->data[10] = this->data[11] =0;
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


void Matrix4::setOrientationAndPos(const Quaternion * _quaternion, const Vector3 * _pos)
{
    this->data[0] = 1 - (2*_quaternion->j*_quaternion->j + 2*_quaternion->k*_quaternion->k);
    this->data[1] = 2*_quaternion->i*_quaternion->j + 2*_quaternion->k*_quaternion->r;
    this->data[2] = 2*_quaternion->i*_quaternion->k - 2*_quaternion->j*_quaternion->r;
    this->data[3] = _pos->x;
    this->data[4] = 2*_quaternion->i*_quaternion->j - 2*_quaternion->k*_quaternion->r;
    this->data[5] = 1 - (2*_quaternion->i*_quaternion->i  + 2*_quaternion->k*_quaternion->k);
    this->data[6] = 2*_quaternion->j*_quaternion->k + 2*_quaternion->i*_quaternion->r;
    this->data[7] = _pos->y;
    this->data[8] = 2*_quaternion->i*_quaternion->k + 2*_quaternion->j*_quaternion->r;
    this->data[9] = 2*_quaternion->j*_quaternion->k - 2*_quaternion->i*_quaternion->r;
    this->data[10] = 1 - (2*_quaternion->i*_quaternion->i  + 2*_quaternion->j*_quaternion->j);
    this->data[11] = _pos->z;
}

void Matrix4::setInverse(const Matrix4 * _matrix)
{
    // make sure the determinant is non-zero.
    real det = this->getDeterminant();
    
    if (det == 0) {
        return;
    }
    
    det = ((real)1.0)/det;
    
    this->data[0] = (-_matrix->data[9]*_matrix->data[6]+_matrix->data[5]*_matrix->data[10])*det;
    this->data[4] = (_matrix->data[8]*_matrix->data[6]-_matrix->data[4]*_matrix->data[10])*det;
    this->data[8] = (-_matrix->data[8]*_matrix->data[5]+_matrix->data[4]*_matrix->data[9])*det;
    
    this->data[1] = (_matrix->data[9]*_matrix->data[2]-_matrix->data[1]*_matrix->data[10])*det;
    this->data[5] = (-_matrix->data[8]*_matrix->data[2]+_matrix->data[0]*_matrix->data[10])*det;
    this->data[9] = (_matrix->data[8]*_matrix->data[1]-_matrix->data[0]*_matrix->data[9])*det;
    
    this->data[2] = (-_matrix->data[5]*_matrix->data[2]+_matrix->data[1]*_matrix->data[6])*det;
    this->data[6] = (+_matrix->data[4]*_matrix->data[2]-_matrix->data[0]*_matrix->data[6])*det;
    this->data[10] = (-_matrix->data[4]*_matrix->data[1]+_matrix->data[0]*_matrix->data[5])*det;
    
    this->data[3] = (_matrix->data[9]*_matrix->data[6]*_matrix->data[3]
                     -_matrix->data[5]*_matrix->data[10]*_matrix->data[3]
                     -_matrix->data[9]*_matrix->data[2]*_matrix->data[7]
                     +_matrix->data[1]*_matrix->data[10]*_matrix->data[7]
                     +_matrix->data[5]*_matrix->data[2]*_matrix->data[11]
                     -_matrix->data[1]*_matrix->data[6]*_matrix->data[11])*det;
    this->data[7] = (-_matrix->data[8]*_matrix->data[6]*_matrix->data[3]
                     +_matrix->data[4]*_matrix->data[10]*_matrix->data[3]
                     +_matrix->data[8]*_matrix->data[2]*_matrix->data[7]
                     -_matrix->data[0]*_matrix->data[10]*_matrix->data[7]
                     -_matrix->data[4]*_matrix->data[2]*_matrix->data[11]
                     +_matrix->data[0]*_matrix->data[6]*_matrix->data[11])*det;
    this->data[11] =(_matrix->data[8]*_matrix->data[5]*_matrix->data[3]
                     -_matrix->data[4]*_matrix->data[9]*_matrix->data[3]
                     -_matrix->data[8]*_matrix->data[1]*_matrix->data[7]
                     +_matrix->data[0]*_matrix->data[9]*_matrix->data[7]
                     +_matrix->data[4]*_matrix->data[1]*_matrix->data[11]
                     -_matrix->data[0]*_matrix->data[5]*_matrix->data[11])*det;}

Matrix4 * Matrix4::inverse() const
{
    Matrix4 * matrix = new Matrix4();
    matrix->setInverse(this);
    
    return matrix;
}

void Matrix4::invert()
{
    this->setInverse(this);
}

Matrix4 * Matrix4::operator*(const Matrix4 * _matrix) const
{
    Matrix4 * matrix = new Matrix4();
    matrix->data[0] = (_matrix->data[0]*this->data[0]) + (_matrix->data[4]*this->data[1]) + (_matrix->data[8]*this->data[2]);
    matrix->data[4] = (_matrix->data[0]*this->data[4]) + (_matrix->data[4]*this->data[5]) + (_matrix->data[8]*this->data[6]);
    matrix->data[8] = (_matrix->data[0]*this->data[8]) + (_matrix->data[4]*this->data[9]) + (_matrix->data[8]*this->data[10]);
    
    matrix->data[1] = (_matrix->data[1]*this->data[0]) + (_matrix->data[5]*this->data[1]) + (_matrix->data[9]*this->data[2]);
    matrix->data[5] = (_matrix->data[1]*this->data[4]) + (_matrix->data[5]*this->data[5]) + (_matrix->data[9]*this->data[6]);
    matrix->data[9] = (_matrix->data[1]*this->data[8]) + (_matrix->data[5]*this->data[9]) + (_matrix->data[9]*this->data[10]);
    
    matrix->data[2] = (_matrix->data[2]*this->data[0]) + (_matrix->data[6]*this->data[1]) + (_matrix->data[10]*this->data[2]);
    matrix->data[6] = (_matrix->data[2]*this->data[4]) + (_matrix->data[6]*this->data[5]) + (_matrix->data[10]*this->data[6]);
    matrix->data[10] = (_matrix->data[2]*this->data[8]) + (_matrix->data[6]*this->data[9]) + (_matrix->data[10]*this->data[10]);
    
    matrix->data[3] = (_matrix->data[3]*this->data[0]) + (_matrix->data[7]*this->data[1]) + (_matrix->data[11]*this->data[2]) + this->data[3];
    matrix->data[7] = (_matrix->data[3]*this->data[4]) + (_matrix->data[7]*this->data[5]) + (_matrix->data[11]*this->data[6]) + this->data[7];
    matrix->data[11] = (_matrix->data[3]*this->data[8]) + (_matrix->data[7]*this->data[9]) + (_matrix->data[11]*this->data[10]) + this->data[11];

    return matrix;
}

Vector3 * Matrix4::operator*(const Vector3 * _vector) const
{
    return new Vector3(_vector->x * this->data[0] +
                       _vector->y * this->data[1] +
                       _vector->z * this->data[2] + this->data[3],
                   
                       _vector->x * this->data[4] +
                       _vector->y * this->data[5] +
                       _vector->z * this->data[6] + this->data[7],
                       
                       _vector->x * this->data[8] +
                       _vector->y * this->data[9] +
                       _vector->z * this->data[10] + this->data[11]
                       );
}

Vector3 * Matrix4::transformInverse(const Vector3 * _vector) const
{
    Vector3 * tmp = new Vector3(_vector->x, _vector->y, _vector->z);
    tmp->x -= this->data[3];
    tmp->y -= this->data[7];
    tmp->z -= this->data[11];
    Vector3 * result = new Vector3(tmp->x * this->data[0] +
                                   tmp->y * this->data[4] +
                                   tmp->z * this->data[8],
                                   tmp->x * this->data[1] +
                                   tmp->y * this->data[5] +
                                   tmp->z * this->data[9],
                                   tmp->x * this->data[2] +
                                   tmp->y * this->data[6] +
                                   tmp->z * this->data[10]
                                   );
    delete tmp;
    tmp = NULL;
    
    return result;
}

Vector3 * Matrix4::localToWorld(const Vector3 * _local, const Matrix4 * _transform)
{
    return _transform->transform(_local);
}

Vector3 * Matrix4::transform(const Vector3 * _vector) const
{
    return *this * _vector;
}

Vector3 * Matrix4::worldToLocal(const Vector3 * _world, const Matrix4 * _transform)
{
    return _transform->transformInverse(_world);
}

Vector3 * Matrix4::transformDirection(const Vector3 * _vector) const
{
    return new Vector3(_vector->x * this->data[0] +
                       _vector->y * this->data[1] +
                       _vector->z * this->data[2],
                       _vector->x * this->data[4] +
                       _vector->y * this->data[5] +
                       _vector->z * this->data[6],
                       _vector->x * this->data[8] +
                       _vector->y * this->data[9] +
                       _vector->z * this->data[10]
                       );
}

Vector3 * Matrix4::transformInverseDirection(const Vector3 * _vector) const
{
    return new Vector3(_vector->x * this->data[0] +
                       _vector->y * this->data[4] +
                       _vector->z * this->data[8],
                       _vector->x * this->data[1] +
                       _vector->y * this->data[5] +
                       _vector->z * this->data[9],
                       _vector->x * this->data[2] +
                       _vector->y * this->data[6] +
                       _vector->z * this->data[10]
                       );
}

Vector3 * Matrix4::localToWorldDirn(const Vector3 * _local, const Matrix4 * _transform)
{
    return _transform->transformDirection(_local);
}

Vector3 * Matrix4::worldToLocalDirn(const Vector3 * _world, const Matrix4 * _transform)
{
    return _transform->transformInverseDirection(_world);
}

// gets a vector representing one axis, one column, in matrix.
// row 3 corresponds to the position of the transform matrix.
Vector3 * Matrix4::getAxisVector(int _i) const
{
    return new Vector3(this->data[_i], this->data[_i+4], this->data[_i+8]);
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

void Quaternion::operator *=(const Quaternion * _quaternion)
{
    this->r = this->r*_quaternion->r - this->i*_quaternion->i -
              this->j*_quaternion->j - this->k*_quaternion->k;
    this->i = this->r*_quaternion->i + this->i*_quaternion->r +
              this->j*_quaternion->k - this->k*_quaternion->j;
    this->j = this->r*_quaternion->j + this->j*_quaternion->r +
              this->k*_quaternion->i - this->i*_quaternion->k;
    this->k = this->r*_quaternion->k + this->k*_quaternion->r +
              this->i*_quaternion->j - this->j*_quaternion->i;
}

void Quaternion::rotateByVector(const Vector3 * _vector)
{
    Quaternion * quaternion = new Quaternion(0, _vector->x, _vector->y, _vector->z);
    *this *= quaternion;
}

void Quaternion::addScaledVector(const Vector3 * _vector, real _scale)
{
    Quaternion * quaternion = new Quaternion(0 ,_vector->x * _scale, _vector->y * _scale, _vector->z * _scale);
    *quaternion *= this;
    this->r += quaternion->r * ((real)0.5);
    this->i += quaternion->i * ((real)0.5);
    this->j += quaternion->j * ((real)0.5);
    this->k += quaternion->k * ((real)0.5);
}