//
//  CommonStruct.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  
//

#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H

#define CHANNEL_COLOR 4
#define COUNT_COORD 3

#include <stdlib.h>
#include "Vector3.h"

/// Enumations
enum TypeObject {
    CIRCLE,
    SQUARE,
    TRIANGLE,
    POLYGON_OPEN,
    POLYGON_CLOSE,
    SPRINGS,
    STRING,
    ENGINE,
    PLAN,
    TEST
};

struct Color {
    unsigned char r, g, b, a;
};

static const real colorBBox[16] = {
    255, 255, 255, 1,
    255, 255, 255, 1,
    255, 255, 255, 1,
    255, 255, 255, 1,
};

struct BBox {
    Vector3 * min;
    Vector3 * max;
    real * ptr;
    const real * color;
    
    BBox() {
        this->min = new Vector3();
        this->max = new Vector3();
        this->ptr = NULL;
        this->color = colorBBox;
    }
    ~BBox() {
        delete this->min;
        delete this->max;
        delete [] this->ptr;
        this->min = NULL;
        this->max = NULL;
        this->ptr = NULL;
    }
};

static inline Vector3 * MakeVector3(real _x, real _y, real _z)
{
    Vector3 * vector = new Vector3();
    vector->x = _x;
    vector->y = _y;
    vector->z = _z;
    
    return vector;
}

static inline Vector3 * MakeVector3(real _x, real _y)
{
    return MakeVector3(_x, _y, 0.0f);
}

static inline Vector3 * MakeVector3(Vector3 * _vector)
{
    return MakeVector3(_vector->x, _vector->y, _vector->z);
}

static inline Color * MakeColor(real _r, real _g, real _b, real _a)
{
    Color * color = new Color();
    color->r = _r;
    color->g = _g;
    color->b = _b;
    color->a = _a;
    
    return color;
}

static inline Color * MakeRandonColor()
{
    return MakeColor(static_cast<unsigned char>(rand() % 256),
                     static_cast<unsigned char>(rand() % 256),
                     static_cast<unsigned char>(rand() % 256),
                     1);
}

#endif
