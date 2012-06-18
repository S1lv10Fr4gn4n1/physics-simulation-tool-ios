//
//  SObject.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SObject.h"

using namespace std;

SObject::SObject()
{
    pointers = new Pointers();
    physicalFeature = MakePhysicalFeature(0, 0, 0, 0, 0); /// TODO - revise default
    color = MakeColor(255, 255, 255); /// TODO - revise default
}

SObject::~SObject()
{
//    if (pointers && pointers->count>0) {
//        delete pointers->at(0);
//    }
    delete pointers;
    delete physicalFeature;
    delete color;
}

void SObject::addPointer(Pointer _pointer)
{
    //points->push_back(_pointer);
    pointers = MakePointers(pointers, _pointer);
}

void SObject::deletePointer(Pointer * _pointer)
{
//    Pointer * p;
    
//    for (int i=0; 0<points->size(); i++) {
//        p = points->at(i);
//        
//        if (p->p == _pointer->p) { /// TODO - revise
//            points->erase(points->begin()+i);
//            
//            return;
//        }
//    }
}

Pointers * SObject::getPointers()
{
    return pointers;
}

Color * SObject::getColor()
{
    return color;
}

void SObject::setColor(Color * _color)
{
    color = _color;
}

PhysicalFeature * SObject::getPhysicalFeature()
{
    return physicalFeature;
}

void SObject::setPhysicalFeature(PhysicalFeature * _physicalFeature)
{
    physicalFeature = _physicalFeature;
}
