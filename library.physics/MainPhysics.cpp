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
    
    if (physicalFeature->getInverseMass() <= 0.0f) {
        return;
    }
    
    // update all forces in all objects
    ForceRegistry::getInstance()->updateForces(_duration);
    
    // update linear position.
    physicalFeature->getPosition()->addScaledVector(physicalFeature->getVelocity(), _duration);
    
    // work out the acceleration from the force.
    Vector3 * resultingAcc = physicalFeature->getAcceleration();
    resultingAcc->addScaledVector(physicalFeature->getForceAccum(), physicalFeature->getInverseMass());
    
    // update linear velocity from the acceleration.
    physicalFeature->getVelocity()->addScaledVector(resultingAcc, _duration);
    
    // impose drag.
    *physicalFeature->getVelocity() *= real_pow(physicalFeature->getDamping(), _duration);
    
    physicalFeature->clearAccumulator();
    
    physicalFeature = NULL;
}
