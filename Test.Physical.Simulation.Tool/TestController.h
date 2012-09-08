//
//  TestController.h
//  Test.Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/07/12.
//  
//

#ifndef TEST_CONTROLLER_H
#define TEST_CONTROLLER_H

#include "LibraryGraphic.h"
#include "PhysicsEngine.h"
#include "Commons.h"

class TestController {
private:
    MainEngine * mainEngine;
    MainGraphic * mainGraphic;
    
    SimulatedObject * objectEdition;
    SimulatedObject * objectOffset;
    
    static TestController * testController;

public:
    TestController();
    ~TestController();
    
    void createScene();
    
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
    
    void createSimulatedObject2D(TypeObject _typeObject);
    void createSimulatedObject3D(TypeObject _typeObject);
    
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
    
    static TestController * getInstance();
};

#endif
