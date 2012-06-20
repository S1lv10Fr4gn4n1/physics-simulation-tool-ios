//
//  SimulatedObject.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SimulatedObject.h"

using namespace std;

SimulatedObject::SimulatedObject()
{
    pointers = new Pointers();
    pointersAux = new vector<Pointer *>();
    
    physicalFeature = MakePhysicalFeature(0, 0, 0, 0, 0); /// TODO - revise default
    color = MakeColor(0, 0, 0, 0, 4);
}

SimulatedObject::~SimulatedObject()
{
    if (pointersAux && pointersAux->size() > 0) {
        delete pointersAux->at(0);
    }
    delete pointersAux;
    delete pointers;
    delete physicalFeature;
    delete color;
}

void SimulatedObject::addPointer(Pointer * _pointer)
{
    pointers = MakePointers(pointers, _pointer);
    pointersAux->push_back(CpyPointer(_pointer));
}

void SimulatedObject::deletePointer(Pointer * _pointer)
{
    Pointer * p;
    
    for (int i=0; 0<pointersAux->size(); i++) {
        p = pointersAux->at(i);
        
        if (p->x == _pointer->x &&
            p->y == _pointer->y &&
            p->z == _pointer->z) {
            pointersAux->erase(pointersAux->begin()+i);
            
            break;
        }
    }
    
    pointers = 0;
    pointers = new Pointers();
    for (int i=0; i<pointersAux->size(); i++) {
        pointers = MakePointers(pointers, pointersAux->at(i));
    }
}

Pointers * SimulatedObject::getPointers()
{
    return pointers;
}

Color * SimulatedObject::getColor()
{
    return color;
}

void SimulatedObject::setColor(Color * _color)
{
    color = _color;
}

PhysicalFeature * SimulatedObject::getPhysicalFeature()
{
    return physicalFeature;
}

void SimulatedObject::setPhysicalFeature(PhysicalFeature * _physicalFeature)
{
    physicalFeature = _physicalFeature;
}

void SimulatedObject::setMode(unsigned int _mode)
{
    mode = _mode;
}

unsigned int SimulatedObject::getMode()
{
    return mode;
}

