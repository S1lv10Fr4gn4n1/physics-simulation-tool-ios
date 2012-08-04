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
    this->objectEdition = 0;
    this->objectOffset = 0;
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
    
    if (this->objectEdition) {
        delete this->objectEdition;
    }

    if (this->objectOffset) {
        delete this->objectOffset;
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
    this->objectEdition = 0;
    this->objectOffset = 0;
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

void Controller::touchesBegan(Pointer * _pointer)
{
    // TODO
//    printf("touchesBegan\n");
}

void Controller::touchesEnded(Pointer * _pointer)
{
    // TODO
//    printf("touchesEnded\n");
}

void Controller::touchesCancelled(Pointer * _pointer)
{
    // TODO
//    printf("TouchesCancelled\n");
}

void Controller::touchesMoved(Pointer * _pointer)
{
    printf("touchesMoved\n");
    this->mainEngine->calcNDCCoordinates(&_pointer->x, &_pointer->y);
    this->objectOffset = this->mainEngine->selectedSimulatedObject(_pointer);;

    if (this->objectOffset) {
        if (this->objectEdition) {
            this->objectEdition->setSelected(false);
            this->objectEdition = 0;
        }
        MatrixTranslate(this->objectOffset->getMatrixTransformation(), _pointer);
    }
}

void Controller::pinchDetected(float scale, float velocity)
{
    printf("pinch-> scale: %f, velocity: %f\n", scale, velocity);
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->objectEdition->setMatrixTransformation(MatrixMultiplay(this->objectEdition->getMatrixTransformation(), MatrixMakeScale(scale*100, scale*100)));
    }
}

void Controller::rotationDetected(float radians, float velocity)
{
    printf("rotation-> radians: %f, velocity: %f\n", radians, velocity);
    if (this->objectEdition && this->objectEdition->isSelected()) {
        float teta = (M_PI * radians * 10) / 180.0;
        this->objectEdition->setMatrixTransformation(MatrixMultiplay(this->objectEdition->getMatrixTransformation(), MatrixMakeZRotation(teta)));
    }
}

void Controller::doubleTapOneFingerDetected(Pointer * _pointer)
{
    printf("doubleTapOnFingerDetected\n");
    this->mainEngine->calcNDCCoordinates(&_pointer->x, &_pointer->y);
    this->objectEdition = this->mainEngine->selectedSimulatedObject(_pointer);
    
    if (this->objectEdition) {
        if (this->objectEdition->isSelected()) {
            this->objectEdition->setSelected(false);
            this->objectEdition = 0;
        } else {
            this->objectEdition->setSelected(true);
        }
    }
}

void Controller::longPressDetected(Pointer * _pointer)
{
    // TODO
//    printf("longPressDetected\n");
}

void Controller::swipeRightDetected(Pointer * _pointer)
{
    // TODO
//    printf("swipeRightDetected\n");
}
void Controller::swipeLeftDetected(Pointer * _pointer)
{
    // TODO
//    printf("swipeLeftDetected\n");
}

void Controller::createSimulatedObject(TypeObject typeObject)
{    
//    if (this->objectEdition) {
//        this->objectEdition->setSelected(false);
//    }
    
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
    
    this->objectEdition = object;
    this->objectEdition->setSelected(true);
}