//
//  World.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "SimulatedObject.h"

class World {
private:
    std::vector<SimulatedObject *> * simulatedObjects;
    real * orthoMatrix;
    real * lookAtMatrix;
    real * perspectiveMatrix;
protected:
    
public:
    World();
    ~World();
    
    void addSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteAllSimulatedObject();
    
    real * getOrthoMatrix();
    real * getLookAtMatrix();
    real * getPerspectiveMatrix();
    std::vector<SimulatedObject *> * getSimulatedObjects();
};

#endif
