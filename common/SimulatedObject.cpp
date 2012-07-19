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

    bbox = new BBox();
    bbox->max->x = bbox->max->y = bbox->max->z = -1000000;
	bbox->min->x = bbox->min->y = bbox->min->z =  1000000;	
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

void SimulatedObject::loadBbox()
{
    Pointer * pointer = NULL;
    
    for(int i=0; i < pointersAux->size(); i++) {
		pointer = pointersAux->at(i);
		
		//point = [value transformPoint: point];
		
		//definindo o maior X
		if (pointer->x > bbox->max->x) {
			bbox->max->x = pointer->x;
		}
		
		//definindo o menor X
		if (pointer->x < bbox->min->x) {
			bbox->min->x = pointer->x;
		}
		
		//definindo o maior Y
		if (pointer->y > bbox->max->y) {
			bbox->max->y = pointer->y;
		}
		
		//definindo o menor Y
		if (pointer->y < bbox->min->y) {
			bbox->min->y = pointer->y;
		}
        
		//definindo o maior Z
		if (pointer->z > bbox->max->z) {
			bbox->max->z = pointer->z;
		}
		
		//definindo o menor Z
		if (pointer->z < bbox->min->z) {
			bbox->min->z = pointer->z;
		}
	}
}

void SimulatedObject::initialize()
{
    loadBbox();
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

void SimulatedObject::addAllPointers(std::vector<Pointer *> * _pointers)
{
    for (int i=0; i<_pointers->size(); i++) {
        pointers = MakePointers(pointers, _pointers->at(i));
        pointersAux->push_back(CpyPointer(_pointers->at(i)));
    }
}

Pointers * SimulatedObject::getPointers()
{
    return pointers;
}

std::vector<Pointer *> * SimulatedObject::getPointersAux()
{
    return pointersAux;
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

BBox * SimulatedObject::getBBox()
{
    return bbox;
}

void SimulatedObject::setBBox(BBox * _bbox)
{
    bbox = _bbox;
}
