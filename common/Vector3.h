//
//  Vector3.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 13/08/12.
//
//

#ifndef VECTOR3_H
#define VECTOR3_H

#include "Precision.h"
#include <math.h>

class Vector3 {
private:
    real pad;
public:
    real x, y, z;
    
    Vector3();
    Vector3(const real _x, const real _y, const real _z);
    ~Vector3();
    
    void operator+=(const Vector3 *_vector);
    Vector3 * operator+(const Vector3 *_vector) const;
    void operator-=(const Vector3 *_vector);
    Vector3 * operator-(const Vector3 *_vector) const;
    void addScaledVector(const Vector3 *_vector, real _scale);
    Vector3 * componentProduct(const Vector3 *_vector) const;
    void componentProductUpdate(const Vector3 *_vector);
    real scalarProduct(const Vector3 *_vector) const;
    real operator *(const Vector3 *_vector) const;
    void operator*=(const real _value);
    void multiplyVectorForValue(const real _value);
    Vector3 * operator*(const real _value) const;
    Vector3 * vectorProduct(const Vector3 *_vector) const;
//    void operator%=(const Vector3 *_vector);
    Vector3 * operator%(const Vector3 *_vector) const;
    
    void invert();
    real magnitude() const;
    real squareMagnitude() const;
    void normalize();
    
};

#endif
