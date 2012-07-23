//
//  NDC.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 14/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "NDC.h"

NDC::NDC()
{
    this->maxWindow = 0;
    this->minWindow = 0;
    this->maxOrtho = 0;
    this->minOrtho = 0;
}

NDC::~NDC()
{
    delete this->maxWindow;
    delete this->minWindow;
    delete this->maxOrtho;
    delete this->minOrtho;

    this->maxWindow = 0;
    this->minWindow = 0;
    this->maxOrtho = 0;
    this->minOrtho = 0;
}

void NDC::calcNDCCoordinates(float * _x, float * _y)
{
    float deltaO = (this->maxWindow->x - this->minWindow->x);
    float deltaD = (this->maxOrtho->x - this->minOrtho->x);
    *_x = (*_x * (deltaD / deltaO)) + this->minOrtho->x;
    deltaO = (this->maxWindow->y - this->minWindow->y);
    deltaD = (this->maxOrtho->y - this->minOrtho->y);
    *_y = ((this->maxWindow->y - *_y) * (deltaD /deltaO)) + this->minOrtho->y;
}

void NDC::update(float _width, float _height)
{
    if (this->maxOrtho == 0) {
        this->maxOrtho = MakePointer( 1,  1,  0);
    }
    
    if (this->minOrtho == 0) {
        this->minOrtho = MakePointer(-1, -1,  0);
    }
    
    this->maxWindow = MakePointer(_width, _height, 0);
    
    if (this->minWindow == 0) {
        this->minWindow = MakePointer( 0,  0,  0);
    }
}
