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
    
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
}

void MainEngine::zoom(float _scale)
{
    float value = this->ndc->getAspect() * _scale;
    
    this->ndc->setLeft(-value);
    this->ndc->setRight(value);
    this->ndc->setBottom(-_scale);
    this->ndc->setTop(_scale);

    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
}

void MainEngine::pan(Pointer * _pointer)
{
    // TODO
    this->ndc->setLeft(-this->ndc->getAspect() + _pointer->x);
    this->ndc->setRight(this->ndc->getAspect() + _pointer->x);
    this->ndc->setBottom(-1 + _pointer->y);
    this->ndc->setTop(1 + _pointer->y);
    
    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
}

void MainEngine::centralizedWorld()
{
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
}

void MainEngine::scaleSimulatedObject(SimulatedObject * _simulatedObject, float _scale)
{
    _simulatedObject->setMatrixTransformation(MatrixMultiply(_simulatedObject->getMatrixTransformation(), MatrixMakeScale(_scale, _scale)));
}

void MainEngine::rotateSimulatedObject(SimulatedObject * _simulatedObject, float _radians)
{
    float teta = -(M_PI * _radians) / 180.0;
    _simulatedObject->setMatrixTransformation(MatrixMultiply(_simulatedObject->getMatrixTransformation(), MatrixMakeZRotation(teta)));
}

void MainEngine::translateSimulatedObject(SimulatedObject * _simulatedObject, Pointer * _pointer)
{
    MatrixTranslate(_simulatedObject->getMatrixTransformation(), _pointer);
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
    this->ndc->calcNDCCoordinates(&_pointer->x, &_pointer->y);
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