//
//  MainEngine.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MainEngine.h"

MainEngine::MainEngine()
{
    world = new World();
}

MainEngine::~MainEngine()
{ 
    delete world;
}

void MainEngine::start()
{
    /// TODO - implementation
}

void MainEngine::updateInformation()
{
    /// TODO - implementation
}

void MainEngine::rotatedScreen(float _width, float _heigth)
{
    /// TODO - implementation
}

World * MainEngine::getWorld()
{
    return world;
}

void MainEngine::addSimulatedObjectInWorld(SimulatedObject * _simulatedObject)
{
    _simulatedObject->start();
    
    world->addSimulatedObject(_simulatedObject);
}

SimulatedObject * MainEngine::selectedSimulatedObject(Pointer * _pointer)
{
    return Selection::selectSimulatedObject(world, _pointer);
}


