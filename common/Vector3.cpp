//
//  Vector3.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 13/08/12.
//
//

#include "Vector3.h"

Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->pad = 0;
}

Vector3::Vector3(const real _x, const real _y, const real _z)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
}

Vector3::~Vector3()
{
    // TODO put your code here
}

void Vector3::operator+=(const Vector3 * _vector)
{
    this->x += _vector->x;
    this->y += _vector->y;
    this->x += _vector->z;
}

Vector3 * Vector3::operator+(const Vector3 * _vector) const
{
    return new Vector3(this->x+_vector->x, this->y+_vector->y, this->z+_vector->z);
}

void Vector3::operator-=(const Vector3 * _vector)
{
    this->x -= _vector->x;
    this->y -= _vector->y;
    this->x -= _vector->z;
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

void Vector3::multiplyVectorForValue(const real _value)
{
    this->x *= _value;
    this->y *= _value;
    this->z *= _value;
}

Vector3 * Vector3::vectorProduct(const Vector3 * _vector) const
{
    return new Vector3(this->y*_vector->z - this->z*_vector->y,
                       this->z*_vector->x - this->x*_vector->z,
                       this->x*_vector->y - this->y*_vector->x);
}

//void Vector3::operator%=(const Vector3 * _vector)
//{
//    this = this->vectorProduct(_vector);
//}

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
    real l = magnitude();
    if (l > 0) {
        (*this)*=((real)1)/l;
    }
}