//
//  Controller.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "Controller.h"

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
    // TODO
}

void Controller::touchesEnded(float _x, float _y)
{
    // TODO
}

void Controller::touchesCancelled(float _x, float _y)
{
    // TODO
}

void Controller::touchesMoved(float _x, float _y)
{
    Pointer * pointer = MakePointer(_x, _y);
    this->objectOffset = this->mainEngine->selectedSimulatedObject(pointer);
    
    if (this->objectOffset) {
        if (this->objectEdition) {
            this->objectEdition->setSelected(false);
            this->objectEdition = NULL;
        }
        
        this->mainEngine->translateSimulatedObject(this->objectOffset, pointer);
    }
//    } else {
//        // move scene
//        this->mainEngine->pan(_pointer);
//    }
    delete pointer;
    pointer = NULL;
}

void Controller::pinchDetected(float _scale, float _velocity)
{
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->scaleSimulatedObject(this->objectEdition, _scale);
    } else {
        this->mainEngine->zoom(_scale);
    }
}

void Controller::rotationDetected(float _radians, float _velocity)
{
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->rotateSimulatedObject(this->objectEdition, _radians);
    }
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
    // TODO
}

void Controller::swipeRightDetected(float _x, float _y)
{
    // TODO
}
void Controller::swipeLeftDetected(float _x, float _y)
{
    // TODO
}

void Controller::oneTapThreeFingerDetected(float _x, float _y)
{
    //this->mainEngine->centralizedWorld();
}

void Controller::createSimulatedObject(TypeObject typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }
    
    SimulatedObject * object = new SimulatedObject();
// TODO    object->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));
    object->setColorAux(MakeColor(255, 255, 255, 1));
    object->setMode(GL_LINE_LOOP);
    
    switch (typeObject) {
        case CIRCLE:
        {
            // calculates the radius
            // takes the first point, which indicates the origin of the circle
            Pointer * p1 = MakePointer( 0.0, 0.0);
            Pointer * p2 = MakePointer( 0.0, 0.052083);
            
            float x = p2->x - p1->x;
            float y = p2->y - p1->y;
            
            /// d²=(x0-x)²+(y0-y)²
            float d = (x*x) + (y*y);
            float radius = pow(d, 0.5);
            
            float x1;
            float y1;
            
            /// generates points to create the circle, these points are stored
            /// to be subsequently used in the algorithm scanline
            for (int i=0; i<360; i++) {
                x1 = (radius * cos(M_PI * i / 180.0f));
                y1 = (radius * sin(M_PI * i / 180.0f));
                
                object->addPointer(MakePointer(x1 + p1->x, y1 + p1->y));
            }
            
            delete p1;
            delete p2;
            p1 = NULL;
            p2 = NULL;
            
            break;
        }   
            
        case SQUARE:
        {
            object->addPointer(MakePointer( -0.052083, -0.052083));
            object->addPointer(MakePointer(  0.052083, -0.052083));
            object->addPointer(MakePointer(  0.052083,  0.052083));
            object->addPointer(MakePointer( -0.052083,  0.052083));
            break;
        }
            
        case TRIANGLE:
        {
            object->addPointer(MakePointer(  0.000000,  0.052083));
            object->addPointer(MakePointer( -0.052083, -0.052083));
            object->addPointer(MakePointer(  0.052083, -0.052083));
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
}