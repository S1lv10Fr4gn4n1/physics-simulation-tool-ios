//
//  MainPhysics.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#include "MainPhysics.h"


MainPhysics::MainPhysics()
{
    // TODO put your code here
}

MainPhysics::~MainPhysics()
{
    // TODO put your code here
}

void MainPhysics::updateFeatures(SimulatedObject * _simulatedObject, real _duration)
{
    PhysicalFeature * physicalFeature = _simulatedObject->getPhysicalFeature();
    
    if (!physicalFeature) {
        return;
    }
    
    if (physicalFeature->inverseMass <= 0.0f) {
        return;
    }
    
    // update linear position.
    physicalFeature->position->addScaledVector(physicalFeature->velocity, _duration);
    
    // Work out the acceleration from the force.
    Vector3 * resultingAcc = physicalFeature->acceleration;
    resultingAcc->addScaledVector(physicalFeature->forceAccum, physicalFeature->inverseMass);
    
    // Update linear velocity from the acceleration.
    physicalFeature->velocity->addScaledVector(resultingAcc, _duration);
    
    // Impose drag.
    physicalFeature->velocity->multiplyVectorForValue(real_pow(physicalFeature->damping, _duration));

    physicalFeature = NULL;
}
