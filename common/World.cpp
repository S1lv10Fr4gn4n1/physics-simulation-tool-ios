//
//  World.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  
//

#include "World.h"

using namespace std;

World::World()
{
    this->simulatedObjects = new vector<SimulatedObject *>();

    this->orthoMatrix = new real[16];
    MatrixTransformIdentity(&this->orthoMatrix);
}

World::~World()
{
    if (this->simulatedObjects && this->simulatedObjects->size()>0) {
        SimulatedObject * simulatedObject = NULL;
        
        for (int i=0; i < this->simulatedObjects->size(); i++) {
            simulatedObject = this->simulatedObjects->at(i);
            this->simulatedObjects->erase(this->simulatedObjects->begin()+i);
            delete simulatedObject;
            simulatedObject = NULL;
        }
        
        delete this->simulatedObjects;
    }
    
    if (this->orthoMatrix) {
        delete [] this->orthoMatrix;
    }
    
    this->simulatedObjects = NULL;
    this->orthoMatrix = NULL;
}

void World::addSimulatedObject(SimulatedObject * _simulatedObject)
{
    this->simulatedObjects->push_back(_simulatedObject);
}

void World::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    for (int i=0; i < this->simulatedObjects->size(); i++) {
        if (this->simulatedObjects->at(i) == _simulatedObject) {
            delete _simulatedObject;
            _simulatedObject = NULL;
            this->simulatedObjects->erase(this->simulatedObjects->begin()+i);
            return;
        }
    }
}

void World::deleteAllSimulatedObject()
{
    if (this->simulatedObjects && this->simulatedObjects->size()>0) {
        SimulatedObject * simulatedObject = NULL;
        for (int i=0; i < this->simulatedObjects->size(); i++) {
            simulatedObject = this->simulatedObjects->at(i);
            delete simulatedObject;
            simulatedObject = NULL;
        }
        this->simulatedObjects->clear();
    }
}

std::vector<SimulatedObject *> * World::getSimulatedObjects()
{
    return this->simulatedObjects;
}

void World::setOrthoMatrix(real * _matrix)
{
    this->orthoMatrix = _matrix;
}

real * World::getOrthoMatrix()
{
    return this->orthoMatrix;
}
