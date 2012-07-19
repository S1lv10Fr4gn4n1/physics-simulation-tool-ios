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
    world = new World();
}

MainEngine::~MainEngine()
{ 
    delete world;
}

void MainEngine::start()
{
    running = true;
}

void MainEngine::stop()
{
    running = false;
}

bool MainEngine::isRunning()
{
    return running;
}

void MainEngine::updateInformation()
{
    if (!running) {
        return;
    }

    /// TODO - implementation
}

void MainEngine::rotatedScreen(float _width, float _heigth)
{
    /// TODO - implementation
}

World * MainEngine::getWorld()
{
    return world;
}

void MainEngine::addSimulatedObjectInWorld(SimulatedObject * _simulatedObject)
{
    _simulatedObject->initialize();    
    
    world->addSimulatedObject(_simulatedObject);
}

void MainEngine::createSimulatedObject(TypeObject typeObject)
{
    SimulatedObject * object = new SimulatedObject();
    object->setMode(0x0002); // TODO - revise
    object->setColor(MakeColor(0, 0, 0, 0, 4));
    object->setPhysicalFeature(MakePhysicalFeature(1, 1, 1, 1, 1));    
    object->addAllPointers(makePointersToObject(typeObject));
    
    object->initialize();
    world->addSimulatedObject(object);
}

vector<Pointer *> * MainEngine::makePointersToObject(TypeObject typeObject)
{
    vector<Pointer *> * vectorPointers = new vector<Pointer *>();
    
    switch (typeObject) {
        case CIRCLE:
        {
            /// calculates the radius
            /// takes the first point, which indicates the origin of the circle
            Pointer * p1 = MakePointer( 0.0,  0.0, 0.0);
            Pointer * p2 = MakePointer( 0.0, 0.1, 0.0);
            
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
                
                vectorPointers->push_back(MakePointer(x1 + p1->x, y1 + p1->y, 0.0));
                ang += 10;
            }

            break;
        }
            
        case SQUARE:
        {
            vectorPointers->push_back(MakePointer(-0.1, 0.1, 0.0));
            vectorPointers->push_back(MakePointer( 0.1, 0.1, 0.0));
            vectorPointers->push_back(MakePointer( 0.1,-0.1, 0.0));
            vectorPointers->push_back(MakePointer(-0.1,-0.1, 0.0));
            break;
        }
            
        case TRIANGLE:
        {
            vectorPointers->push_back(MakePointer( 0.0, 0.1, 0));
            vectorPointers->push_back(MakePointer( 0.1,-0.1, 0));
            vectorPointers->push_back(MakePointer(-0.1,-0.1, 0));
            break;
        }
            
    }
    
    return vectorPointers;
}

SimulatedObject * MainEngine::selectedSimulatedObject(Pointer * _pointer)
{
    return Selection::selectSimulatedObject(world, _pointer);
}


