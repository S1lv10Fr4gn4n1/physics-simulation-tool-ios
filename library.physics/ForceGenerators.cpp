//
//  ForceGenerators.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#include "ForceGenerators.h"

ForceGenerator::~ForceGenerator()
{
    // TODO put your code here
}

ForceRegistry * ForceRegistry::forceRegistry = NULL;
ForceRegistry::ForceRegistry()
{
    this->registrations = new std::vector<ForceRegistration *>();
}

ForceRegistry::~ForceRegistry()
{
    ForceRegistration * forceRegistration = NULL;
    for (int i=0; i < this->registrations->size(); i++) {
        forceRegistration = this->registrations->at(i);
        delete forceRegistration;
    }
    forceRegistration = NULL;
    
    this->registrations->clear();
    delete this->registrations;
}

//!Main method of the class, responsible for updating all the forces for all rigid bodies registered
void ForceRegistry::updateForces(real _duration)
{
    ForceRegistration * forceRegistration = NULL;
    for (int i=0; i < this->registrations->size(); i++) {
        forceRegistration = this->registrations->at(i);
        forceRegistration->forceGenerator->updateForce(forceRegistration->rigidBody, _duration);
    }
    forceRegistration = NULL;
}

//!Method for registering a force (ForceGeneration) to an object (RigidBody)
void ForceRegistry::add(RigidBody * _body, ForceGenerator * _forceGenerator)
{
    ForceRegistration * forceRegistration = new ForceRegistration();
    forceRegistration->rigidBody = _body;
    forceRegistration->forceGenerator = _forceGenerator;
    
    this->registrations->push_back(forceRegistration);
    forceRegistration = NULL;
}

//!Method for exlcuir a record of strength
void ForceRegistry::removeObject(RigidBody * _body)
{
    ForceRegistration * forceRegistration = NULL;
    int total = this->registrations->size();
    for (int i=total-1; i >= 0 ; i--) {
        forceRegistration = this->registrations->at(i);
        if (forceRegistration->rigidBody == _body) {
            delete forceRegistration->forceGenerator;
            delete forceRegistration;
            this->registrations->erase(this->registrations->begin() + i);
        }
    }
    forceRegistration = NULL;
}

//!Method to delete a strength of a rigid body
void ForceRegistry::removeForceOfObject(RigidBody * _body, ForceGenerator * _forceGenerator)
{
    ForceRegistration * forceRegistration = NULL;
    int total = this->registrations->size();
    for (int i=total-1; i >= 0 ; i--) {
        forceRegistration = this->registrations->at(i);
        if (forceRegistration->rigidBody == _body &&
            forceRegistration->forceGenerator == _forceGenerator) {
            delete forceRegistration->forceGenerator;
            this->registrations->erase(this->registrations->begin() + i);
        }
    }
    forceRegistration = NULL;
}

void ForceRegistry::clear()
{
    this->registrations->clear();
}

//!Static method to get the instance (singleton)
ForceRegistry * ForceRegistry::getInstance()
{
    if (!ForceRegistry::forceRegistry) {
        ForceRegistry::forceRegistry = new ForceRegistry();
    }
    
    return ForceRegistry::forceRegistry;
}


Gravity::Gravity(const Vector3 &_gravity, bool _considerMass)
{
    this->gravity = _gravity;
    this->considerMass = _considerMass;
}

Gravity::~Gravity()
{
    ForceGenerator::~ForceGenerator();
}

void Gravity::updateGravity(real _gravity)
{
    this->gravity.y = _gravity;
}

//!Method responsible for updating the gravitational force of a rigid body
void Gravity::updateForce(RigidBody * _body, real _duration)
{
    if (!_body->hasFiniteMass()) {
        return;
    }
    
    // apply the mass-scaled force to the body
    if (this->considerMass) {
        _body->addForce(this->gravity * _body->getMass());

    } else {
        _body->addForce(this->gravity);
    }
    
}

//Drag::Drag(real _k1, real _k2)
//{
//    this->k1 = _k1;
//    this->k2 = _k2;
//}
//
//Drag::~Drag()
//{
//    // TODO put your code here
//    ForceGenerator::~ForceGenerator();
//}
//
//void Drag::updateForce(RigidBody * _body, real _duration)
//{
//    Vector3 * force = Vector3::MakeVector3(_body->getVelocity());
//    
//    // calculate the total drag coefficient.
//    real dragCoeff = force->magnitude();
//    dragCoeff = this->k1 * dragCoeff + this->k2 * dragCoeff * dragCoeff;
//    
//    // calculate the final force and apply it.
//    force->normalize();
//    *force *= -dragCoeff;
//    
//    _body->addForce(force);
//    
//    delete force;
//    force = NULL;
//}


Spring::Spring(const Vector3 &_localConnectionPt, RigidBody * _other, const Vector3 &_otherConnectionPt,
               real _springConstant, real _restLength)
{
    this->connectionPoint = _localConnectionPt;
    this->other = _other;
    this->otherConnectionPoint = _otherConnectionPt;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

Spring::~Spring()
{
    // TODO put your code here
}

void Spring::updateForce(RigidBody * _body, real _duration)
{
    // calculate the two ends in world space.
    Vector3 lws = _body->getPointInWorldSpace(this->connectionPoint);
    Vector3 ows = this->other->getPointInWorldSpace(this->otherConnectionPoint);
    
    // calculate the vector of the spring.
    Vector3 force = lws - ows;
    
    // calculate the magnitude of the force.
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - this->restLength);
    magnitude *= this->springConstant;
    
    // calculate the final force and apply it.
    force.normalize();
    force *= -magnitude;
    _body->addForceAtPoint(force, lws);
}