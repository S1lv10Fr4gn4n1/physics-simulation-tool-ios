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
    Vector3 * resultingAcc = new Vector3(_particle->getAcceleration());
    resultingAcc->addScaledVector(_particle->getForceAccum(), _particle->getInverseMass());
    
    // update linear velocity from the acceleration.
    _particle->getVelocity()->addScaledVector(resultingAcc, _duration);
    
    delete resultingAcc;
    resultingAcc = NULL;
    
    _particle->clearAccumulator();
}

void MainPhysics::updateFeatures(RigidBody * _body, real _duration)
{
    if (!_body->hasFiniteMass()) {
        return;
    }

    if (!_body->isAwake()) {
        return;
    }
    
    // calculate linear acceleration from force inputs.
//    if (_body->getLastFrameAcceleration()) {
//        _body->setLastFrameAcceleration(NULL);
//    }
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
    _body->getOrientation()->addScaledVector(_body->getRotation(), _duration);
    
    // normalize the orientation, and update the matrices with the new
    // position and orientation.
    _body->calculateDerivedData();

    printf("info, velocity-> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getVelocity()->x, _body->getVelocity()->y,_body->getVelocity()->z);
//    printf("info, rotation-> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getRotation()->x, _body->getRotation()->y,_body->getRotation()->z);
    printf("info, force   -> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getForceAccum()->x, _body->getForceAccum()->y,_body->getForceAccum()->z);
//    printf("info, torque  -> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getTorqueAccum()->x, _body->getTorqueAccum()->y,_body->getTorqueAccum()->z);
    
    // clear accumulators.
    _body->clearAccumulators();
    
    delete angularAcceleration;
    angularAcceleration = NULL;
    
    // update the kinetic energy store, and possibly put the body to sleep
    if (_body->isCanSleep()) {
        real currentMotion = _body->getVelocity()->scalarProduct(_body->getVelocity()) +
                             _body->getRotation()->scalarProduct(_body->getRotation());
        
        real bias = real_pow(0.5, _duration);
        _body->setMotion(bias*_body->getMotion() + (1-bias)*currentMotion);
        
        printf("sleep, motion: %f, \n", _body->getMotion());
        if (_body->getMotion() < SLEEP_EPSILON) {
            _body->setAwake(false);
        } else if (_body->getMotion() > 10 * SLEEP_EPSILON) {
            _body->setMotion(10 * SLEEP_EPSILON);
        }
        
//        real currentMotion = velocity.scalarProduct(velocity) +
//                             rotation.scalarProduct(rotation);
//        
//        real bias = real_pow(0.5, duration);
//        motion = bias*motion + (1-bias)*currentMotion;
//        
//        printf("sleep, motion: %f, \n", motion);
//        if (motion < sleepEpsilon)
//            setAwake(false);
//        else if (motion > 10 * sleepEpsilon)
//            motion = 10 * sleepEpsilon;

        
        
    }
}