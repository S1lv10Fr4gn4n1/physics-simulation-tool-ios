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
    // TODO - do something
}

Controller::~Controller()
{
    // TODO - do something 
}

Controller * Controller::getInstance()
{
    if (!controller) {
        controller = new Controller();
    }
    
    return controller;
}

void Controller::freeObjects()
{
    delete mainGraphic;
    delete mainEngine;
}

void Controller::initializeSimulator()
{
    initializeLibraryGraphic();
    initializeLibraryEngine();
    initializeFrame();
}

void Controller::initializeLibraryGraphic()
{
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

void Controller::initializeLibraryEngine()
{
    mainEngine = new MainEngine();
    mainEngine->updateInformation();
    mainEngine->start();
}

void Controller::initializeFrame()
{
    SimulatedObject * buttonStartEditor = new SimulatedObject();
    buttonStartEditor->setMode(GL_TRIANGLE_FAN);
    buttonStartEditor->setColor(MakeColor(0, 0, 0, 0, 4));
    buttonStartEditor->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));
    buttonStartEditor->addPointer(MakePointer(-0.960938, -0.843750, 0.0));
    buttonStartEditor->addPointer(MakePointer(-0.882812, -0.843750, 0.0));
    buttonStartEditor->addPointer(MakePointer(-0.882812, -0.947917, 0.0));
    buttonStartEditor->addPointer(MakePointer(-0.960938, -0.947917, 0.0));
    
    mainEngine->addSimulatedObjectInWorld(buttonStartEditor);
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
