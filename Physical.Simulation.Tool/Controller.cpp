//
//  Controller.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#include "Controller.h"

using namespace std;

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
    this->mainEngine->updateInformation(USE_LAST_TIME_GLKIT ? _duration : 0.02f);
}

void Controller::draw()
{
    this->mainGraphic->draw(this->mainEngine->getWorld());
}

void Controller::stopSimulation()
{
    this->mainEngine->stop();

    scaleZoom = ZOOM_INIT;
    this->mainEngine->zoom(scaleZoom);
    this->mainEngine->resetCamera();
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
    } else {
        this->mainEngine->rotateCamera(previousRadians < _radians ? -1 : 1);
    }
    previousRadians = _radians;
}

void Controller::doubleTapOneFingerDetected(real _x, real _y)
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

void Controller::longPressDetected(real _x, real _y)
{
    Vector3 vector(_x, _y);

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
    this->mainEngine->resetCamera();
}

void Controller::createSimulatedObject2D(TypeObject _typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }

    this->mainEngine->makeSimulatedObject2D(_typeObject);
}

SimulatedObject * Controller::makeSimulatedObject3D()
{
    return this->mainEngine->makeSimulatedObject3D(this->typeNextObject);
}

SimulatedObject * Controller::makeSimulatedObject3D(TypeObject _typeObject)
{
    return this->mainEngine->makeSimulatedObject3D(_typeObject);
}

void Controller::addAndInitSimulatedObject3D(SimulatedObject * _simulatedObject, const Vector3 &_gravity)
{
    this->mainEngine->addAndInitializeSimulatedObject3D(_simulatedObject, _gravity);
}

