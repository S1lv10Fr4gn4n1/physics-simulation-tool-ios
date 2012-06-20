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
    /// TODO - implementation
}

NDC::~NDC()
{
    /// TODO - implementation
}

float calcDeltaO(float _maxWindow, float _minWindow)
{
    return _maxWindow - _minWindow;
}

float calcDeltaD(float _maxOrtho, float _minOrtho)
{
    return _maxOrtho - _minOrtho;
}

void NDC::calcNDCCoordinates(float * _x, float * _y)
{
    /// TODO revise
    float deltaO, deltaD;
    
    deltaO = calcDeltaO(maxWindow->x, minWindow->x);
    deltaD = calcDeltaD(maxOrtho->x, minOrtho->x);
    *_x = (*_x * (deltaD / deltaO)) + minOrtho->x;
    deltaO = calcDeltaO(maxWindow->y, minWindow->y);
    deltaD = calcDeltaD(maxOrtho->y, minOrtho->y);
    *_y = ((maxWindow->y - *_y) * (deltaD /deltaO)) + minOrtho->y;
}

void NDC::update(float _width, float _height)
{
//    maxWindow = MakePointer( 1,  1,  0), 
//    minWindow = MakePointer(-1, -1,  0), 
//    maxOrtho  = MakePointer(_width, _height, 0), 
//    minOrtho  = MakePointer( 0,  0,  0);
}
