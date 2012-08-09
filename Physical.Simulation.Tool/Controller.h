//
//  Controller.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
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
    
    void touchesMoved(float _x, float _y, int countFingers);
    void touchesBegan(float _x, float _y);
    void touchesEnded(float _x, float _y);
    void touchesCancelled(float _x, float _y);
    void longPressDetected(float _x, float _y);
    void pinchDetected(float scale, float velocity, bool began);
    void rotationDetected(float radians, float velocity, bool began);
    void doubleTapOneFingerDetected(float _x, float _y);
    void oneTapThreeFingerDetected(float _x, float _y);
    void swipeRightDetected(float _x, float _y);
    void swipeLeftDetected(float _x, float _y);
    
    static Controller * getInstance();
};

#endif
