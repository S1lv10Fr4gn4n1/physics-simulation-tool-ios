//
//  MainEngine.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MainEngine.h"

using namespace std;

MainEngine::MainEngine()
{
    this->world = new World();
    this->ndc = new NDC();
}

MainEngine::~MainEngine()
{ 
    delete this->world;
    delete this->ndc;
}

void MainEngine::start()
{
    this->running = true;
}

void MainEngine::stop()
{
    this->running = false;
}

bool MainEngine::isRunning()
{
    return this->running;
}

void MainEngine::updateInformation()
{
    if (!this->running) {
        return;
    }

    /// TODO - implementation
}

void MainEngine::rotatedScreen(float _width, float _height)
{
    this->ndc->update(_width, _height);
}

void MainEngine::calcNDCCoordinates(float * _x, float * _y)
{
    this->ndc->calcNDCCoordinates(_x, _y);
}

World * MainEngine::getWorld()
{
    return this->world;
}

void MainEngine::addSimulatedObjectInWorld(SimulatedObject * _simulatedObject)
{
    _simulatedObject->initialize();    
    
    this->world->addSimulatedObject(_simulatedObject);
}

SimulatedObject * MainEngine::selectedSimulatedObject(Pointer * _pointer)
{
    return Selection::selectSimulatedObject(this->world, _pointer);
}

SimulatedObject * MainEngine::selectedBBoxSimulatedObject(Pointer * _pointer)
{
    return Selection::selectBBoxSimulatedObject(this->world, _pointer);
}

void MainEngine::deleteAllSimulatedObjects()
{
    this->world->deleteAllSimulatedObject();
}

void MainEngine::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    this->world->deleteSimulatedObject(_simulatedObject);
}