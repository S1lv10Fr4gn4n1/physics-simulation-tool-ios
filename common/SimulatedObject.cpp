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
    this->showBBox = false;
    this->selected = false;
    
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
        Pointer * pointer = 0;
        for (int i=0; i < this->pointersAux->size(); i++) {
            pointer = this->pointersAux->at(i);
            this->pointersAux->erase(this->pointersAux->begin()+i);
            delete pointer;
        }
    }
    
    if (this->joinsSimulatedObject && this->joinsSimulatedObject->size() > 0) {
        SimulatedObject * simulatedObject = 0;
        for (int i=0; i < this->joinsSimulatedObject->size(); i++) {
            simulatedObject = this->joinsSimulatedObject->at(i);
            this->joinsSimulatedObject->erase(this->joinsSimulatedObject->begin()+i);
            delete simulatedObject;
        }
    }
        
    delete this->pointersAux;
    delete this->pointers;
    delete this->physicalFeature;
    delete this->color;
    delete this->bbox;
    delete this->matrixTransformation;
}

void SimulatedObject::loadBbox()
{
    Pointer * pointer = 0;
    
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
    // initialize array of float for drawing
    this->pointers->pointers = new float[this->pointersAux->size()*COUNT_COORD];
    int index = 0;
    Pointer * pointer = 0;
    
    for (int i=0; i<this->pointersAux->size(); i++) {
        pointer = this->pointersAux->at(i);
        index = i*COUNT_COORD;
        *(this->pointers->pointers+index+0) = pointer->x;
        *(this->pointers->pointers+index+1) = pointer->y;
        *(this->pointers->pointers+index+2) = pointer->z;
    }
    
    this->pointers->count = this->pointersAux->size();

    // initialize bbox
    this->loadBbox();
}

void SimulatedObject::addPointer(Pointer * _pointer)
{
    this->pointersAux->push_back(_pointer);
}

void SimulatedObject::deletePointer(Pointer * _pointer)
{
    Pointer * pointer = 0;
    
    for (int i=0; 0<this->pointersAux->size(); i++) {
        pointer = this->pointersAux->at(i);
        
        if (pointer->x == _pointer->x &&
            pointer->y == _pointer->y &&
            pointer->z == _pointer->z) {
            this->pointersAux->erase(this->pointersAux->begin()+i);
            delete pointer;
            
            break;
        }
    }
    
//    this->pointers = 0;
//    this->pointers = new Pointers();
//    for (int i=0; i<this->pointersAux->size(); i++) {
//        this->pointers = MakePointers(this->pointers, this->pointersAux->at(i));
//    }
}

void SimulatedObject::addAllPointers(std::vector<Pointer *> * _pointers)
{
    for (int i=0; i<_pointers->size(); i++) {
        if (_pointers->at(i) == 0) {
            continue;
        }
        
//        this->pointers = MakePointers(this->pointers, _pointers->at(i));
//        this->pointersAux->push_back(CpyPointer(_pointers->at(i)));
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