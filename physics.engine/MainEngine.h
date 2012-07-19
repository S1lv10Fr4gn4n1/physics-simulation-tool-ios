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
#include <vector>

class MainEngine {
private:
    World * world;
    bool running;
    
    std::vector<Pointer *> * makePointersToObject(TypeObject typeObject);
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
    void createSimulatedObject(TypeObject typeObject);
    
    SimulatedObject * selectedSimulatedObject(Pointer * _pointer);
    
    World * getWorld();    
};

#endif
