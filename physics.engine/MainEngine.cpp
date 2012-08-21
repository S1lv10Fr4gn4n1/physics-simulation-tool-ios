//
//  MainEngine.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MainEngine.h"

using namespace std;

MainEngine::MainEngine()
{
    this->running = false;
    
    this->world = new World();
    this->mainPhysics = new MainPhysics();
    this->mainCollision = new MainCollision();
    this->ndc = new NDC();
}

MainEngine::~MainEngine()
{
    if (this->world) {
        delete this->world;
    }
    if (this->ndc) {
        delete this->ndc;
    }
    if (this->mainCollision) {
        delete this->mainCollision;
    }
    if (this->mainPhysics) {
        delete this->mainPhysics;
    }
    
    this->world = NULL;
    this->ndc = NULL;
    this->mainPhysics = NULL;
    this->mainCollision = NULL;
}

void MainEngine::start()
{
    this->running = true;
}

void MainEngine::stop()
{
    this->running = false;
}

bool MainEngine::isRunning()
{
    return this->running;
}

void MainEngine::updateInformation(real _duration)
{
    if (!this->running) {
        return;
    }

    if (_duration <= 0.0) {
        return;
    }
    
    SimulatedObject * object = NULL;

    // update all forces in all objects
    ForceRegistry::getInstance()->updateForces(_duration);
    
    for (int i=0; i<this->world->getSimulatedObjects()->size(); i++) {
        object = this->world->getSimulatedObjects()->at(i);
        
        // mainPhysics updates the physical features
        this->mainPhysics->updateFeatures(object, _duration);

        this->mainCollision->update(object, _duration);
        
        // mainEngine(this) translate object
        this->translateSimulatedObject(object, object->getPosition());
        
        // TODO revise
        // remove objects that left the scene
        if (real_abs(object->getPosition()->x) >= 4.0f ||
            real_abs(object->getPosition()->y) >= 3.0f) {
            this->deleteSimulatedObject(object);
        }
    }
    
    object = NULL;
}

void MainEngine::rotatedScreen(real _width, real _height)
{
    this->ndc->update(_width, _height);
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
}

void MainEngine::zoom(real _scale)
{
    real value = this->ndc->getAspect() * _scale;

    this->ndc->setLeft(-value);
    this->ndc->setRight(value);
    this->ndc->setBottom(-_scale);
    this->ndc->setTop(_scale);
    
    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
}

void MainEngine::pan(real _scaleX, real _scaleY)
{
    //TODO revise
    this->ndc->setLeft(-this->ndc->getAspect() - _scaleX);
    this->ndc->setRight(this->ndc->getAspect() - _scaleX);
    this->ndc->setBottom(-1 - _scaleY);
    this->ndc->setTop(1 - _scaleY);
    
    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
}

void MainEngine::scaleSimulatedObject(SimulatedObject * _simulatedObject, real _scale)
{
    real * matrixScale = MatrixMakeScale(_scale, _scale);
    real * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation(), matrixScale);
    _simulatedObject->setMatrixTransformation(matrix);
    
    delete [] matrixScale;
    matrixScale = NULL;
    matrix = NULL;
}

void MainEngine::rotateSimulatedObject(SimulatedObject * _simulatedObject, real _radians)
{
//    real teta = (M_PI * _radians) / 180.0;
    real * matrixRotation = MatrixMakeZRotation(_radians);
    real * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation(), matrixRotation);
    _simulatedObject->setMatrixTransformation(matrix);
    
    delete [] matrixRotation;
    matrixRotation = NULL;
    matrix = NULL;
}

void MainEngine::translateSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    MatrixTranslate(_simulatedObject->getMatrixTransformation(), _vector);
}

void MainEngine::updatePositionSimulatedObject(SimulatedObject * _simulatedObject, Vector3 * _vector)
{
    Vector3 * position = _simulatedObject->getPosition();
    position->x = _vector->x;
    position->y = _vector->y;
    position->z = _vector->z;
}

World * MainEngine::getWorld()
{
    return this->world;
}

SimulatedObject * MainEngine::selectedSimulatedObject(Vector3 * _vector)
{
    this->ndc->calcNDCCoordinates(&_vector->x, &_vector->y);
    return Selection::selectSimulatedObject(this->world, _vector);
}

void MainEngine::deleteAllSimulatedObjects()
{
    this->world->deleteAllSimulatedObject();
}

void MainEngine::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    // remove object of ForceRegistry
    ForceRegistry::getInstance()->removeObject(_simulatedObject);
    this->world->deleteSimulatedObject(_simulatedObject);
}

void MainEngine::makeSimulatedObject(SimulatedObject * _simulatedObject, TypeObject typeObject)
{
    switch (typeObject) {
        case CIRCLE:
        {
            // calculates the radius
            // takes the first point, which indicates the origin of the circle
            Vector3 * v1 = MakeVector3(0.0f, 0.0f);
            Vector3 * v2 = MakeVector3(0.0f, 0.052083f);
            
            real x = v2->x - v1->x;
            real y = v2->y - v1->y;
            
            // d²=(x0-x)²+(y0-y)²
            real d = (x*x) + (y*y);
            real radius = real_pow(d, 0.5f);
            
            real x1;
            real y1;
            
            // generates points to create the circle, these points are stored
            // to be subsequently used in the algorithm scanline
            int ang = 0;
            for (int i=0; i<36; i++) {
                x1 = (radius * cos(M_PI * ang / 180.0f));
                y1 = (radius * sin(M_PI * ang / 180.0f));
                
                _simulatedObject->addVector3(MakeVector3(x1 + v1->x, y1 + v1->y));
                ang+=10;
            }
            
            delete v1;
            delete v2;
            v1 = NULL;
            v2 = NULL;
            
            break;
        }
            
        case SQUARE:
        {
            _simulatedObject->addVector3(MakeVector3( -0.052083f, -0.052083f));
            _simulatedObject->addVector3(MakeVector3(  0.052083f, -0.052083f));
            _simulatedObject->addVector3(MakeVector3(  0.052083f,  0.052083f));
            _simulatedObject->addVector3(MakeVector3( -0.052083f,  0.052083f));
            break;
        }
            
        case TRIANGLE:
        {
            _simulatedObject->addVector3(MakeVector3(  0.000000f,  0.052083f));
            _simulatedObject->addVector3(MakeVector3( -0.052083f, -0.052083f));
            _simulatedObject->addVector3(MakeVector3(  0.052083f, -0.052083f));
            break;
        }
            
        case PLAN:
        {
            // TODO this is max screem possible for simulation
            // left: -3.960000, right: 3.960000, bottom: -2.970000, top: 2.970000
            _simulatedObject->setMass(0.0f); // immovable
            _simulatedObject->addVector3(MakeVector3(-5.0f, -7.0f));
            _simulatedObject->addVector3(MakeVector3(-5.0f, -0.9f));
            _simulatedObject->addVector3(MakeVector3( 5.0f, -0.9f));
            _simulatedObject->addVector3(MakeVector3( 5.0f, -7.0f));
            break;
        }
            
        case ENGINE:
            break;
            
        case SPRING:
            break;
            
        case SPRINGS:
            break;
            
        case POLYGON_OPEN:
            break;
            
        case POLYGON_CLOSE:
            break;
        
        case PARTICLE:
            _simulatedObject->addVector3(MakeVector3(0.0f, 0.5f));
            break;

        default:
            break;
    }
    
    _simulatedObject->initialize();
    this->world->addSimulatedObject(_simulatedObject);
}