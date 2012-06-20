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
    simulatedObjects = new vector<SimulatedObject *>();
}

World::~World()
{
    if (simulatedObjects && simulatedObjects->size()>0) {
        for (int i=0; 0 < simulatedObjects->size(); i++) {
            delete simulatedObjects->at(i);
        }
    }
    
    delete simulatedObjects;
}

void World::addSimulatedObject(SimulatedObject * _simulatedObject)
{
    simulatedObjects->push_back(_simulatedObject);
}

void World::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    for (int i=0; 0 < simulatedObjects->size(); i++) {
        if (simulatedObjects->at(i) == _simulatedObject) {
            simulatedObjects->erase(simulatedObjects->begin()+i);
            return;
        }
    }
}

std::vector<SimulatedObject *> * World::getSimulatedObjects()
{
    return simulatedObjects;
}

