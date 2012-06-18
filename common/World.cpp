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
    sObjects = new vector<SObject *>();
}

World::~World()
{
    if (sObjects && sObjects->size()>0) {
        for (int i=0; 0 < sObjects->size(); i++) {
            delete sObjects->at(i);
        }
    }
    
    delete sObjects;
}

void World::addObject(SObject * _sObject)
{
    sObjects->push_back(_sObject);
}

void World::deleteObject(SObject * _sObject)
{
    for (int i=0; 0 < sObjects->size(); i++) {
        if (sObjects->at(i) == _sObject) {
            sObjects->erase(sObjects->begin()+i);
            return;
        }
    }
}

std::vector<SObject *> * World::getSObjects()
{
    return sObjects;
}

