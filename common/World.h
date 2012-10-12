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

    Camera * camera;
protected:
    
public:
    World();
    ~World();
    
    void addSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteAllSimulatedObject();
    
    std::vector<SimulatedObject *> * getSimulatedObjects();
    Camera * getCamera();
};

#endif
