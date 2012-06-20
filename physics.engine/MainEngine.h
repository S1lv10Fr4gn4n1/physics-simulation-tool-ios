//
//  MainEngine.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 09/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MAINENGINE_H
#define MAINENGINE_H

#include "World.h"

class MainEngine {
private:
    World * world;
protected:
public:
    MainEngine();
    ~MainEngine();
    
    void updateInformation();
    void rotatedScreen(float _width, float _heigth);
    
    void addSimulatedObjectInWorld(SimulatedObject * _simulatedObject);
    
    World * getWorld();
    
    void start();
};

#endif
