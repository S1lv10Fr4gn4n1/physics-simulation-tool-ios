//
//  CommonStruct.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H



/// Struct Pointer
struct Pointer {
    float x;
    float y;
    float z;
};
typedef Pointer Pointer;

struct Pointers {
    float * ps;
    int count;
};

static inline Pointer MakePointer(float _x, float _y, float _z) 
{
    Pointer pointer;
    pointer.x = _x;
    pointer.y = _y;
    pointer.z = _z;
    
    return pointer;
}

static inline Pointers * MakePointers(Pointers * _pointers, Pointer _point) 
{
    if (!_pointers) {
        _pointers = new Pointers();
        _pointers->ps = new float();
        _pointers->count = 0; 
    } else if (!_pointers->ps) {
        _pointers->ps = new float();
        _pointers->count = 0; 
    } else {
        _pointers->count++;
    }
    
    *(_pointers->ps+_pointers->count+0) = _point.x;
    *(_pointers->ps+_pointers->count+1) = _point.y;
    *(_pointers->ps+_pointers->count+2) = _point.z;
    
    return _pointers;
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
    unsigned char * c;
};
typedef Color Color;

static inline Color * MakeColor(unsigned char _r, unsigned char _g, unsigned char _b)
{
    Color * color = new Color();
    color->c = new unsigned char(3);
    *(color->c+0) = _r;
    *(color->c+1) = _g;
    *(color->c+2) = _b;
    
    return color;
}

#endif
