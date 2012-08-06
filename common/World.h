//
//  World.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "SimulatedObject.h"

class World {
private:
    std::vector<SimulatedObject *> * simulatedObjects;
    float * orthoMatrix;
protected:
    
public:
    World();
    ~World();
    
    void addSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteAllSimulatedObject();
    
    void setOrthoMatrix(float * _matrix);
    float * getOrthoMatrix();
    
    std::vector<SimulatedObject *> * getSimulatedObjects();
};

#endif
