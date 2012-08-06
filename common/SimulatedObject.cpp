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
//    TODO
//    this->color = MakeColor(0, 0, 0, 0, 4);
    this->matrixTransformation = MatrixMakeIdentity();
    
    this->joinsSimulatedObject = 0; //TO-BE

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

void SimulatedObject::initBBox(const float * _matrix)
{
    Pointer * pointer = 0;
    Pointer * pointerAux = 0;
    
    for(int i=0; i < this->pointersAux->size(); i++) {
		pointerAux = this->pointersAux->at(i);
        
        pointer = MatrixTransformPoint(this->matrixTransformation, pointerAux);
		
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
        
        delete pointer; //TODO revise
	}
    
//    TODO
//    this->bbox->pointers = new Pointers();
//    this->bbox->pointers->p = new float[12];
//    this->bbox->pointers->count = 4;
//    *(this->bbox->pointers->p+0) = this->bbox->min->x;
//    *(this->bbox->pointers->p+1) = this->bbox->max->y;
//    *(this->bbox->pointers->p+2) = 0.0f;
//    *(this->bbox->pointers->p+3) = this->bbox->max->x;
//    *(this->bbox->pointers->p+4) = this->bbox->max->y;
//    *(this->bbox->pointers->p+5) = 0.0f;
//    *(this->bbox->pointers->p+6) = this->bbox->max->x;
//    *(this->bbox->pointers->p+7) = this->bbox->min->y;
//    *(this->bbox->pointers->p+8) = 0.0f;
//    *(this->bbox->pointers->p+9) = this->bbox->min->x;
//    *(this->bbox->pointers->p+10) = this->bbox->min->y;
//    *(this->bbox->pointers->p+11) = 0.0f;
}

void SimulatedObject::initialize()
{
    this->makePointers();
    this->initBBox(this->matrixTransformation);
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
    
    this->makePointers();
    this->initBBox(this->matrixTransformation);
}

void SimulatedObject::addAllPointers(std::vector<Pointer *> * _pointers)
{
    if (this->pointersAux && this->pointersAux->size()) {
        this->pointersAux->clear();
    }
    
    this->pointersAux = _pointers;
}

void SimulatedObject::makePointers()
{
    // initialize array of float for drawing
    if (this->pointers->p) {
        delete [] this->pointers->p;
    }
    
    this->pointers->p = new float[this->pointersAux->size()*COUNT_COORD];
    int index = 0;
    Pointer * pointer = 0;
    
    for (int i=0; i<this->pointersAux->size(); i++) {
        pointer = this->pointersAux->at(i);
        index = i*COUNT_COORD;
        *(this->pointers->p+index+0) = pointer->x;
        *(this->pointers->p+index+1) = pointer->y;
        *(this->pointers->p+index+2) = pointer->z;
    }
    
    this->pointers->count = this->pointersAux->size();
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