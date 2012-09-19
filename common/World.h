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
    Matrix4x4 orthoMatrix;
    Matrix4x4 lookAtMatrix;
    Matrix4x4 perspectiveMatrix;
protected:
    
public:
    World();
    ~World();
    
    void addSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteSimulatedObject(SimulatedObject * _simulatedObject);
    void deleteAllSimulatedObject();
    
    Matrix4x4 getOrthoMatrix();
    Matrix4x4 getLookAtMatrix();
    void setLookAtMatrix(Matrix4x4 _matrix);
    Matrix4x4 getPerspectiveMatrix();
    void setPerspectiveMatrix(Matrix4x4 _matrix);
    std::vector<SimulatedObject *> * getSimulatedObjects();
};

#endif
