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
    
    this->vectorsAux = new vector<Vector3 *>();
    this->vectors = NULL;
    
    this->colorAux = NULL;
    this->color = NULL;
    this->colorVectors = NULL;
    
    this->matrixTransformation = new real[16];
    MatrixTransformIdentity(&this->matrixTransformation);
    
    this->bbox = new BBox();
    this->bbox->max->x = this->bbox->max->y = this->bbox->max->z = -1000000;
	this->bbox->min->x = this->bbox->min->y = this->bbox->min->z =  1000000;
    
    // Rigid-body
    this->setMass(1.0f);
}

SimulatedObject::~SimulatedObject()
{
    if (this->vectorsAux && this->vectorsAux->size() > 0) {
        Vector3 * vector = NULL;
        for (int i=0; i < this->vectorsAux->size(); i++) {
            vector = this->vectorsAux->at(i);
            delete vector;
            vector = NULL;
        }
        this->vectorsAux->clear();
    }
    
    if (this->vectorsAux) {
        delete this->vectorsAux;
    }
    if (this->vectors) {
        delete [] this->vectors;
    }
    if (this->color) {
        delete [] this->color;
    }
    if (this->colorVectors) {
        delete [] this->colorVectors;
    }
    if (this->colorAux) {
        delete this->colorAux;
    }
    if (this->bbox) {
        delete this->bbox;
    }
    if (this->matrixTransformation) {
        delete this->matrixTransformation;
    }
    
    this->vectorsAux = NULL;
    this->vectors = NULL;
    this->color = NULL;
    this->colorVectors = NULL;
    this->colorAux = NULL;
    this->bbox = NULL;
    this->matrixTransformation = NULL;
}

void SimulatedObject::initBBox(real * _matrix)
{
    Vector3 * vector = NULL;
    Vector3 * vectorAux = NULL;
    
    for(int i=0; i < this->vectorsAux->size(); i++) {
		vectorAux = this->vectorsAux->at(i);
        
        vector = MatrixTransformPoint(this->matrixTransformation, vectorAux);
		
		//definindo o maior X
		if (vector->x > this->bbox->max->x) {
			this->bbox->max->x = vector->x;
		}
		
		//definindo o menor X
		if (vector->x < this->bbox->min->x) {
			this->bbox->min->x = vector->x;
		}
		
		//definindo o maior Y
		if (vector->y > this->bbox->max->y) {
			this->bbox->max->y = vector->y;
		}
		
		//definindo o menor Y
		if (vector->y < this->bbox->min->y) {
			this->bbox->min->y = vector->y;
		}
        
		//definindo o maior Z
		if (vector->z > this->bbox->max->z) {
			this->bbox->max->z = vector->z;
		}
		
		//definindo o menor Z
		if (vector->z < this->bbox->min->z) {
			this->bbox->min->z = vector->z;
		}
        
        delete vector;
        vector = NULL;
	}
    
    this->makeBBox();
}

void SimulatedObject::initialize()
{
    this->makeVectors();
    this->makeColorObject();
    this->makeColorVectors();
    this->initBBox(this->matrixTransformation);
}

void SimulatedObject::addVector3(Vector3 * Vector3)
{
    this->vectorsAux->push_back(Vector3);
}

void SimulatedObject::deleteVector3(Vector3 * _vector)
{
    Vector3 * vector = NULL;
    
    for (int i=0; 0<this->vectorsAux->size(); i++) {
        vector = this->vectorsAux->at(i);
        
        if (vector->x == _vector->x &&
            vector->y == _vector->y &&
            vector->z == _vector->z) {
            delete vector;
            vector = NULL;
            this->vectorsAux->erase(this->vectorsAux->begin()+i);
            
            break;
        }
    }
    
    this->makeVectors();
    this->initBBox(this->matrixTransformation);
}

void SimulatedObject::addAllVectors(std::vector<Vector3 *> * _vectors)
{
    if (this->vectorsAux && this->vectorsAux->size()) {
        this->vectorsAux->clear();
    }
    
    this->vectorsAux = _vectors;
}

void SimulatedObject::makeVectors()
{
    // initialize array of real for drawing
    if (this->vectors) {
        delete [] this->vectors;
        this->vectors = NULL;
    }
    
    this->vectors = new real[this->vectorsAux->size()*COUNT_COORD];
    int index = 0;
    Vector3 * vector = NULL;
    
    for (int i=0; i<this->vectorsAux->size(); i++) {
        vector = this->vectorsAux->at(i);
        index = i*COUNT_COORD;
        *(this->vectors+index+0) = vector->x;
        *(this->vectors+index+1) = vector->y;
        *(this->vectors+index+2) = vector->z;
    }
}

void SimulatedObject::makeColorObject()
{
    int total = this->vectorsAux->size()*CHANNEL_COLOR;
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

void SimulatedObject::makeColorVectors()
{
    int total = this->vectorsAux->size()*CHANNEL_COLOR;
    this->colorVectors = new unsigned char[total];
    
    for (int i = 0; i < total; i += CHANNEL_COLOR) {
		this->colorVectors[i]   = 0;
		this->colorVectors[i+1] = 0;
		this->colorVectors[i+2] = 0;
        
        if (CHANNEL_COLOR == 4) {
            this->colorVectors[i+3] = 1;
        }
	}
}

void SimulatedObject::makeBBox()
{
    if (this->bbox->ptr) {
        delete [] this->bbox->ptr;
        this->bbox->ptr = NULL;
    }
    this->bbox->ptr = new real[12];
    *(this->bbox->ptr+0) = this->bbox->min->x;
    *(this->bbox->ptr+1) = this->bbox->max->y;
    *(this->bbox->ptr+2) = 0.0f;
    *(this->bbox->ptr+3) = this->bbox->max->x;
    *(this->bbox->ptr+4) = this->bbox->max->y;
    *(this->bbox->ptr+5) = 0.0f;
    *(this->bbox->ptr+6) = this->bbox->max->x;
    *(this->bbox->ptr+7) = this->bbox->min->y;
    *(this->bbox->ptr+8) = 0.0f;
    *(this->bbox->ptr+9) = this->bbox->min->x;
    *(this->bbox->ptr+10) = this->bbox->min->y;
    *(this->bbox->ptr+11) = 0.0f;
}

real * SimulatedObject::getVectors()
{
    return this->vectors;
}

std::vector<Vector3 *> * SimulatedObject::getVectorsAux()
{
    return this->vectorsAux;
}

unsigned char * SimulatedObject::getColor()
{
    return this->color;
}

unsigned char * SimulatedObject::getColorVectors()
{
    return this->colorVectors;
}

void SimulatedObject::setColorAux(Color * _color)
{
    this->colorAux = _color;
}

Color * SimulatedObject::getColorAux()
{
    return this->colorAux;
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

real * SimulatedObject::getMatrixTransformation()
{
    return this->matrixTransformation;
}

void SimulatedObject::setMatrixTransformation(real * _matrix)
{
    delete this->matrixTransformation;
    this->matrixTransformation = NULL;
    
    this->matrixTransformation = _matrix;
}