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
    this->editMode = false;
}

Controller::~Controller()
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

//!Static method to get the instance (singleton)
Controller * Controller::getInstance()
{
    if (!Controller::controller) {
        Controller::controller = new Controller();
        Controller::controller->initializeEngine();
    }
    
    return Controller::controller;
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

//!Responsible receive the callback method update the OpenGL and send to MainEngine
void Controller::updateInformation(real _duration)
{
    // use or not use _duration variable and set its value if too high
    if (!USE_LAST_TIME_GLKIT && _duration > 0.05f) {
        _duration = 0.01;
    }
    // send de MainEngine treat
    this->mainEngine->updateInformation(USE_LAST_TIME_GLKIT ? _duration : 0.02f);
}

//!Responsible receive the callback method draw the OpenGL and send to MainGraphic
void Controller::draw()
{
    this->mainGraphic->draw(this->mainEngine->getWorld());
}

//!Stop simulation
void Controller::stopSimulation()
{
    this->mainEngine->stop();

    this->editMode = false;

    scaleZoom = ZOOM_INIT;
    this->mainEngine->zoom(scaleZoom);
    this->mainEngine->resetCamera();
}

//!Start simulation
void Controller::startSimulation()
{
    this->editMode = false;
    this->mainEngine->start();
}

//!Start simulation in edit mode
void Controller::editSimulation()
{
    this->editMode = true;
}

bool Controller::isEditMode()
{
    return this->editMode;
}

void Controller::setEditMode(bool _editMode)
{
    this->editMode = _editMode;
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

//!Method responsible to receive information from touch screen
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

//!Method responsible for receiving and manipulating information than pinch gesture
void Controller::pinchDetected(real _scale, real _velocity, bool _began)
{
    // checks whether the object is selected and in order to be able to climb edition of the object
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
        // zooming in the scene
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

//!Method responsible for receiving and manipulating information than rotation gesture
void Controller::rotationDetected(real _radians, real _velocity, bool _began)
{
    if (_began) {
        previousRadians = 0.0f;
    }
    // checks whether the object is selected to be able to rotate it
    if (this->objectEdition && this->objectEdition->isSelected()) {
        this->mainEngine->rotateSimulatedObject(this->objectEdition, previousRadians < _radians ? -0.03 : 0.03);
    } else {
        // else rotates the scene
        this->mainEngine->rotateCamera(previousRadians < _radians ? -1 : 1);
    }
    previousRadians = _radians;
}

//!Method responsible for receiving and manipulating information than doubleTabOnFinger gesture
void Controller::doubleTapOneFingerDetected(real _x, real _y)
{
    Vector3 vector(_x, _y);

    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);

    // checks whether the object is selected in order to place edition
    if (this->objectEdition) {
        if (this->objectEdition->isSelected()) {
            this->objectEdition->setSelected(false);
            this->objectEdition = NULL;
        } else {
            this->objectEdition->setSelected(true);
        }
    }
}

//!Method responsible for receiving and manipulating information than longPressDetected gesture
void Controller::longPressDetected(real _x, real _y)
{
    Vector3 vector(_x, _y);

    this->objectEdition = this->mainEngine->selectedSimulatedObject(vector);

    // checks whether the object is selected to put power to delete it
    if (this->objectEdition) {
        this->mainEngine->deleteSimulatedObject(this->objectEdition);
    }
    
    this->objectEdition = NULL;
}

//!Method responsible for receiving and manipulating information than swipeRight gesture
void Controller::swipeRightDetected(real _x, real _y)
{
    // TODO put your code here
}

//!Method responsible for receiving and manipulating information than swipeLeft gesture
void Controller::swipeLeftDetected(real _x, real _y)
{
    // TODO put your code here
}

//!Method responsible for receiving and manipulating information than oneTapTreeFinger gesture
void Controller::oneTapThreeFingerDetected(real _x, real _y)
{
//    delete previousVector;
//    previousVector = NULL;

    // reset camera
    scaleZoom = ZOOM_INIT;
    this->mainEngine->resetCamera();
}

//!Method responsible for creating a 2D object
void Controller::createSimulatedObject2D(TypeObject _typeObject)
{    
    if (this->objectEdition) {
        this->objectEdition->setSelected(false);
    }

    this->mainEngine->makeSimulatedObject2D(_typeObject);
}

//!Method responsible for make a 3D object
SimulatedObject * Controller::makeSimulatedObject3D()
{
    return this->mainEngine->makeSimulatedObject3D(this->typeNextObject);
}

//!Method responsible for make a 3D object by type 
SimulatedObject * Controller::makeSimulatedObject3D(TypeObject _typeObject)
{
    return this->mainEngine->makeSimulatedObject3D(_typeObject);
}

//!Method responsible initiate an object and puts it in the object list
void Controller::addAndInitSimulatedObject3D(SimulatedObject * _simulatedObject, const Vector3 &_gravity)
{
    this->mainEngine->addAndInitializeSimulatedObject3D(_simulatedObject, _gravity);
}

//!Method responsible to update an object
void Controller::updateSimulatecObject(SimulatedObject * _simulatedObject, const Vector3 &_gravity)
{
    this->mainEngine->updateSimulatecObject(_simulatedObject, _gravity);
}

//!Method responsible for creating a 3D object by typeObject
void Controller::createSimulatedObject3D(TypeObject _typeObject)
{
    // make object 3D
    SimulatedObject * simulatedObject = this->mainEngine->makeSimulatedObject3D(_typeObject);
    // add and initialize
    this->mainEngine->addAndInitializeSimulatedObject3D(simulatedObject);
}

//!Method responsible for cleaning the simulation, excluding all objects and freeing memory
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

//!Method responsible for reading through a simulation of a file
void Controller::loadSceneFromFile(string _contentFile)
{
    long posBegin[4000];
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
    int r=0, g=0, b=0, a=0;
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

        // color
        pos = _contentFile.find("@C ", posBegin[i]);
        posAux = _contentFile.find("\n", pos);
        pos = pos+3;

        r=0, g=0, b=0, a=0;
        sscanf(_contentFile.substr(pos, posAux-pos).c_str(), "%u %u %u %u", &r, &g, &b, &a);
        object->setColorAux(r, g, b, a);

        this->addAndInitSimulatedObject3D(object, object->getAccelerationGravity());
    }
}

//!Method responsible to generate a file with the current simulation
string Controller::generateSimulationToCharacter()
{
    vector<SimulatedObject *> * objects = this->mainEngine->getWorld()->getSimulatedObjects();

    string str;

    if (objects && objects->size() == 0) {
        return str;
    }

    time_t t = time(0);
    struct tm * now = localtime(&t);

    char * charTime = new char(16);
    sprintf(charTime, "%u-%02u-%02u %02u:%02u:%02u", now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);

    str.append("# time: ").append(charTime).append("\n\n");

    SimulatedObject * object = NULL;
    for (unsigned i=0; i<objects->size(); i++) {
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

        char buffer [50];
        // mass
        gcvt(object->getMass(), 7, buffer);
        str.append("@M ").append(buffer).append("\n");

        // coefficient friction
        gcvt(object->getFriction(), 7, buffer);
        str.append("@CF ").append(buffer).append("\n");

        // coefficient restitution
        gcvt(object->getRestitution(), 7, buffer);
        str.append("@CR ").append(buffer).append("\n");

        // coefficient linear damping
        gcvt(object->getLinearDamping(), 7, buffer);
        str.append("@CLD ").append(buffer).append("\n");

        // coefficient angular damping
        gcvt(object->getAngularDamping(), 7, buffer);
        str.append("@CAD ").append(buffer).append("\n");

        // can awake
        gcvt(object->isCanSleep(), 1, buffer);
        str.append("@AWAKE ").append(buffer).append("\n");

        // acceleration gravity
        sprintf(buffer, "%f %f %f", object->getAccelerationGravity().x, object->getAccelerationGravity().y, object->getAccelerationGravity().z);
        str.append("@AG ").append(buffer).append("\n");

        // position
        sprintf(buffer, "%f %f %f", object->getPosition().x, object->getPosition().y, object->getPosition().z);
        str.append("@P ").append(buffer).append("\n");

        // velocity
        sprintf(buffer, "%f %f %f", object->getVelocity().x, object->getVelocity().y, object->getVelocity().z);
        str.append("@V ").append(buffer).append("\n");

        // rotation
        sprintf(buffer, "%f %f %f", object->getRotation().x, object->getRotation().y, object->getRotation().z);
        str.append("@R ").append(buffer).append("\n");

        // acceleration
        sprintf(buffer, "%f %f %f", object->getAcceleration().x, object->getAcceleration().y, object->getAcceleration().z);
        str.append("@A ").append(buffer).append("\n");

        // force
        sprintf(buffer, "%f %f %f", object->getForceAccum().x, object->getForceAccum().y, object->getForceAccum().z);
        str.append("@F ").append(buffer).append("\n");

        // color
        sprintf(buffer, "%u %u %u %u", object->getColorAux()->r, object->getColorAux()->g, object->getColorAux()->b, object->getColorAux()->a);
        str.append("@C ").append(buffer).append("\n");

        // end of object
        str.append("END").append("\n");
        str.append("\n");
    }

    return str;
}

//!Responsible method to generate a simulation with random values ​​for objects
void Controller::generateRandonSimulation(unsigned _numberObjects)
{
    this->mainEngine->deleteAllSimulatedObjects();

    // limit x and z -2.5 a 2.5
    // limit y 0.1 a 1.5
    real x=0, y=0, z=0;

    SimulatedObject * object = NULL;
    object = this->makeSimulatedObject3D(PLAN);
    object->setMass(0.0f);
    object->setFriction(0.9f);
    this->addAndInitSimulatedObject3D(object);

    srand(time(NULL));

    for (int i=0; i< _numberObjects; i++) {
        if (i%2 == 0) {
            object = this->makeSimulatedObject3D(BOX);
        } else {
            object = this->makeSimulatedObject3D(SPHERE);
        }

        // position
        x = getRand(-2.0f, 2.0f);
        y = getRand(0.1f, 1.5f);
        z = getRand(-2.0f, 2.0f);
        object->setPosition(x, y, z);

        // velocity
        x = getRand(-1.0f, 1.0f);
        y = getRand(-1.0f, 1.0f);
        z = getRand(-1.0f, 1.0f);
        object->setVelocity(x, y, z);

        // torque
        x = getRand(-0.5f, 0.5f);
        y = getRand(-0.5f, 0.5f);
        z = getRand(-0.5f, 0.5f);
        object->setRotation(x, y, z);
        
        object->setFriction(getRand(0.1f, 0.6f));
        object->setRestitution(getRand(0.1f, 0.6f));
        object->setLinearDamping(getRand(0.5f, 0.9f));
        object->setAngularDamping(getRand(0.5f, 0.9f));
        object->setMass(getRand(3.0f, 10.0f));

        this->addAndInitSimulatedObject3D(object);
    }
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

std::vector<SimulatedObject *> * Controller::getSimulatedObjects()
{
    return this->mainEngine->getWorld()->getSimulatedObjects();
}

//!Method responsible for deleting an object
void Controller::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    if (_simulatedObject) {
        this->mainEngine->deleteSimulatedObject(_simulatedObject);
    }
}
