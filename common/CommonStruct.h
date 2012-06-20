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

/// Struct Pointer
struct Pointer {
    float x;
    float y;
    float z;
};
typedef Pointer Pointer;

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

static inline Pointers * MakePointers(Pointers * _pointers, Pointer * _point) 
{
    if (!_pointers) {
        _pointers = new Pointers();
        _pointers->pointers = new float();
        _pointers->count = 0; 
    } else if (!_pointers->pointers) {
        _pointers->pointers = new float();
        _pointers->count = 0; 
    }
    
    char index=_pointers->count*COUNT_COORD;
    *(_pointers->pointers+index+0) = _point->x;
    *(_pointers->pointers+index+1) = _point->y;
    *(_pointers->pointers+index+2) = _point->z;
    
    _pointers->count++;
    
    return _pointers;
}

static inline Pointer * CpyPointer(Pointer * _pointer)
{
    Pointer * p = new Pointer();
    p->x = _pointer->x;
    p->y = _pointer->y;
    p->z = _pointer->z;
    
    return p;
}


/// Struct PhysicalFeature
struct PhysicalFeature {
    float mass;
    float volume;
    float density;
    float acceleration;
    float speed;
};
typedef PhysicalFeature PhysicalFeature;

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
typedef Color Color;

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

#endif
