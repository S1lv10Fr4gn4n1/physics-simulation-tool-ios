//
//  Controller.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "LibraryGraphic.h"
#include "PhysicsEngine.h"
#include "Commons.h"

class Controller {
private:
    MainEngine * mainEngine;
    MainGraphic * mainGraphic;
    
    static Controller * controller;
public:
    Controller();
    ~Controller();
    
    void initializeContextOpenGLES();
    void initializeEngine();

    void freeObjects();
    void resizeScreen(float _width, float _height);
    
    void updateInformation();
    void draw();
    void stopSimulation();
    void startSimulation();
    bool isRunning();
    bool isInitialized();
    
    void addSimulatedObjectInWorld(SimulatedObject * _simulatedObject);
    void createSimulatedObject(TypeObject typeObject);
    
    void calcNDCCoordinates(float * _x, float * _y);
    
    SimulatedObject * selectedSimulatedObject(Pointer * _pointer);
    
    static Controller * getInstance();
};

#endif
