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
    world->addSimulatedObject(_simulatedObject);
}

void MainEngine::start()
{
    /// TODO - implementation
}

