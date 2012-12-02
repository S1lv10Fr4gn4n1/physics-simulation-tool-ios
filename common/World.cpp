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
    this->camera = new Camera();
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

    if (this->camera) {
        delete this->camera;
    }

    this->camera = NULL;
    this->simulatedObjects = NULL;
}

//!SimulatedObject adds worldwide
void World::addSimulatedObject(SimulatedObject * _simulatedObject)
{
    this->simulatedObjects->push_back(_simulatedObject);
}

//!excludes the world SimulatedObject
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

//!deletes all the world SimulatedObject
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

Camera * World::getCamera()
{
    return this->camera;
}