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
    this->mainEngine = 0;
    this->mainGraphic = 0;
    this->currentObject = 0;
}

Controller::~Controller()
{
    this->freeObjects();
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
    if (this->mainGraphic) {
        delete this->mainGraphic;
    }
    
    if (this->mainEngine) {
        delete this->mainEngine;
    }
    
    if (this->currentObject) {
        delete this->currentObject;
    }
}

void Controller::initializeContextOpenGLES()
{
    if (this->mainGraphic) {
        return;
    }
    
    //TODO revise, no objects 'no C++ ansi'
    NSString * vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString * fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    NSString * geomShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"gsh"];
    const char * vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];
    const char * fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];
    const char * geomShaderSource = [[NSString stringWithContentsOfFile:geomShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];

    this->mainGraphic = new MainGraphic();
    this->mainGraphic->initializeShader(vertShaderSource, fragShaderSource, geomShaderSource);
}

void Controller::initializeEngine()
{
    if (this->mainEngine) {
        return;
    }
    
    this->mainEngine = new MainEngine();
    this->mainEngine->updateInformation();
    this->mainEngine->rotatedScreen(1024, 768);
    this->mainEngine->start();
}

void Controller::resizeScreen(float _width, float _height)
{
    this->mainEngine->rotatedScreen(_width, _height);
}

void Controller::updateInformation()
{
    this->mainEngine->updateInformation();
}

void Controller::clearSimularion()
{
    this->mainEngine->deleteAllSimulatedObjects();
    currentObject = 0;
}

void Controller::draw()
{
    this->mainGraphic->draw(this->mainEngine->getWorld());
}

void Controller::stopSimulation()
{
    this->mainEngine->stop();
}

void Controller::startSimulation()
{
    this->mainEngine->start();
}

bool Controller::isRunning()
{
    if (!this->mainEngine) {
        return false;
    }
    
    return this->mainEngine->isRunning();
}

bool Controller::isInitialized()
{
    if (this->mainEngine && this->mainGraphic && this->mainEngine->isRunning()) {
        return true;
    }
    
    return false;
}

void Controller::addSimulatedObjectInWorld(SimulatedObject * _simulatedObject)
{
    this->mainEngine->addSimulatedObjectInWorld(_simulatedObject);
}

void Controller::calcNDCCoordinates(float * _x, float * _y)
{
    this->mainEngine->calcNDCCoordinates(_x, _y);
}

void Controller::selectedSimulatedObject(Pointer * _pointer)
{
    this->calcNDCCoordinates(&_pointer->x, &_pointer->y);
    this->currentObject = this->mainEngine->selectedSimulatedObject(_pointer);
    
    if (this->currentObject) {
        this->currentObject->setSelected(!this->currentObject->isSelected());
    }
}

void Controller::touchesMoved(Pointer * _pointer)
{
    this->calcNDCCoordinates(&_pointer->x, &_pointer->y);
    this->currentObject = this->mainEngine->selectedSimulatedObject(_pointer);
    
    if (this->currentObject && this->currentObject->isSelected()) {
        MatrixTranslate(this->currentObject->getMatrixTransformation(), _pointer);
    }
}

void Controller::pinchDetected(float scale, float velocity)
{
    if (this->currentObject && this->currentObject->isSelected()) {
        //TODO - implement zoom
    }
}

void Controller::rotationDetected(float radians, float velocity)
{
    if (this->currentObject /*&& this->currentObject->isSelected()*/) {
        //TODO - implement rotation object
        
        static float i = 0;
        
        float teta = (M_PI * i * 10) / 180.0;
        this->currentObject->setMatrixTransformation(MatrixMakeZRotation(teta));

        i += 0.1;
    }
}

void Controller::createSimulatedObject(TypeObject typeObject)
{    
    if (this->currentObject) {
        this->currentObject->setSelected(false);
    }
    
    SimulatedObject * object = new SimulatedObject();
    object->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));
    object->setMatrixTransformation(MatrixMakeIdentity());
    object->setMode(GL_LINE_LOOP);
    
    switch (typeObject) {
        case CIRCLE:
        {
            object->setColor(MakeColor(255, 255, 255, 255, 36));
            
            // calculates the radius
            // takes the first point, which indicates the origin of the circle
            Pointer * p1 = MakePointer( 0.000000, 0.000000, 0.000000);
            Pointer * p2 = MakePointer( 0.000000, 0.052083, 0.000000);
            
            float x = p2->x - p1->x;
            float y = p2->y - p1->y;
            
            /// d²=(x0-x)²+(y0-y)²
            float d = (x*x) + (y*y);
            float radius = pow(d, 0.5);
            
            float x1;
            float y1;
            
            /// generates points to create the circle, these points are stored
            /// to be subsequently used in the algorithm scanline
            int ang = 0;
            for (int i=0; i<36; i++) {
                x1 = (radius * cos(M_PI * ang / 180.0f));
                y1 = (radius * sin(M_PI * ang / 180.0f));
                
                object->addPointer(MakePointer(x1 + p1->x, y1 + p1->y, 0.0));
                ang += 10;
            }
            
            break;
        }   
            
        case SQUARE:
        {
            object->setColor(MakeColor(255, 255, 255, 255, 4));
            object->addPointer(MakePointer( -0.039062, -0.052083, 0.000000));
            object->addPointer(MakePointer(  0.039062, -0.052083, 0.000000));
            object->addPointer(MakePointer(  0.039062,  0.052083, 0.000000));
            object->addPointer(MakePointer( -0.039062,  0.052083, 0.000000));         
            break;
        }
            
        case TRIANGLE:
        {
            object->setColor(MakeColor(255, 255, 255, 255, 3));
            object->addPointer(MakePointer(  0.000000,  0.052083, 0.000000));
            object->addPointer(MakePointer( -0.039062, -0.052083, 0.000000));
            object->addPointer(MakePointer(  0.039062, -0.052083, 0.000000));
            break;
        }
            
        case ENGINE:
            break;
            
        case STRING:
            break;
        
        case SPRINGS:
            break;
        
        case POLYGON_OPEN:
            break;
        
        case POLYGON_CLOSE:
            break;
            
        default:
            break;            
    }
    
    this->mainEngine->addSimulatedObjectInWorld(object);
    
    this->currentObject = object;
    this->currentObject->setSelected(true);
}