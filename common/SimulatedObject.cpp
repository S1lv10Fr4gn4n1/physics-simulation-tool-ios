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
    this->pointers = new Pointers();
    this->pointersAux = new vector<Pointer *>();
    
    this->physicalFeature = MakePhysicalFeature(0, 0, 0, 0, 0);
    this->color = MakeColor(0, 0, 0, 0, 4);

    this->bbox = new BBox();
    this->bbox->max->x = this->bbox->max->y = this->bbox->max->z = -1000000;
	this->bbox->min->x = this->bbox->min->y = this->bbox->min->z =  1000000;	
}

SimulatedObject::~SimulatedObject()
{
    if (this->pointersAux && this->pointersAux->size() > 0) {
        delete this->pointersAux->at(0);
    }
    delete this->pointersAux;
    delete this->pointers;
    delete this->physicalFeature;
    delete this->color;
}

void SimulatedObject::loadBbox()
{
    Pointer * pointer = NULL;
    
    for(int i=0; i < this->pointersAux->size(); i++) {
		pointer = this->pointersAux->at(i);
		
		//definindo o maior X
		if (pointer->x > this->bbox->max->x) {
			this->bbox->max->x = pointer->x;
		}
		
		//definindo o menor X
		if (pointer->x < this->bbox->min->x) {
			this->bbox->min->x = pointer->x;
		}
		
		//definindo o maior Y
		if (pointer->y > this->bbox->max->y) {
			this->bbox->max->y = pointer->y;
		}
		
		//definindo o menor Y
		if (pointer->y < this->bbox->min->y) {
			this->bbox->min->y = pointer->y;
		}
        
		//definindo o maior Z
		if (pointer->z > this->bbox->max->z) {
			this->bbox->max->z = pointer->z;
		}
		
		//definindo o menor Z
		if (pointer->z < this->bbox->min->z) {
			this->bbox->min->z = pointer->z;
		}
	}
}

void SimulatedObject::initialize()
{
    this->loadBbox();
}

void SimulatedObject::addPointer(Pointer * _pointer)
{
    this->pointers = MakePointers(this->pointers, _pointer);
    this->pointersAux->push_back(CpyPointer(_pointer));
}

void SimulatedObject::deletePointer(Pointer * _pointer)
{
    Pointer * p;
    
    for (int i=0; 0<this->pointersAux->size(); i++) {
        p = this->pointersAux->at(i);
        
        if (p->x == _pointer->x &&
            p->y == _pointer->y &&
            p->z == _pointer->z) {
            this->pointersAux->erase(this->pointersAux->begin()+i);
            
            break;
        }
    }
    
    this->pointers = 0;
    this->pointers = new Pointers();
    for (int i=0; i<this->pointersAux->size(); i++) {
        this->pointers = MakePointers(this->pointers, this->pointersAux->at(i));
    }
}

void SimulatedObject::addAllPointers(std::vector<Pointer *> * _pointers)
{
    for (int i=0; i<_pointers->size(); i++) {
        if (_pointers->at(i) == 0) {
            continue;
        }
        this->pointers = MakePointers(this->pointers, _pointers->at(i));
        this->pointersAux->push_back(CpyPointer(_pointers->at(i)));
    }
}

Pointers * SimulatedObject::getPointers()
{
    return this->pointers;
}

std::vector<Pointer *> * SimulatedObject::getPointersAux()
{
    return this->pointersAux;
}

Color * SimulatedObject::getColor()
{
    return this->color;
}

void SimulatedObject::setColor(Color * _color)
{
    this->color = _color;
}

PhysicalFeature * SimulatedObject::getPhysicalFeature()
{
    return this->physicalFeature;
}

void SimulatedObject::setPhysicalFeature(PhysicalFeature * _physicalFeature)
{
    this->physicalFeature = _physicalFeature;
}

void SimulatedObject::setMode(unsigned int _mode)
{
    this->mode = _mode;
}

unsigned int SimulatedObject::getMode()
{
    return this->mode;
}

BBox * SimulatedObject::getBBox()
{
    return this->bbox;
}

void SimulatedObject::setBBox(BBox * _bbox)
{
    this->bbox = _bbox;
}

bool SimulatedObject::isShowBBox()
{
    return this->showBBox;
}

void SimulatedObject::setShowBBox(bool _show)
{
    this->showBBox = _show;
}

bool SimulatedObject::isSelected()
{
    return this->selected;
}

void SimulatedObject::setSelected(bool _selected)
{
    this->selected = _selected;
}

float * SimulatedObject::getMatrixTransformation()
{
    return this->matrixTransformation;
}

void SimulatedObject::setMatrixTransformation(float * _matrix)
{
    this->matrixTransformation = _matrix;
}