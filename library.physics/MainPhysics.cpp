//
//  MainPhysics.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#include "MainPhysics.h"

MainPhysics * MainPhysics::mainPhysics = NULL;

MainPhysics::MainPhysics()
{
    // TODO put your code here
}

MainPhysics::~MainPhysics()
{
    // TODO put your code here
}

MainPhysics * MainPhysics::getInstance()
{
    if (!MainPhysics::mainPhysics) {
        MainPhysics::mainPhysics = new MainPhysics();
    }
    
    return MainPhysics::mainPhysics;
}

void MainPhysics::updateFeatures(Particle * _particle, real _duration)
{
    if (_particle->getInverseMass() <= 0.0f) {
        return;
    }
    
    // update linear position.
    _particle->getPosition()->addScaledVector(_particle->getVelocity(), _duration);
    
    // work out the acceleration from the force.
    Vector3 * resultingAcc = Vector3::MakeVector3(_particle->getAcceleration());
    resultingAcc->addScaledVector(_particle->getForceAccum(), _particle->getInverseMass());
    
    // update linear velocity from the acceleration.
    _particle->getVelocity()->addScaledVector(resultingAcc, _duration);
    
    delete resultingAcc;
    resultingAcc = NULL;
    
//    // TODO revise: is needed? damping force is already being applied
//    // impose drag.
//    *_simulatedObject->getVelocity() *= real_pow(_simulatedObject->getDamping(), _duration);
    
//    printf("position: %f, velocity: %f, force: %f, accel: %f\n", _simulatedObject->getPosition()->y,
//                                                                 _simulatedObject->getVelocity()->y,
//                                                                 _simulatedObject->getForceAccum()->y,
//                                                                 _simulatedObject->getAcceleration()->y);
    
    _particle->clearAccumulator();
}

void MainPhysics::updateFeatures(RigidBody * _body, real _duration)
{
    if (!_body->hasFiniteMass()) {
        return;
    }
    
    // calculate linear acceleration from force inputs.
    if (_body->getLastFrameAcceleration()) {
        _body->setLastFrameAcceleration(NULL);
    }
    _body->setLastFrameAcceleration(_body->getAcceleration()->x, _body->getAcceleration()->y, _body->getAcceleration()->z);
    _body->getLastFrameAcceleration()->addScaledVector(_body->getForceAccum(), _body->getInverseMass());
    
    // calculate angular acceleration from torque inputs.
    Vector3 * angularAcceleration = _body->getInverseInertiaTensorWorld()->transform(_body->getTorqueAccum());
    
    // adjust velocities
    // update linear velocity from both acceleration and impulse.
    _body->getVelocity()->addScaledVector(_body->getLastFrameAcceleration(), _duration);
    
    // update angular velocity from both acceleration and impulse.
    _body->getRotation()->addScaledVector(angularAcceleration, _duration);
    
    // impose drag.
    *_body->getVelocity() *= real_pow(_body->getLinearDamping(), _duration);
    *_body->getRotation() *= real_pow(_body->getAngularDamping(), _duration);
    
    // adjust positions
    // update linear position.
    _body->getPosition()->addScaledVector(_body->getVelocity(), _duration);
    
    // update angular position.
    // impose drag.
    *_body->getVelocity() *= real_pow(_body->getLinearDamping(), _duration);
    *_body->getRotation() *= real_pow(_body->getAngularDamping(), _duration);
    
    // normalize the orientation, and update the matrices with the new
    // position and orientation.
    _body->calculateDerivedData();
    
    // clear accumulators.
    _body->clearAccumulators();
    
    delete angularAcceleration;
    angularAcceleration = NULL;
}