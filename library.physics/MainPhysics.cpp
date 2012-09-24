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
    _particle->getPosition().addScaledVector(_particle->getVelocity(), _duration);
    
    // work out the acceleration from the force.
    Vector3 resultingAcc(_particle->getAcceleration());
    resultingAcc.addScaledVector(_particle->getForceAccum(), _particle->getInverseMass());
    
    // update linear velocity from the acceleration.
    _particle->getVelocity().addScaledVector(resultingAcc, _duration);

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
    
    // calculate linear acceleration from force
    Vector3 lastAcceleration = _body->getAcceleration();
    lastAcceleration.addScaledVector(_body->getForceAccum(), _body->getInverseMass());
    _body->setLastFrameAcceleration(lastAcceleration);
    
    // calculate angular acceleration from torque
    Matrix3 inverseInertia = _body->getInverseInertiaTensorWorld();
    Vector3 angularAcceleration = inverseInertia.transform(_body->getTorqueAccum());
    
    // adjust velocities
    // update linear velocity from both acceleration and impulse
    Vector3 velocity = _body->getVelocity();
    velocity.addScaledVector(_body->getLastFrameAcceleration(), _duration);
    _body->setVelocity(velocity);
    
    // update angular velocity from both acceleration and impulse
    Vector3 rotation = _body->getRotation();
    rotation.addScaledVector(angularAcceleration, _duration);
    
    // impose drag
    _body->setVelocity(_body->getVelocity()*real_pow(_body->getLinearDamping(), _duration));
    _body->setRotation(_body->getRotation()*real_pow(_body->getAngularDamping(), _duration));
    
    // adjust positions
    // update linear position
    Vector3 position = _body->getPosition();
    position.addScaledVector(_body->getVelocity(), _duration);
    _body->setPosition(position);
    
    // update angular position.
    Quaternion orientation = _body->getOrientation();
    orientation.addScaledVector(_body->getRotation(), _duration);
    _body->setOrientation(orientation);
    
    // normalize the orientation, and update the matrices with the new position and orientation
    _body->calculateDerivedData();

//    printf("info, velocity-> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getVelocity()->x, _body->getVelocity()->y,_body->getVelocity()->z);
//    printf("info, rotation-> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getRotation()->x, _body->getRotation()->y,_body->getRotation()->z);
//    printf("info, force   -> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getForceAccum()->x, _body->getForceAccum()->y,_body->getForceAccum()->z);
//    printf("info, torque  -> x: %2.6f, y: %2.6f, z: %2.6f \n", _body->getTorqueAccum()->x, _body->getTorqueAccum()->y,_body->getTorqueAccum()->z);
    
    // clear accumulators.
    _body->clearAccumulators();
    
    // update the kinetic energy store, and possibly put the body to sleep
    if (_body->isCanSleep()) {
        real currentMotion = _body->getVelocity().scalarProduct(_body->getVelocity()) +
                             _body->getRotation().scalarProduct(_body->getRotation());
        
        real bias = real_pow(0.5, _duration);
        _body->setMotion(bias*_body->getMotion() + (1-bias)*currentMotion);
        
//        printf("sleep, motion: %f, \n", _body->getMotion());
        if (_body->getMotion() < SLEEP_EPSILON) {
            _body->setAwake(false);
        } else if (_body->getMotion() > 10.0f * SLEEP_EPSILON) {
            _body->setMotion(10.0f * SLEEP_EPSILON);
        }
        
     }
}