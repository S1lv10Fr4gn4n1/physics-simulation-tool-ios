//
//  Controller.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "Controller.h"

Controller * Controller::controller;

// TODO revise: variables for this place is correct?
float scaleObject = 1.0f;
float scaleZoom = 1.0f;
//float scalePanX = 0.0f;
//float scalePanY = 0.0f;
float previousRadians = 0.0f;
float previousZoom = 0.0f;
float previousScale = 0.0f;
//Pointer * previousPointer = NULL;

Controller::Controller()
{
    this->mainEngine = NULL;
    this->mainGraphic = NULL;
    this->objectEdition = NULL;
    this->objectOffset = NULL;
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
        this->mainGraphic = NULL;
    }
    
    if (this->mainEngine) {
        delete this->mainEngine;
        this->mainEngine = NULL;
    }
    
    if (this->objectEdition) {
        delete this->objectEdition;
        this->objectEdition = NULL;
    }

    if (this->objectOffset) {
        delete this->objectOffset;
        this->objectOffset = NULL;
    }
}

void Controller::initializeContextOpenGLES()
{
    if (this->mainGraphic) {
        return;
    }
    
    // TODO revise, no objects 'no C++ ansi'
    NSString * vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    NSString * fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    const char * vertShaderSource = [[NSString stringWithContentsOfFile:vertShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];
    const char * fragShaderSource = [[NSString stringWithContentsOfFile:fragShaderPathname encoding:NSUTF8StringEncoding error:0] UTF8String];

    this->mainGraphic = new MainGraphic();
    this->mainGraphic->initializeShader(vertShaderSource, fragShaderSource);
}

void Controller::initializeEngine()
{
    if (this->mainEngine) {
        return;
    }
    
    this->mainEngine = new MainEngine();
    this->mainEngine->updateInformation();
//    TODO revise: identify device and inform value correctly of screen dimension
//    this->mainEngine->rotatedScreen(1024, 768);
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
    this->objectEdition = NULL;
    this->objectOffset = NULL;
    this->objectEdition = NULL;
    this->objectOffset = NULL;
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

void Controller::touchesBegan(float _x, float _y)
{
    // TODO put your code here
}

void Controller::touchesEnded(float _x, float _y)
{
    // TODO put your code here
}

void Controller::touchesCancelled(float _x, float _y)
{
    // TODO put your code here
}

void Controller::touchesMoved(float _x, float _y, int countFingers)
{
    Pointer * pointer = MakePointer(_x, _y);

    // move object
    if (countFingers == 1) {
        this->objectOffset = this->mainEngine->selectedSimulatedObject(pointer);
        
        if (this->objectOffset && !this->objectOffset->isImmovable()) {
            if (this->objectEdition) {
                this->objectEdition->setSelected(false);
                this->objectEdition = NULL;
            }
            
            this->mainEngine->translateSimulatedObject(this->objectOffset, pointer);
        }
    }
    
//    TODO revise: pan
//    move scene
//    if (countFingers == 2) {
//        if (previousPointer) {
//            if (previousPointer->x > pointer->x) {
//                scalePanX -= 0.009;
//            } else if (previousPointer->x < pointer->x) {
//                scalePanX += 0.009;
//            }
//
//            if (previousPointer->y > pointer->y) {
//                scalePanY += 0.009;
//            } else if (previousPointer->y < pointer->y) {
//                scalePanY -= 0.009;
//            }
//
//            this->mainEngine->pan(scalePanX, scalePanY);
//
//            delete previousPointer;
//            previousPointer = NULL;
//        }
//        
//        previousPointer = MakePointer(pointer);
//    }
    
    delete pointer;
    pointer = NULL;
}

void Controller::pinchDetected(float _scale, float _velocity, bool began)
{
    if (this->objectEdition && this->objectEdition->isSelected() && !this->objectEdition->isImmovable()) {
        if (began) {
            scaleObject = 1.0f;
            previousScale = 0.0;
        }
         
        if ((_scale - previousScale) < 0.0f) {
            scaleObject-=0.0005f;
        } else {
            scaleObject+=0.0005f;
        }
        this->mainEngine->scaleSimulatedObject(this->objectEdition, scaleObject);
        previousScale = _scale;
    } else {
        if ((_scale - previousZoom) < 0.0f) {
            scaleZoom+=0.03f;
        } else {
            scaleZoom-=0.03f;
        }
        // limit zoom in
        if (scaleZoom >= 3.0f){
            scaleZoom = 3.0f;
            return;
        }
        // limit zoom out
        if (scaleZoom <= 0.1f) {
            scaleZoom = 0.1f;
            return;
        }
        this->mainEngine->zoom(scaleZoom);
        previousZoom = _scale;
    }
}

void Controller::rotationDetected(float _radians, float _velocity, bool began)
{
    if (began) {
        previousRadians = 0.0f;
    }
  
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->rotateSimulatedObject(this->objectEdition, previousRadians < _radians ? -0.03 : 0.03);
    }
    previousRadians = _radians;
}

void Controller::doubleTapOneFingerDetected(float _x, float _y)
{
    Pointer * pointer = MakePointer(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(pointer);
    
    if (this->objectEdition) {
        if (this->objectEdition->isSelected()) {
            this->objectEdition->setSelected(false);
            this->objectEdition = NULL;
        } else {
            this->objectEdition->setSelected(true);
        }
    }
    
    delete pointer;
    pointer = NULL;
}

void Controller::longPressDetected(float _x, float _y)
{
    Pointer * pointer = MakePointer(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(pointer);

    if (this->objectEdition) {
        this->mainEngine->deleteSimulatedObject(this->objectEdition);
    }
}

void Controller::swipeRightDetected(float _x, float _y)
{
    // TODO put your code here
}
void Controller::swipeLeftDetected(float _x, float _y)
{
    // TODO put your code here
}

void Controller::oneTapThreeFingerDetected(float _x, float _y)
{
//    delete previousPointer;
//    previousPointer = NULL;
    
    scaleZoom = 1.0f;
    this->mainEngine->zoom(scaleZoom);
}

void Controller::createSimulatedObject(TypeObject typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }
    
    SimulatedObject * object = new SimulatedObject();
    object->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));
    object->setColorAux(MakeRandonColor());
    object->setMode(GL_TRIANGLE_FAN);
    
    this->mainEngine->makeSimulatedObject(object, typeObject);
}