//
//  TestController.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "TestController.h"


// TODO revise: variables for this place is correct?
real scaleObject = 1.0f;
real scaleZoom = ZOOM_INIT;
real scalePanX = 5.0f;
real scalePanY = 2.0f;
real previousRadians = 0.0f;
real previousZoom = 0.0f;
real previousScale = 0.0f;
Vector3 * previousVector = NULL;

TestController * TestController::testController;
TestController::TestController()
{
    this->mainEngine = NULL;
    this->mainGraphic = NULL;
    this->objectEdition = NULL;
    this->objectOffset = NULL;
}

TestController::~TestController()
{
    this->freeObjects();
}

TestController * TestController::getInstance()
{
    if (!TestController::testController) {
        TestController::testController = new TestController();
        TestController::testController->initializeEngine();
    }
    
    return TestController::testController;
}

void TestController::freeObjects()
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

void TestController::initializeContextOpenGLES()
{
    if (this->mainGraphic) {
        return;
    }
    
    this->mainGraphic = new MainGraphic();
}

void TestController::initializeEngine()
{
    if (this->mainEngine) {
        return;
    }
    
    this->mainEngine = new MainEngine();
}

void TestController::resizeScreen(real _width, real _height)
{
    this->mainEngine->rotatedScreen(_width, _height);
}

void TestController::updateInformation(real _duration)
{
    this->mainEngine->updateInformation(USE_LAST_TIME_GLKIT ? _duration : 0.02f);
}

void TestController::draw()
{
    this->mainGraphic->draw(this->mainEngine->getWorld());
}

void TestController::stopSimulation()
{
    this->mainEngine->stop();
//    scaleZoom = ZOOM_INIT;
//    this->mainEngine->zoom(scaleZoom);
}

void TestController::startSimulation()
{
    this->createScene();
    this->mainEngine->start();
}

void TestController::editSimulation()
{
    // TODO put your code here
}

bool TestController::isRunning()
{
    if (!this->mainEngine) {
        return false;
    }
    
    return this->mainEngine->isRunning();
}

bool TestController::isInitialized()
{
    if (this->mainEngine && this->mainGraphic && this->mainEngine->isRunning()) {
        return true;
    }
    
    return false;
}

void TestController::touchesBegan(real _x, real _y)
{
    // TODO put your code here
}

void TestController::touchesEnded(real _x, real _y)
{
    // TODO put your code here
}

void TestController::touchesCancelled(real _x, real _y)
{
    // TODO put your code here
}

void TestController::touchesMoved(real _x, real _y, int _countFingers)
{
    Vector3 vector(_x, _y);

    // move object
    if (_countFingers == 1) {
        this->objectOffset = this->mainEngine->selectedSimulatedObject(vector);
        
        if (this->objectOffset && this->objectOffset->hasFiniteMass()) {
            if (this->objectEdition) {
                this->objectEdition->setSelected(false);
                this->objectEdition = NULL;
            }
            
            this->mainEngine->translateSimulatedObject(this->objectOffset, vector);
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
}

void TestController::pinchDetected(real _scale, real _velocity, bool _began)
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

void TestController::rotationDetected(real _radians, real _velocity, bool _began)
{
    if (_began) {
        previousRadians = 0.0f;
    }
  
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->rotateSimulatedObject(this->objectEdition, previousRadians < _radians ? -0.03 : 0.03);
    }
    previousRadians = _radians;
}

void TestController::doubleTapOneFingerDetected(real _x, real _y)
{
    Vector3 vector(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);
    
    if (this->objectEdition) {
        if (this->objectEdition->isSelected()) {
            this->objectEdition->setSelected(false);
            this->objectEdition = NULL;
        } else {
            this->objectEdition->setSelected(true);
        }
    }
}

void TestController::longPressDetected(real _x, real _y)
{
    Vector3 vector(_x, _y);
    
    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);

    if (this->objectEdition) {
        this->mainEngine->deleteSimulatedObject(this->objectEdition);
    }
    
    this->objectEdition = NULL;
}

void TestController::swipeRightDetected(real _x, real _y)
{
    // TODO put your code here
}
void TestController::swipeLeftDetected(real _x, real _y)
{
    // TODO put your code here
}

void TestController::oneTapThreeFingerDetected(real _x, real _y)
{
//    delete previousVector;
//    previousVector = NULL;
    
    scaleZoom = 1.0f;
    this->mainEngine->zoom(scaleZoom);
}

void TestController::createSimulatedObject2D(TypeObject _typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }

    this->mainEngine->makeSimulatedObject2D(_typeObject);
}

void TestController::createSimulatedObject3D(TypeObject _typeObject)
{
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }
    
    this->mainEngine->makeSimulatedObject3D(_typeObject);
}

void TestController::clearSimularion()
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

void TestController::createScene()
{
    SimulatedObject * object = this->mainEngine->makeSimulatedObject3D(PLAN);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(0.25f, 0.1f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(0.0f, 0.1f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);


    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(-0.25f, 0.1f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);


    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(0.15f, 0.3f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);


    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(-0.15f, 0.3f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(0.0f, 0.7f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);


    

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(0.25f, 0.1f, -0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(0.0f, 0.1f, -0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(-0.25f, 0.1f, -0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(0.15f, 0.3f, -0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(-0.15f, 0.3f, -0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(0.0f, 0.5f, -0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);



    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(-0.8f, 0.1f, 0.5f);
    object->setVelocity(1.0f, 0.0f, 0.2f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    object = this->mainEngine->makeSimulatedObject3D(SPHERE);
    object->setPosition(0.8f, 0.1f, 0.5f);
    object->setVelocity(-1.5f, 0.0f, 0.0f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);

    

    object = this->mainEngine->makeSimulatedObject3D(BOX);
    object->setPosition(0.0f, 0.8f, 1.0f);
    object->setRotation(0.0f, 0.0f, 0.5f);
    this->mainEngine->addAndInitializeSimulatedObject3D(object);



//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.25f, 0.1f, -1.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.1f, -1.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.25f, 0.1f, -1.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.15f, 0.3f, -1.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.15f, 0.3f, -1.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.5f, -1.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.25f, 0.1f, -1.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.1f, -1.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.25f, 0.1f, -1.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.15f, 0.3f, -1.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.15f, 0.3f, -1.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.5f, -1.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.25f, 0.1f, -2.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.1f, -2.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.25f, 0.1f, -2.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.15f, 0.3f, -2.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.15f, 0.3f, -2.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.5f, -2.0f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    
//    
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.25f, 0.1f, -2.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.1f, -2.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.25f, 0.1f, -2.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.15f, 0.3f, -2.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(-0.15f, 0.3f, -2.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);
//
//    object = this->mainEngine->makeSimulatedObject3D(BOX);
//    object->setPosition(0.0f, 0.5f, -2.5f);
//    this->mainEngine->addAndInitializeSimulatedObject3D(object);

}
