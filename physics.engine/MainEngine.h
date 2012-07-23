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
    void rotatedScreen(float _width, float _heigth);
    void addSimulatedObjectInWorld(SimulatedObject * _simulatedObject);
    void deleteAllSimulatedObjects();
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void calcNDCCoordinates(float * _x, float * _y);
    SimulatedObject * selectedSimulatedObject(Pointer * _pointer);
    World * getWorld();    
};

#endif
