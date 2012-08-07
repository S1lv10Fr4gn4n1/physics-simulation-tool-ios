//
//  SimulatedObject.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  
//

#include "SimulatedObject.h"

using namespace std;

SimulatedObject::SimulatedObject()
{
    this->showBBox = false;
    this->selected = false;
    
    this->pointersAux = new vector<Pointer *>();
    this->pointers = NULL;
    
    this->colorAux = NULL;
    this->color = NULL;
    
    this->physicalFeature = NULL;

    this->matrixTransformation = new float[16];
    MatrixTransformIdentity(&this->matrixTransformation);
    
    this->joinsSimulatedObject = NULL;

    this->bbox = new BBox();
    this->bbox->max->x = this->bbox->max->y = this->bbox->max->z = -1000000;
	this->bbox->min->x = this->bbox->min->y = this->bbox->min->z =  1000000;	
}

SimulatedObject::~SimulatedObject()
{
    if (this->pointersAux && this->pointersAux->size() > 0) {
        Pointer * pointer = NULL;
        for (int i=0; i < this->pointersAux->size(); i++) {
            pointer = this->pointersAux->at(i);
            delete pointer;
            pointer = NULL;
        }
        this->pointersAux->clear();
    }
    
    if (this->joinsSimulatedObject && this->joinsSimulatedObject->size() > 0) {
        SimulatedObject * simulatedObject = NULL;
        for (int i=0; i < this->joinsSimulatedObject->size(); i++) {
            simulatedObject = this->joinsSimulatedObject->at(i);
            delete simulatedObject;
            simulatedObject = NULL;
        }
        this->joinsSimulatedObject->clear();
    }
        
    delete this->pointersAux;
    delete [] this->pointers;
    delete this->physicalFeature;
    delete [] this->color;
    delete this->colorAux;
    delete this->bbox;
    delete this->matrixTransformation;
    
    this->pointersAux = NULL;
    this->pointers = NULL;
    this->physicalFeature = NULL;
    this->color = NULL;
    this->colorAux = NULL;
    this->bbox = NULL;
    this->matrixTransformation = NULL;

}

void SimulatedObject::initBBox(float * _matrix)
{
    Pointer * pointer = NULL;
    Pointer * pointerAux = NULL;
    
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
        pointer = NULL;
	}
    
    this->makeBBox();
}

void SimulatedObject::initialize()
{
    this->makePointers();
    this->makeColor();
    this->initBBox(this->matrixTransformation);
}

void SimulatedObject::addPointer(Pointer * _pointer)
{
    this->pointersAux->push_back(_pointer);
}

void SimulatedObject::deletePointer(Pointer * _pointer)
{
    Pointer * pointer = NULL;
    
    for (int i=0; 0<this->pointersAux->size(); i++) {
        pointer = this->pointersAux->at(i);
        
        if (pointer->x == _pointer->x &&
            pointer->y == _pointer->y &&
            pointer->z == _pointer->z) {
            delete pointer;
            pointer = NULL;
            this->pointersAux->erase(this->pointersAux->begin()+i);
            
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
    if (this->pointers) {
        delete [] this->pointers;
        this->pointers = NULL;
    }
    
    this->pointers = new float[this->pointersAux->size()*COUNT_COORD];
    int index = 0;
    Pointer * pointer = NULL;
    
    for (int i=0; i<this->pointersAux->size(); i++) {
        pointer = this->pointersAux->at(i);
        index = i*COUNT_COORD;
        *(this->pointers+index+0) = pointer->x;
        *(this->pointers+index+1) = pointer->y;
        *(this->pointers+index+2) = pointer->z;
    }
}

void SimulatedObject::makeColor()
{
    int total = this->pointersAux->size()*CHANNEL_COLOR;
    this->color = new unsigned char[total];
    
    for (int i = 0; i < total; i += CHANNEL_COLOR) {
		this->color[i]   = this->colorAux->r;
		this->color[i+1] = this->colorAux->g;
		this->color[i+2] = this->colorAux->b;
        
        if (CHANNEL_COLOR == 4) {
            this->color[i+3] = this->colorAux->a;
        }
	}
}

void SimulatedObject::makeBBox()
{
    if (this->bbox->p) {
        delete [] this->bbox->p;
        this->bbox->p = NULL;
    }
    this->bbox->p = new float[12];
    *(this->bbox->p+0) = this->bbox->min->x;
    *(this->bbox->p+1) = this->bbox->max->y;
    *(this->bbox->p+2) = 0.0f;
    *(this->bbox->p+3) = this->bbox->max->x;
    *(this->bbox->p+4) = this->bbox->max->y;
    *(this->bbox->p+5) = 0.0f;
    *(this->bbox->p+6) = this->bbox->max->x;
    *(this->bbox->p+7) = this->bbox->min->y;
    *(this->bbox->p+8) = 0.0f;
    *(this->bbox->p+9) = this->bbox->min->x;
    *(this->bbox->p+10) = this->bbox->min->y;
    *(this->bbox->p+11) = 0.0f;

}

float * SimulatedObject::getPointers()
{
    return this->pointers;
}

std::vector<Pointer *> * SimulatedObject::getPointersAux()
{
    return this->pointersAux;
}

unsigned char * SimulatedObject::getColor()
{
    return this->color;
}

void SimulatedObject::setColorAux(Color * _color)
{
    this->colorAux = _color;
}

Color * SimulatedObject::getColorAux()
{
    return this->colorAux;
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
    delete this->matrixTransformation;
    this->matrixTransformation = NULL;
    
    this->matrixTransformation = _matrix;
}