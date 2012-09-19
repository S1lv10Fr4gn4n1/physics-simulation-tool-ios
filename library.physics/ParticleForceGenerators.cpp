//
//  ParticleForceGenerators.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 17/08/12.
//
//

#include "ParticleForceGenerators.h"

ParticleForceGenerator::~ParticleForceGenerator()
{
    // TODO put your code here
}


ParticleForceRegistry * ParticleForceRegistry::particleForceRegistry = NULL;
ParticleForceRegistry::ParticleForceRegistry()
{
    this->registrations = new std::vector<ParticleForceRegistration *>();
}

ParticleForceRegistry::~ParticleForceRegistry()
{
    ParticleForceRegistration * particleforceRegistration = NULL;
    for (int i=0; i < this->registrations->size(); i++) {
        particleforceRegistration = this->registrations->at(i);
        delete particleforceRegistration;
    }
    particleforceRegistration = NULL;

    this->registrations->clear();
    delete this->registrations;
}

void ParticleForceRegistry::updateForces(real _duration)
{
    ParticleForceRegistration * particleforceRegistration = NULL;
    for (int i=0; i < this->registrations->size(); i++) {
        particleforceRegistration = this->registrations->at(i);
        particleforceRegistration->particleForceGenerator->updateForce(particleforceRegistration->particle, _duration);
    }
    particleforceRegistration = NULL;
}

void ParticleForceRegistry::add(Particle * _particle, ParticleForceGenerator * _particleForceGenerator)
{
    ParticleForceRegistration * particleForceRegistration = new ParticleForceRegistration();
    particleForceRegistration->particle = _particle;
    particleForceRegistration->particleForceGenerator = _particleForceGenerator;

    this->registrations->push_back(particleForceRegistration);
    particleForceRegistration = NULL;
}

void ParticleForceRegistry::removeObject(Particle * _particle)
{
    ParticleForceRegistration * particleForceRegistration = NULL;
    int total = this->registrations->size();
    for (int i=total-1; i >= 0 ; i--) {
        particleForceRegistration = this->registrations->at(i);
        if (particleForceRegistration->particle == _particle) {
            delete particleForceRegistration->particleForceGenerator;
            delete particleForceRegistration;
            this->registrations->erase(this->registrations->begin() + i);
        }
    }
    particleForceRegistration = NULL;
}

void ParticleForceRegistry::removeForceOfObject(Particle * _particle, ParticleForceGenerator * _particleForceGenerator)
{
    ParticleForceRegistration * particleForceRegistration = NULL;
    int total = this->registrations->size();
    for (int i=total-1; i >= 0 ; i--) {
        particleForceRegistration = this->registrations->at(i);
        if (particleForceRegistration->particle == _particle && particleForceRegistration->particleForceGenerator == _particleForceGenerator) {
            delete particleForceRegistration->particleForceGenerator;
            this->registrations->erase(this->registrations->begin() + i);
        }
    }
    particleForceRegistration = NULL;
}

void ParticleForceRegistry::clear()
{
    this->registrations->clear();
}

ParticleForceRegistry * ParticleForceRegistry::getInstance()
{
    if (!ParticleForceRegistry::particleForceRegistry) {
        ParticleForceRegistry::particleForceRegistry = new ParticleForceRegistry();
    }
    
    return ParticleForceRegistry::particleForceRegistry;
}


ParticleGravity::ParticleGravity(const Vector3 &_gravity)
{
    this->gravity = _gravity;
}

ParticleGravity::~ParticleGravity()
{
    // TODO put your code here
}

void ParticleGravity::updateGravity(real _value)
{
    this->gravity.y = _value;
}

void ParticleGravity::updateForce(Particle * _particle, real _duration)
{
    if (!_particle->hasFiniteMass()) {
        return;
    }
    
    Vector3 force = this->gravity * _particle->getMass();
    _particle->addForce(force);
}


ParticleDrag::ParticleDrag(real _k1, real _k2)
{
    this->k1 = _k1;
    this->k2 = _k2;
}

