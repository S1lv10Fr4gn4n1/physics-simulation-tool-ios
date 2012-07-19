//
//  Controller.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Controller.h"

Controller * Controller::controller;

Controller::Controller()
{
    mainEngine = nil;
    mainGraphic = nil;
}

Controller::~Controller()
{
    freeObjects();
}

Controller * Controller::getInstance()
{
    if (!controller) {
        controller = new Controller();
        controller->initializeEngine();
    }
    
    return controller;
}

void Controller::freeObjects()
{
    delete mainGraphic;
    delete mainEngine;

    mainEngine = nil;
    mainGraphic = nil;
}

void Controller::initializeContextOpenGLES()
{
    if (mainGraphic) {
        return;
    }
    
    //TODO revise, no objects 'no C++ ansi'
    NSString * vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString * fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    NSString * geomShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"gsh"];
    const char * vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char * fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];
    const char * geomShaderSource = [[NSString stringWithContentsOfFile:geomShaderPathname encoding:NSUTF8StringEncoding error:nil] UTF8String];

    mainGraphic = new MainGraphic();
    mainGraphic->initializeShader(vertShaderSource, fragShaderSource, geomShaderSource);
    mainGraphic->initializeNDC(1024, 768); //TODO - revise
}

void Controller::initializeEngine()
{
    if (mainEngine) {
        return;
    }
    
    mainEngine = new MainEngine();
    mainEngine->updateInformation();
    mainEngine->start();
}

void Controller::resizeScreen(float _width, float _height)
{
    mainEngine->rotatedScreen(_width, _height);
    mainGraphic->rotatedScreen(_width, _height);
}

void Controller::updateInformation()
{
    mainEngine->updateInformation();
}

void Controller::draw()
{
    mainGraphic->draw(mainEngine->getWorld());
}

void Controller::stopSimulation()
{
    mainEngine->stop();
}

void Controller::startSimulation()
{
    mainEngine->start();
}

bool Controller::isRunning()
{
    if (!mainEngine) {
        return false;
    }
    return mainEngine->isRunning();
}

bool Controller::isInitialized()
{
    if (mainEngine && mainGraphic && mainEngine->isRunning()) {
        return true;
    }
    
    return false;
}

void Controller::addSimulatedObjectInWorld(SimulatedObject * _simulatedObject)
{
    mainEngine->addSimulatedObjectInWorld(_simulatedObject);
}

void Controller::calcNDCCoordinates(float * _x, float * _y)
{
    mainGraphic->getNdc()->calcNDCCoordinates(_x, _y);   
}

SimulatedObject * Controller::selectedSimulatedObject(Pointer * _pointer)
{
    return mainEngine->selectedSimulatedObject(_pointer);
}

void Controller::createSimulatedObject(TypeObject typeObject)
{
    mainEngine->createSimulatedObject(typeObject);
}
