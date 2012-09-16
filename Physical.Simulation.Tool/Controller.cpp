//
//  Controller.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "Controller.h"


// TODO revise: variables for this place is correct?
real scaleObject = 1.0f;
real scaleZoom = ZOOM_INIT;
real scalePanX = 5.0f;
real scalePanY = 2.0f;
real previousRadians = 0.0f;
real previousZoom = 0.0f;
real previousScale = 0.0f;
Vector3 * previousVector = NULL;

Controller * Controller::controller;
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
    if (!Controller::controller) {
        Controller::controller = new Controller();
        Controller::controller->initializeEngine();
    }
    
    return Controller::controller;
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

    this->mainGraphic = NULL;
    this->mainEngine = NULL;
    this->objectEdition = NULL;
    this->objectOffset = NULL;
}

void Controller::initializeContextOpenGLES()
{
    if (this->mainGraphic) {
        return;
    }
    
    this->mainGraphic = new MainGraphic();
}

void Controller::initializeEngine()
{
    if (this->mainEngine) {
        return;
    }
    
    this->mainEngine = new MainEngine();
}

void Controller::resizeScreen(real _width, real _height)
{
    this->mainEngine->rotatedScreen(_width, _height);
}

void Controller::updateInformation(real _duration)
{
    this->mainEngine->updateInformation(_duration);
}

void Controller::draw()
{
    this->mainGraphic->draw(this->mainEngine->getWorld());
}

void Controller::stopSimulation()
{
    this->mainEngine->stop();
    scaleZoom = scaleZoom;
    this->mainEngine->zoom(scaleZoom);
}

void Controller::startSimulation()
{
    this->mainEngine->start();
}

void Controller::editSimulation()
{
    // TODO put your code here
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

void Controller::touchesBegan(real _x, real _y)
{
    // TODO put your code here
}

void Controller::touchesEnded(real _x, real _y)
{
    // TODO put your code here
}

void Controller::touchesCancelled(real _x, real _y)
{
    // TODO put your code here
}

void Controller::touchesMoved(real _x, real _y, int _countFingers)
{
    Vector3 * vector = new Vector3(_x, _y);

    // move object
    if (_countFingers == 1) {
        this->objectOffset = this->mainEngine->selectedSimulatedObject(vector);
        
        if (this->objectOffset && this->objectOffset->hasFiniteMass()) {
            if (this->objectEdition) {
                this->objectEdition->setSelected(false);
                this->objectEdition = NULL;
            }
            
            this->mainEngine->translateSimulatedObject(this->objectOffset, vector);
            this->mainEngine->updatePositionSimulatedObject(this->objectOffset, vector);
        }
        
        this->objectOffset = NULL;
    }
    
#if defined (_3D_)
//    TODO revise: pan
    //move scene
//    if (_countFingers == 2) {
//        if (previousVector) {
//            if (previousVector->x > vector->x) {
//                scalePanX -= PAN_SCALE;
//            } else if (previousVector->x < vector->x) {
//                scalePanX += PAN_SCALE;
//            }
//
//            if (previousVector->y > vector->y) {
//                scalePanY += PAN_SCALE;
//            } else if (previousVector->y < vector->y) {
//                scalePanY -= PAN_SCALE;
//            }
//
//            this->mainEngine->pan(scalePanX, scalePanY);
//
//            delete previousVector;
//            previousVector = NULL;
//        }
//        
//        previousVector = new Vector3(vector);
//    }
#endif
    
    delete vector;
    vector = NULL;
}

void Controller::pinchDetected(real _scale, real _velocity, bool _began)
{
    if (this->objectEdition && this->objectEdition->isSelected() && this->objectEdition->hasFiniteMass()) {
        if (_began) {
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
            scaleZoom+=ZOOM_SCALE;
        } else {
            scaleZoom-=ZOOM_SCALE;
        }
        // limit zoom in
        if (scaleZoom >= ZOOM_IN_LIMIT){
            scaleZoom = ZOOM_IN_LIMIT;
            return;
        }
        // limit zoom out
        if (scaleZoom <= ZOOM_OUT_LIMIT) {
            scaleZoom = ZOOM_OUT_LIMIT;
            return;
        }
        this->mainEngine->zoom(scaleZoom);
        previousZoom = _scale;
    }
}

void Controller::rotationDetected(real _radians, real _velocity, bool _began)
{
    if (_began) {
        previousRadians = 0.0f;
    }
  
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->rotateSimulatedObject(this->objectEdition, previousRadians < _radians ? -0.03 : 0.03);
    }
    previousRadians = _radians;
}

void Controller::doubleTapOneFingerDetected(real _x, real _y)
{
    Vector3 * vector = new Vector3(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);
    
    if (this->objectEdition) {
        if (this->objectEdition->isSelected()) {
            this->objectEdition->setSelected(false);
            this->objectEdition = NULL;
        } else {
            this->objectEdition->setSelected(true);
        }
    }
    
    delete vector;
    vector = NULL;    
}

void Controller::longPressDetected(real _x, real _y)
{
    Vector3 * vector = new Vector3(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);

    if (this->objectEdition) {
        this->mainEngine->deleteSimulatedObject(this->objectEdition);
    }
    
    this->objectEdition = NULL;
}

void Controller::swipeRightDetected(real _x, real _y)
{
    // TODO put your code here
}
void Controller::swipeLeftDetected(real _x, real _y)
{
    // TODO put your code here
}

void Controller::oneTapThreeFingerDetected(real _x, real _y)
{
//    delete previousVector;
//    previousVector = NULL;
    
    scaleZoom = ZOOM_INIT;
    this->mainEngine->zoom(scaleZoom);
}

void Controller::createSimulatedObject2D(TypeObject _typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }

    this->mainEngine->makeSimulatedObject2D(_typeObject);
}

void Controller::createSimulatedObject3D(TypeObject _typeObject)
{
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }
    
    this->mainEngine->makeSimulatedObject3D(_typeObject);
}

void Controller::clearSimularion()
{
    this->mainEngine->deleteAllSimulatedObjects();
    
    if (this->objectEdition) {
        delete this->objectEdition;
    }
    
    if (this->objectOffset) {
        delete this->objectOffset;
    }
    
    this->objectEdition = NULL;
    this->objectOffset = NULL;
}