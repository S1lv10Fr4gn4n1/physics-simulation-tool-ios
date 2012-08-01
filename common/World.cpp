//
//  World.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "World.h"

using namespace std;

World::World()
{
    this->simulatedObjects = new vector<SimulatedObject *>();
}

World::~World()
{
    if (this->simulatedObjects && this->simulatedObjects->size()>0) {
        SimulatedObject * simulatedObject = 0;
        
        for (int i=0; i < this->simulatedObjects->size(); i++) {
            simulatedObject = this->simulatedObjects->at(i);
            this->simulatedObjects->erase(this->simulatedObjects->begin()+i);
            delete simulatedObject;
        }
    }
    
    delete this->simulatedObjects;
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
            this->simulatedObjects->erase(this->simulatedObjects->begin()+i);
            return;
        }
    }
}

void World::deleteAllSimulatedObject()
{
    if (this->simulatedObjects && this->simulatedObjects->size()>0) {
        SimulatedObject * simulatedObject = 0;
        for (int i=0; i < this->simulatedObjects->size(); i++) {
            simulatedObject = this->simulatedObjects->at(i);
            delete simulatedObject;
        }
        this->simulatedObjects->clear();
    }
}

std::vector<SimulatedObject *> * World::getSimulatedObjects()
{
    return this->simulatedObjects;
}

