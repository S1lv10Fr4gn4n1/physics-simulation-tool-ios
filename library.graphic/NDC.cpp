
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
}

void NDC::calcNDCCoordinates(float * _x, float * _y)
{
    *_x = *_x * ((this->getRight() - this->getLeft()) / this->width) + this->getLeft();
    *_y = *_y * ((this->getBottom() - this->getTop()) / this->height) + this->getTop();    
}

void NDC::update(float _width, float _height)
{
    this->width = _width;
    this->height = _height;
    
    this->aspect = fabsf(this->width / this->height);
    
    if (this->maxOrtho == 0) {
        this->maxOrtho = MakePointer( 1,  1,  0);
    }
    
    if (this->minOrtho == 0) {
        this->minOrtho = MakePointer(-1, -1,  0);
    }
    
    this->maxWindow = MakePointer(this->width, this->height, 0);
    
    if (this->minWindow == 0) {
        this->minWindow = MakePointer( 0,  0,  0);
    }

    this->setLeft(-this->aspect);
    this->setRight(this->aspect);
}

float NDC::getAspect()
{
    return this->aspect;
}

float NDC::getLeft()
{
    return this->minOrtho->x;
}

void  NDC::setLeft(float _left)
{
    this->minOrtho->x = _left;
}

float NDC::getRight()
{
    return this->maxOrtho->x;
}

void NDC::setRight(float _right)
{
    this->maxOrtho->x = _right;
}

float NDC::getTop()
{
    return this->maxOrtho->y;
}

void NDC::setTop(float _top)
{
    this->maxOrtho->y = _top;
}

float NDC::getBottom()
{
    return this->minOrtho->y;
}

void NDC::setBottom(float _bottom)
{
    this->minOrtho->y = _bottom;
}