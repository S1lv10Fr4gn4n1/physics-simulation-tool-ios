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
    
    SimulatedObject * objectEdition;
    SimulatedObject * objectOffset;
    
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
    
    void createSimulatedObject(TypeObject typeObject);
    
    void touchesMoved(Pointer * _pointer);
    void touchesBegan(Pointer * _pointer);
    void touchesEnded(Pointer * _pointer);
    void touchesCancelled(Pointer * _pointer);
    void longPressDetected(Pointer * _pointer);
    void pinchDetected(float scale, float velocity);
    void rotationDetected(float radians, float velocity);
    void doubleTapOneFingerDetected(Pointer * _pointer);
    void swipeRightDetected(Pointer * _pointer);
    void swipeLeftDetected(Pointer * _pointer);
    
    static Controller * getInstance();
};

#endif
