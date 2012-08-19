//
//  PhysicalFeature.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/08/12.
//
//

#include "PhysicalFeature.h"

PhysicalFeature::PhysicalFeature()
{
    this->position = NULL;
    this->acceleration = NULL;
    this->velocity = NULL;
    this->mass = 0;
    this->volume = 0;
    this->density = 0;
    this->inverseMass = 0;
    this->damping = 0;
    this->forceAccum = MakeVector3(0.0f, 0.0f);
}

PhysicalFeature::~PhysicalFeature()
{
    if (this->position) {
        delete this->position;
    }
    if (this->acceleration) {
        delete this->acceleration;
    }
    if (this->velocity) {
        delete this->velocity;
    }
    if (this->forceAccum) {
        delete this->forceAccum;
    }
    this->position = NULL;
    this->acceleration = NULL;
    this->velocity = NULL;
    this->forceAccum = NULL;
}

PhysicalFeature * PhysicalFeature::MakePhysicalFeature(real _mass,
                                                       real _volume,
                                                       real _density,
                                                       real _damping,
                                                       Vector3 * _acceleration,
                                                       Vector3 * _position,
                                                       Vector3 * _velocity)
{
    PhysicalFeature * pf = new PhysicalFeature();
    pf->mass = _mass;
    pf->volume = _volume;
    pf->density = _density;
    pf->inverseMass = ((real)1.0)/_mass; // TODO attention
    pf->damping = _damping;
    pf->acceleration = _acceleration;
    pf->velocity = _velocity;
    pf->position = _position;
    
    return pf;
}


void PhysicalFeature::clearAccumulator()
{
    this->forceAccum->clear();
}

void PhysicalFeature::addForce(Vector3 * _force)
{
    *this->forceAccum += _force;
}

bool PhysicalFeature::hasFiniteMass()
{
    return this->mass > 0.0f;
}

real PhysicalFeature::getMass()
{
    return this->mass;
}

void PhysicalFeature::setMass(real _mass)
{
    this->mass = _mass;
}

real PhysicalFeature::getVolume()
{
    return this->volume;
}

void PhysicalFeature::setVolume(real _volume)
{
    this->volume = _volume;
}

real PhysicalFeature::getDensity()
{
    return this->density;
}

void PhysicalFeature::setDensity(real _density)
{
    this->density = _density;
}

real PhysicalFeature::getInverseMass()
{
    this->inverseMass = ((real)1.0)/this->mass; // TODO attention
    return this->inverseMass;
}

real PhysicalFeature::getDamping()
{
    return this->damping;
}

void PhysicalFeature::setDamping(real _damping)
{
    this->damping = _damping;
}

Vector3 * PhysicalFeature::getPosition()
{
    return this->position;
}

void PhysicalFeature::setPosition(Vector3 * _position)
{
    this->position = _position;
}

Vector3 * PhysicalFeature::getAcceleration()
{
    return this->acceleration;
}

void PhysicalFeature::setAcceleration(Vector3 * _acceleration)
{
    this->acceleration = _acceleration;
}

Vector3 * PhysicalFeature::getVelocity()
{
    return this->velocity;
}

void PhysicalFeature::setVelocicy(Vector3 * _velocity)
{
    this->velocity = _velocity;
}

Vector3 * PhysicalFeature::getForceAccum()
{
    return this->forceAccum;
}