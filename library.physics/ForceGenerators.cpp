//
//  ForceGenerators.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#include "ForceGenerators.h"

Gravity::Gravity(Vector3 * _gravity)
{
    this->gravity = _gravity;
}

Gravity::~Gravity()
{
    if (this->gravity) {
        delete this->gravity;
    }
    
    this->gravity = NULL;
}

void Gravity::updateGravity(real _gravity)
{
    this->gravity->y = _gravity;
}

void Gravity::updateForce(RigidBody * _body, real _duration)
{
    if (!_body->hasFiniteMass()) {
        return;
    }
    
    // apply the mass-scaled force to the body
    _body->addForce(*this->gravity * _body->getMass());
}


Spring::Spring(Vector3 * _localConnectionPt, RigidBody * _other, Vector3 * _otherConnectionPt,
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
    Vector3 * lws = _body->getPointInWorldSpace(this->connectionPoint);
    Vector3 * ows = this->other->getPointInWorldSpace(this->otherConnectionPoint);
    
    // calculate the vector of the spring.
    Vector3 * force = *lws - ows;
    
    // calculate the magnitude of the force.
    real magnitude = force->magnitude();
    magnitude = real_abs(magnitude - this->restLength);
    magnitude *= this->springConstant;
    
    // calculate the final force and apply it.
    force->normalize();
    *force *= -magnitude;
    _body->addForceAtPoint(force, lws);
}