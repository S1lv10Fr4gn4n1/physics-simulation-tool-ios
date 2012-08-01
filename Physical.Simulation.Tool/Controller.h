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
    
    SimulatedObject * currentObject;
    
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
    void clearSimularion();
    bool isRunning();
    bool isInitialized();
    
    void addSimulatedObjectInWorld(SimulatedObject * _simulatedObject);
    void createSimulatedObject(TypeObject typeObject);
    void calcNDCCoordinates(float * _x, float * _y);
    
    void selectedSimulatedObject(Pointer * _pointer);
    void touchesMoved(Pointer * _pointer);
    void pinchDetected(float scale, float velocity);
    void rotationDetected(float radians, float velocity);
    
    static Controller * getInstance();
};

#endif
