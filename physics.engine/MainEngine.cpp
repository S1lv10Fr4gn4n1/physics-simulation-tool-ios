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
    this->world = new World();
    this->ndc = new NDC();
}

MainEngine::~MainEngine()
{ 
    delete this->world;
    delete this->ndc;
    this->world = NULL;
    this->ndc = NULL;
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

void MainEngine::updateInformation()
{
    if (!this->running) {
        return;
    }

    /// TODO - implementation
}

void MainEngine::rotatedScreen(float _width, float _height)
{
    this->ndc->update(_width, _height);
    
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
}

void MainEngine::zoom(float _scale)
{
    float value = this->ndc->getAspect() * _scale;
    
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

void MainEngine::pan(Pointer * _pointer)
{
    // TODO
    this->ndc->setLeft(-this->ndc->getAspect() + _pointer->x);
    this->ndc->setRight(this->ndc->getAspect() + _pointer->x);
    this->ndc->setBottom(-1 + _pointer->y);
    this->ndc->setTop(1 + _pointer->y);
    
    MatrixOrtho(this->world->getOrthoMatrix(),
                this->ndc->getLeft(),
                this->ndc->getRight(),
                this->ndc->getBottom(),
                this->ndc->getTop(),
                -1,
                1);
}

void MainEngine::centralizedWorld()
{
    MatrixOrtho(this->world->getOrthoMatrix(), -this->ndc->getAspect(), this->ndc->getAspect(), -1, 1, -1, 1);
}

void MainEngine::scaleSimulatedObject(SimulatedObject * _simulatedObject, float _scale)
{
    float * matrixScale = MatrixMakeScale(_scale, _scale);
    float * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation(), matrixScale);
    _simulatedObject->setMatrixTransformation(matrix);
    
    delete [] matrixScale;
    matrixScale = NULL;
    matrix = NULL;
}

void MainEngine::rotateSimulatedObject(SimulatedObject * _simulatedObject, float _radians)
{
//    float teta = (M_PI * _radians) / 180.0;
    float * matrixRotation = MatrixMakeZRotation(_radians);
    float * matrix = MatrixMultiply(_simulatedObject->getMatrixTransformation(), matrixRotation);
    _simulatedObject->setMatrixTransformation(matrix);
    
    delete [] matrixRotation;
    matrixRotation = NULL;
    matrix = NULL;
}

void MainEngine::translateSimulatedObject(SimulatedObject * _simulatedObject, Pointer * _pointer)
{
    MatrixTranslate(_simulatedObject->getMatrixTransformation(), _pointer);
}

World * MainEngine::getWorld()
{
    return this->world;
}

SimulatedObject * MainEngine::selectedSimulatedObject(Pointer * _pointer)
{
    this->ndc->calcNDCCoordinates(&_pointer->x, &_pointer->y);
    return Selection::selectSimulatedObject(this->world, _pointer);
}

void MainEngine::deleteAllSimulatedObjects()
{
    this->world->deleteAllSimulatedObject();
}

void MainEngine::deleteSimulatedObject(SimulatedObject * _simulatedObject)
{
    this->world->deleteSimulatedObject(_simulatedObject);
}

void MainEngine::makeSimulatedObject(SimulatedObject * _simulatedObject, TypeObject typeObject)
{
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
                
                _simulatedObject->addPointer(MakePointer(x1 + p1->x, y1 + p1->y));
            }
            
            delete p1;
            delete p2;
            p1 = NULL;
            p2 = NULL;
            
            break;
        }
            
        case SQUARE:
        {
            _simulatedObject->addPointer(MakePointer( -0.052083, -0.052083));
            _simulatedObject->addPointer(MakePointer(  0.052083, -0.052083));
            _simulatedObject->addPointer(MakePointer(  0.052083,  0.052083));
            _simulatedObject->addPointer(MakePointer( -0.052083,  0.052083));
            break;
        }
            
        case TRIANGLE:
        {
            _simulatedObject->addPointer(MakePointer(  0.000000,  0.052083));
            _simulatedObject->addPointer(MakePointer( -0.052083, -0.052083));
            _simulatedObject->addPointer(MakePointer(  0.052083, -0.052083));
            break;
        }
            
        case PLAN:
        {
            //left: -3.960000, right: 3.960000, bottom: -2.970000, top: 2.970000
            _simulatedObject->setMode(GL_POINTS);
            _simulatedObject->setColorAux(MakeColor(255, 0, 0, 1));
            _simulatedObject->addPointer(MakePointer(-3.960000, -2.970000));
            _simulatedObject->addPointer(MakePointer(3.960000, -2.970000));
            _simulatedObject->addPointer(MakePointer(-3.960000, 2.970000));
            _simulatedObject->addPointer(MakePointer(3.960000, 2.970000));
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
    
    _simulatedObject->initialize();
    this->world->addSimulatedObject(_simulatedObject);
}