
//  NDC.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 14/06/12.
//  
//

#include "NDC.h"

NDC::NDC()
{
    this->maxWindow = NULL;
    this->minWindow = NULL;
    this->maxOrtho = NULL;
    this->minOrtho = NULL;

    this->aspect = 0.0f;
    this->width = 0.0f;
    this->height = 0.0f;
}

NDC::~NDC()
{
    if (this->maxWindow) {
        delete this->maxWindow;
    }
    if (this->minWindow) {
        delete this->minWindow;
    }
    if (this->maxOrtho) {
        delete this->maxOrtho;
    }
    if (this->minOrtho) {
        delete this->minOrtho;
    }

    this->maxWindow = NULL;
    this->minWindow = NULL;
    this->maxOrtho = NULL;
    this->minOrtho = NULL;
}

void NDC::calcNDCCoordinates(real * _x, real * _y)
{
    *_x = *_x * ((this->getRight() - this->getLeft()) / this->width) + this->getLeft();
    *_y = *_y * ((this->getBottom() - this->getTop()) / this->height) + this->getTop();    
}

//!Method responsible for updating the content of the NDC, as the size of the screen
void NDC::update(real _width, real _height)
{
    this->width = _width;
    this->height = _height;
    
    this->aspect = fabsf(this->width / this->height);
    
    if (this->maxOrtho == NULL) {
        this->maxOrtho = new Vector3(1.0f, 1.0f);
    }
    
    if (this->minOrtho == NULL) {
        this->minOrtho = new Vector3(-1.0f, -1.0f);
    }
    
    if (this->maxWindow) {
        delete this->maxWindow;
    }
    this->maxWindow = new Vector3(this->width, this->height);
    
    if (this->minWindow == NULL) {
        this->minWindow = new Vector3(0.0f, 0.0f);
    }

    this->setLeft(-this->aspect);
    this->setRight(this->aspect);
}

real NDC::getAspect()
{
    return this->aspect;
}

real NDC::getLeft()
{
    return this->minOrtho->x;
}

void  NDC::setLeft(real _left)
{
    this->minOrtho->x = _left;
}

real NDC::getRight()
{
    return this->maxOrtho->x;
}

void NDC::setRight(real _right)
{
    this->maxOrtho->x = _right;
}

real NDC::getTop()
{
    return this->maxOrtho->y;
}

void NDC::setTop(real _top)
{
    this->maxOrtho->y = _top;
}

real NDC::getBottom()
{
    return this->minOrtho->y;
}

void NDC::setBottom(real _bottom)
{
    this->minOrtho->y = _bottom;
}

Vector3 * NDC::getMaxWindow()
{
    return this->maxWindow;
}