void Controller::createSimulatedObject3D(TypeObject _typeObject)
{
    SimulatedObject * simulatedObject = this->mainEngine->makeSimulatedObject3D(_typeObject);
    this->mainEngine->addAndInitializeSimulatedObject3D(simulatedObject);
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

void Controller::loadSceneFromFile(string _contentFile)
{
    //# # comment
    //# BEGIN init of object
    //# T type object (PLAN, SPHERE, BOX, TRINAGLE)
    //# M mass
    //# P position
    //# V velocity
    //# R rotation
    //# A acceleration
    //# F force
    //# AG acceleration gravity
    //# CF coefficient friction
    //# CR coefficient restitution
    //# CLD coefficient linear damping
    //# CAD coefficient angular damping
    //# AWAKE can awake
    //# END ended object
    //
    //    BEGIN
    //    T PLAN
    //    M 0.0 # no move
    //    P 0.0 0.0 0.0
    //    CF 0.9
    //    END
    //
    //    BEGIN
    //    T SPHERE
    //    M 4.0
    //    P 0.0 0.1 0.1
    //    AG 9.8
    //    CF 0.9
    //    CR 0.4
    //    CLD 0.99
    //    CAD 0.8
    //    AWAKE 1
    //    END

    long posBegin[200];
    unsigned index = 0;

    size_t pos = _contentFile.find("BEGIN");
    while (pos != string::npos) {
        posBegin[index] = pos;
        index++;
        pos = _contentFile.find("BEGIN", pos+1);
    }

    if (index == 0) {
        return;
    }

    unsigned totalObjects = index;

    SimulatedObject * object = NULL;
    string str;
    real realValue;
    Vector3 vectorValue;
    char x[10], y[10], z[10];
    size_t posAux = string::npos;

    for (int i=0; i<totalObjects; i++) {
        // type object
        pos = _contentFile.find("@T ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        str = _contentFile.substr(pos, posAux-pos);
        if (str == "PLAN") {
            object = this->makeSimulatedObject3D(PLAN);
        } else if (str == "SPHERE") {
            object = this->makeSimulatedObject3D(SPHERE);
        } else if (str == "BOX") {
            object = this->makeSimulatedObject3D(BOX);
        } else if (str == "PYRAMID") {
            object = this->makeSimulatedObject3D(PYRAMID);
        } else {
            continue;
        }

        // mass
        pos = _contentFile.find("@M ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;
        realValue = atof(_contentFile.substr(pos, posAux-pos).c_str());
        object->setMass(realValue);

        // position
        pos = _contentFile.find("@P ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%s %s %s", x, y, z);
        vectorValue.x = atof(x);
        vectorValue.y = atof(y);
        vectorValue.z = atof(z);
        object->setPosition(vectorValue);

        // velocity
        pos = _contentFile.find("@V ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%s %s %s", x, y, z);
        vectorValue.x = atof(x);
        vectorValue.y = atof(y);
        vectorValue.z = atof(z);
        object->setVelocity(vectorValue);

        // rotation
        pos = _contentFile.find("@R ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%s %s %s", x, y, z);
        vectorValue.x = atof(x);
        vectorValue.y = atof(y);
        vectorValue.z = atof(z);
        object->setRotation(vectorValue);

        // acceleration
        pos = _contentFile.find("@A ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%s %s %s", x, y, z);
        vectorValue.x = atof(x);
        vectorValue.y = atof(y);
        vectorValue.z = atof(z);
        object->setAcceleration(vectorValue);

        // force
        pos = _contentFile.find("@F ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%s %s %s", x, y, z);
        vectorValue.x = atof(x);
        vectorValue.y = atof(y);
        vectorValue.z = atof(z);
        object->addForce(vectorValue);

        // acceleration gravity
        pos = _contentFile.find("@AG ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+4;

        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%s %s %s", x, y, z);
        vectorValue.x = atof(x);
        vectorValue.y = atof(y);
        vectorValue.z = atof(z);
        object->setAccelerationGravity(vectorValue);

        // coefficient friction
        pos = _contentFile.find("@CF ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+4;
        realValue = atof(_contentFile.substr(pos, posAux-pos).c_str());
        object->setFriction(realValue);

        // coefficient restitution
        pos = _contentFile.find("@CR ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+4;
        realValue = atof(_contentFile.substr(pos, posAux-pos).c_str());
        object->setRestitution(realValue);

        // coefficient linear damping
        pos = _contentFile.find("@CLD ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+5;
        realValue = atof(_contentFile.substr(pos, posAux-pos).c_str());
        object->setLinearDamping(realValue);

        // coefficient angular damping
        pos = _contentFile.find("@CAD ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+5;
        realValue = atof(_contentFile.substr(pos, posAux-pos).c_str());
        object->setAngularDamping(realValue);

        // can awake
        pos = _contentFile.find("@AWAKE ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+7;
        realValue = atof(_contentFile.substr(pos, posAux-pos).c_str());
        object->setCanSleep(realValue);

        this->addAndInitSimulatedObject3D(object, object->getAccelerationGravity());
    }
}

string Controller::generateSimulationToCharacter()
{
    vector<SimulatedObject *> * objects = this->mainEngine->getWorld()->getSimulatedObjects();

    if (objects && objects->size() == 0) {
        return "";
    }

    string str;
    time_t t = time(0);
    struct tm * now = localtime( & t );

    char * charTime = new char(16);
    sprintf(charTime, "%u-%02u-%02u %02u:%02u:%02u", now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);

    str.append("# time: ").append(charTime).append("\n\n");

    SimulatedObject * object = NULL;
    for (int i=0; i<objects->size(); i++) {
        object = objects->at(i);

        str.append("BEGIN").append("\n");
        if (object->getTypeObject() == PLAN) {
            str.append("@T ").append("PLAN").append("\n");
        } else if (object->getTypeObject() == SPHERE) {
            str.append("@T ").append("SPHERE").append("\n");
        } else if (object->getTypeObject() == BOX) {
            str.append("@T ").append("BOX").append("\n");
        } else if (object->getTypeObject() == PYRAMID) {
            str.append("@T ").append("PYRAMID").append("\n");
        }

        char buffer [25];
        gcvt(object->getMass(), 7, buffer);
        str.append("@M ").append(buffer).append("\n");

        gcvt(object->getFriction(), 7, buffer);
        str.append("@CF ").append(buffer).append("\n");

        gcvt(object->getRestitution(), 7, buffer);
        str.append("@CR ").append(buffer).append("\n");

        gcvt(object->getLinearDamping(), 7, buffer);
        str.append("@CLD ").append(buffer).append("\n");

        gcvt(object->getAngularDamping(), 7, buffer);
        str.append("@CAD ").append(buffer).append("\n");

        gcvt(object->isCanSleep(), 1, buffer);
        str.append("@AWAKE ").append(buffer).append("\n");

        sprintf(buffer, "%2.5f %2.5f %2.5f", object->getAccelerationGravity().x, object->getAccelerationGravity().y, object->getAccelerationGravity().z);
        str.append("@AG ").append(buffer).append("\n");

        sprintf(buffer, "%2.5f %2.5f %2.5f", object->getPosition().x, object->getPosition().y, object->getPosition().z);
        str.append("@P ").append(buffer).append("\n");

        sprintf(buffer, "%2.5f %2.5f %2.5f", object->getVelocity().x, object->getVelocity().y, object->getVelocity().z);
        str.append("@V ").append(buffer).append("\n");

        sprintf(buffer, "%2.5f %2.5f %2.5f", object->getRotation().x, object->getRotation().y, object->getRotation().z);
        str.append("@R ").append(buffer).append("\n");

        sprintf(buffer, "%2.5f %2.5f %2.5f", object->getAcceleration().x, object->getAcceleration().y, object->getAcceleration().z);
        str.append("@A ").append(buffer).append("\n");

        sprintf(buffer, "%2.5f %2.5f %2.5f", object->getForceAccum().x, object->getForceAccum().y, object->getForceAccum().z);
        str.append("@F ").append(buffer).append("\n");

        str.append("END").append("\n");
        str.append("\n");
    }

    return str;
}

void Controller::setTypeNextObject(TypeObject _typeObject)
{
    this->typeNextObject = _typeObject;
}

TypeObject Controller::getTypeNextObject()
{
    return this->typeNextObject;
}

bool Controller::alreadyExistPlan()
{
    return this->mainEngine->alreadyExistsPlan();
}

SimulatedObject * Controller::getExistingPlan()
{
    return this->mainEngine->getExistingPlan();
}