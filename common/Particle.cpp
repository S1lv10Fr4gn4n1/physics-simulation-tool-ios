//
//  Particle.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 19/08/12.
//
//

#include "Particle.h"

Particle::Particle()
{
    this->mass = 0.0f;
    this->inverseMass = 0.0f;
    this->damping = 0.0f;
}

Particle::~Particle()
{
    // TODO put your code here
}

void Particle::clearAccumulator()
{
    this->forceAccum.clear();
}

void Particle::addForce(const Vector3 &_force)
{
    this->forceAccum += _force;
}

bool Particle::hasFiniteMass()
{
    return this->mass > 0.0f;
}

real Particle::getMass()
{
    return this->mass;
}

void Particle::setMass(real _mass)
{
    if (_mass <= 0.0f) {
        this->mass = 0.0f;
        this->inverseMass = 0.0f;
    } else {
        this->inverseMass = ((real)1.0)/_mass; // TODO attention
        this->mass = _mass;
    }
}

real Particle::getInverseMass()
{
    return this->inverseMass;
}

real Particle::getDamping()
{
    return this->damping;
}

void Particle::setDamping(real _damping)
{
    this->damping = _damping;
}

Vector3 Particle::getPosition()
{
    return this->position;
}

void Particle::setPosition(const Vector3 &_position)
{
    this->position.x = _position.x;
    this->position.y = _position.y;
    this->position.z = _position.z;
}

void Particle::setPosition(real _x, real _y, real _z)
{
    this->position.x = _x;
    this->position.y = _y;
    this->position.z = _z;
}

void Particle::setPosition(real _x, real _y)
{
    this->position.x = _x;
    this->position.y = _y;
}

Vector3 Particle::getAcceleration()
{
    return this->acceleration;
}

void Particle::setAcceleration(const Vector3 &_acceleration)
{
    this->acceleration.x = _acceleration.x;
    this->acceleration.y = _acceleration.y;
    this->acceleration.z = _acceleration.z;
}

void Particle::setAcceleration(real _x, real _y, real _z)
{
    this->acceleration.x = _x;
    this->acceleration.y = _y;
    this->acceleration.z = _z;
}

void Particle::setAcceleration(real _x, real _y)
{
    this->acceleration.x = _x;
    this->acceleration.y = _y;
}

Vector3 Particle::getVelocity()
{
    return this->velocity;
}

void Particle::setVelocity(const Vector3 &_velocity)
{
    this->velocity.x = _velocity.x;
    this->velocity.y = _velocity.y;
    this->velocity.z = _velocity.z;
}

void Particle::setVelocity(real _x, real _y, real _z)
{
    this->velocity.x = _x;
    this->velocity.y = _y;
    this->velocity.z = _z;
}

void Particle::setVelocity(real _x, real _y)
{
    this->velocity.x = _x;
    this->velocity.y = _y;
}

Vector3 Particle::getForceAccum()
{
    return this->forceAccum;
}