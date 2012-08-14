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
    void resizeScreen(real _width, real _height);
    
    void updateInformation(real _duration);
    void draw();
    void stopSimulation();
    void startSimulation();
    void editSimulation();
    void clearSimularion();
    bool isRunning();
    bool isInitialized();
    
    void createSimulatedObject(TypeObject _typeObject);
    
    void touchesMoved(real _x, real _y, int _countFingers);
    void touchesBegan(real _x, real _y);
    void touchesEnded(real _x, real _y);
    void touchesCancelled(real _x, real _y);
    void longPressDetected(real _x, real _y);
    void pinchDetected(real _scale, real _velocity, bool _began);
    void rotationDetected(real _radians, real _velocity, bool _began);
    void doubleTapOneFingerDetected(real _x, real _y);
    void oneTapThreeFingerDetected(real _x, real _y);
    void swipeRightDetected(real _x, real _y);
    void swipeLeftDetected(real _x, real _y);
    
    static Controller * getInstance();
};

#endif
