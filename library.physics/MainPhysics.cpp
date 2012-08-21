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
    if (_simulatedObject->getInverseMass() <= 0.0f) {
        return;
    }
    
    // update linear position.
    _simulatedObject->getPosition()->addScaledVector(_simulatedObject->getVelocity(), _duration);
    
    // work out the acceleration from the force.
    Vector3 * resultingAcc = Vector3::MakeVector3(_simulatedObject->getAcceleration());
    resultingAcc->addScaledVector(_simulatedObject->getForceAccum(), _simulatedObject->getInverseMass());
    
    // update linear velocity from the acceleration.
    _simulatedObject->getVelocity()->addScaledVector(resultingAcc, _duration);
    
    delete resultingAcc;
    resultingAcc = NULL;
    
//    // TODO revise: is needed? damping force is already being applied
//    // impose drag.
//    *_simulatedObject->getVelocity() *= real_pow(_simulatedObject->getDamping(), _duration);
    
//    printf("position: %f, velocity: %f, force: %f, accel: %f\n", _simulatedObject->getPosition()->y,
//                                                                 _simulatedObject->getVelocity()->y,
//                                                                 _simulatedObject->getForceAccum()->y,
//                                                                 _simulatedObject->getAcceleration()->y);
    
    _simulatedObject->clearAccumulator();
}
