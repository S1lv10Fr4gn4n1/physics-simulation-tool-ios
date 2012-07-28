//
//  CommonStruct.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
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

/// Struct Pointer
struct Pointer {
    float x;
    float y;
    float z;
};

static inline Pointer * MakePointer(float _x, float _y, float _z) 
{
    Pointer * pointer = new Pointer();
    pointer->x = _x;
    pointer->y = _y;
    pointer->z = _z;
    
    return pointer;
}

/// Struct Pointers
struct Pointers {
    float * pointers;
    unsigned int count;
};


/// Struct PhysicalFeature
struct PhysicalFeature {
    float mass;
    float volume;
    float density;
    float acceleration;
    float speed;
};

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

/// Struct Color
struct Color {
    unsigned char * color;
};

static inline Color * MakeColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a, unsigned char _vertexes)
{
    Color * color = new Color();
    color->color = new unsigned char(16);
    
    char index=0;
    
    for (int i=0; i<_vertexes ; i++) {
        index=i*CHANNEL_COLOR;
        *(color->color+index+0) = _r;
        *(color->color+index+1) = _g;
        *(color->color+index+2) = _b;
        *(color->color+index+3) = _a;
    }
    
    return color;
}

struct BBox {
    Pointer * min;
    Pointer * max;
    
    BBox() {
        min = new Pointer();
        max = new Pointer();
    }
    ~BBox() {
        delete min;
        delete max;
    }    
};

static inline float * MakeMatrixIdentity()
{
    float * matrix = new float[16];
    for (int i=0; i< sizeof(matrix); i++) {
        *(matrix+i) = 0;
    }
    
    return matrix;
}

#endif
