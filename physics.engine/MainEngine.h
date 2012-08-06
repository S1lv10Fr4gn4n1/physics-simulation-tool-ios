//
//  MainEngine.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MAINENGINE_H
#define MAINENGINE_H

#include "Commons.h"
#include "Selection.h"
#include "NDC.h"
#include <vector>

class MainEngine {
private:
    World * world;
    bool running;
    
    NDC * ndc;
protected:
public:
    MainEngine();
    ~MainEngine();

    void start();
    void stop();
    bool isRunning();
    
    void updateInformation();
    void rotatedScreen(float _width, float _height);
    
    void addSimulatedObjectInWorld(SimulatedObject * _simulatedObject);
    void deleteAllSimulatedObjects();
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    
    // WORLD
    void zoom(float _scale);
    void pan(Pointer * _pointer);
    void centralizedWorld();
    World * getWorld();
    
    // SIMULATED OBJECT
    void scaleSimulatedObject(SimulatedObject * _simulatedObject, float _scale);
    void rotateSimulatedObject(SimulatedObject * _simulatedObject, float _radians);
    void translateSimulatedObject(SimulatedObject * _simulatedObject, Pointer * _pointer);
    
    SimulatedObject * selectedSimulatedObject(Pointer * _pointer);
    SimulatedObject * selectedBBoxSimulatedObject(Pointer * _pointer);
};

#endif