ParticleDrag::~ParticleDrag()
{
    // TODO put your code here
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleDrag::updateForce(Particle * _particle, real _duration)
{
    Vector3 force(_particle->getVelocity());
    
    // calculate the total drag coefficient.
    real dragCoeff = force.magnitude();
    dragCoeff = this->k1 * dragCoeff + this->k2 * dragCoeff * dragCoeff;
    
    // calculate the final force and apply it.
    force.normalize();
    force *= -dragCoeff;
    
    _particle->addForce(force);
}


ParticleSpring::ParticleSpring(Particle * _other, real _springConstant, real _restLength)
{
    this->other = _other;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ParticleSpring::~ParticleSpring()
{
    // TODO put your code here
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleSpring::updateForce(Particle * _particle, real _duration)
{
    // calculate the vector of the spring.
    Vector3 force(_particle->getPosition());
    force -= this->other->getPosition();
    
    // calculate the magnitude of the force.
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - this->restLength);
    magnitude *= this->springConstant;
    
    // calculate the final force and apply it.
    force.normalize();
    force *= -magnitude;
    _particle->addForce(force);
}


ParticleAnchoredSpring::ParticleAnchoredSpring(const Vector3 &_anchor, real _springConstant, real _restLength)
{
    this->anchor = _anchor;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ParticleAnchoredSpring::~ParticleAnchoredSpring()
{
    // TODO put your code here
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleAnchoredSpring::updateForce(Particle * _particle, real _duration)
{
    Vector3 force(_particle->getPosition());
    force -= this->anchor;
    
    // calculate the magnitude of the force.
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - this->restLength);
    magnitude *= this->springConstant;
    
    // calculate the final force and apply it.
    force.normalize();
    force *= -magnitude;
    _particle->addForce(force);
}

void ParticleAnchoredSpring::setAnchor(const Vector3 &_anchor)
{
    this->anchor = _anchor;
}


ParticleBungee::ParticleBungee(Particle * _other, real _springConstant, real _restLength)
{
    this->other = _other;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ParticleBungee::~ParticleBungee()
{
    // TODO put your code here
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleBungee::updateForce(Particle * _particle, real _duration)
{
    Vector3 force(_particle->getPosition());
    force -= other->getPosition();
    
    // check if the bungee is compressed.
    real magnitude = force.magnitude();
    if (magnitude <= this->restLength) {
        return;
    }
    
    // calculate the magnitude of the force.
    magnitude = this->springConstant * (this->restLength - magnitude);
    
    // calculate the final force and apply it.
    force.normalize();
    force *= -magnitude;
    _particle->addForce(force);
}


ParticleAnchoredBungee::ParticleAnchoredBungee(const Vector3 &_anchor, real _springConstant, real _restLength)
{
    this->anchor = _anchor;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ParticleAnchoredBungee::~ParticleAnchoredBungee()
{
    // TODO put your code here
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleAnchoredBungee::updateForce(Particle * _particle, real _duration)
{
    Vector3 force(_particle->getPosition());
    force -= this->anchor;
    
    // check if the bungee is compressed.
    real magnitude = force.magnitude();
    if (magnitude <= this->restLength) {
        return;
    }
    
    // calculate the magnitude of the force.
    magnitude = this->springConstant * (this->restLength - magnitude);
    
    // calculate the final force and apply it.
    force.normalize();
    force *= -magnitude;
    _particle->addForce(force);
}

void ParticleAnchoredBungee::setAnchor(const Vector3 &_anchor)
{
    this->anchor = _anchor;
}


ParticleBuoyancy::ParticleBuoyancy(real _maxDepth, real _volume, real _waterHeight, real _liquidDensity)
{
    this->maxDepth = _maxDepth;
    this->volume = _volume;
    this->waterHeight = _waterHeight;
    this->liquidDensity = _liquidDensity;
}

ParticleBuoyancy::~ParticleBuoyancy()
{
    // TODO put your code here
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleBuoyancy::updateForce(Particle * _particle, real _duration)
{
    // calculate the submersion depth.
    real depth = _particle->getPosition().y;
    
    // check if we’re out of the water.
    if (depth >= this->waterHeight + this->maxDepth) {
        return;
    }
    
    Vector3 force;
    
    // check if we’re at maximum depth.
    if (depth <= this->waterHeight - this->maxDepth) {
        force.y = this->liquidDensity * this->volume;
        _particle->addForce(force);
        return;
    }
    
    // otherwise we are partly submerged.
    force.y = this->liquidDensity * this->volume * (depth - this->maxDepth - this->waterHeight) / 2 * this->maxDepth;
    _particle->addForce(force);
}

ParticleFakeStiffSpring::ParticleFakeStiffSpring(const Vector3 &_anchor, real _springConstant, real _damping)
{
    this->anchor = _anchor;
    this->springConstant = _springConstant;
    this->damping = _damping;
}

ParticleFakeStiffSpring::~ParticleFakeStiffSpring()
{
    ParticleForceGenerator::~ParticleForceGenerator();
}

void ParticleFakeStiffSpring::updateForce(Particle * _particle, real _duration)
{
    // check that we do not have infinite mass.
    if (!_particle->hasFiniteMass()) {
        return;
    }
    
    // calculate the relative position of the particle to the anchor.
    Vector3 position = _particle->getPosition();
    position -= anchor;
    
    // calculate the constants and check whether they are in bounds.
    real gamma = 0.5f * real_sqrt(4 * this->springConstant - this->damping*this->damping);
    
    if (gamma == 0.0f) {
        return;
    }
    
    Vector3 c = (position * (this->damping / (2.0f * gamma))) + (_particle->getVelocity() * (1.0f / gamma));
    
    // calculate the target position.
    Vector3 target = (position * real_cos(gamma * _duration)) + (c * real_sin(gamma * _duration));
    target *= real_exp(-0.5f * _duration * this->damping);

    // calculate the resulting acceleration and therefore the force
    Vector3 accel = ((target - position) * (1.0f / _duration*_duration)) - (_particle->getVelocity() * _duration);
    _particle->addForce(accel * _particle->getMass());
}
