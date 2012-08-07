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

/// Enumations
enum TypeObject {
    CIRCLE,
    SQUARE,
    TRIANGLE,
    POLYGON_OPEN,
    POLYGON_CLOSE,
    SPRINGS,
    STRING,
    ENGINE
};

//class Pointer {
//public:
//    float x, y, z, w;
//    Pointer() {};
//};

struct Pointer {
    float x;
    float y;
    float z;
    float w;
};

struct Color {
    unsigned char r, g, b, a;
};

struct BBox {
    Pointer * min;
    Pointer * max;
    float * p;
    
    BBox() {
        this->min = new Pointer();
        this->max = new Pointer();
        this->p = NULL;
    }
    ~BBox() {
        delete this->min;
        delete this->max;
        delete [] this->p;
        this->min = NULL;
        this->max = NULL;
        this->p = NULL;
    }
};

struct PhysicalFeature {
    float mass;
    float volume;
    float density;
    float acceleration;
    float speed;
};

static inline Pointer * MakePointer(float _x, float _y, float _z, float _w)
{
    Pointer * pointer = new Pointer();
    pointer->x = _x;
    pointer->y = _y;
    pointer->z = _z;
    pointer->w = _w;
    
    return pointer;
}

static inline Pointer * MakePointer(float _x, float _y, float _z)
{
    return MakePointer(_x, _y, _z, 1.0f);
}

static inline Pointer * MakePointer(float _x, float _y)
{
    return MakePointer(_x, _y, 0.0f, 1.0f);
}

static inline PhysicalFeature * MakePhysicalFeature(float _mass, float _volume, float _density, float _acceleration, float _speed)
{
    PhysicalFeature * pf = new PhysicalFeature();
    pf->mass = _mass;
    pf->volume = _volume;
    pf->density = _density;
    pf->acceleration = _acceleration;
    pf->speed = _speed;
    
    return pf;
}

static inline Color * MakeColor(float _r, float _g, float _b, float _a)
{
    Color * color = new Color();
    color->r = _r;
    color->g = _g;
    color->b = _b;
    color->a = _a;
    
    return color;
}

#endif
