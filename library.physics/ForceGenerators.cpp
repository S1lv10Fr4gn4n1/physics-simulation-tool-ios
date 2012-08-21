//
//  ForceGenerators.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 17/08/12.
//
//

#include "ForceGenerators.h"

// ForceGenerator

ForceGenerator::~ForceGenerator()
{
    // TODO put your code here
}


// ForceRegistry
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

void ForceRegistry::updateForces(real _duration)
{
    ForceRegistration * forceRegistration = NULL;
    for (int i=0; i < this->registrations->size(); i++) {
        forceRegistration = this->registrations->at(i);
        forceRegistration->forceGenerator->updateForce(forceRegistration->particle, _duration);
    }
    forceRegistration = NULL;
}

void ForceRegistry::add(Particle * _particle, ForceGenerator * _forceGenerator)
{
    ForceRegistration * forceRegistration = new ForceRegistration();
    forceRegistration->particle = _particle;
    forceRegistration->forceGenerator = _forceGenerator;

    this->registrations->push_back(forceRegistration);
    forceRegistration = NULL;
}

void ForceRegistry::removeObject(Particle * _particle)
{
    ForceRegistration * forceRegistration = NULL;
    int total = this->registrations->size();
    for (int i=total-1; i >= 0 ; i--) {
        forceRegistration = this->registrations->at(i);
        if (forceRegistration->particle == _particle) {
            delete forceRegistration->forceGenerator;
            delete forceRegistration;
            this->registrations->erase(this->registrations->begin() + i);
        }
    }
    forceRegistration = NULL;
}

void ForceRegistry::removeForceOfObject(Particle * _particle, ForceGenerator * _forceGenerator)
{
    ForceRegistration * forceRegistration = NULL;
    int total = this->registrations->size();
    for (int i=total-1; i >= 0 ; i--) {
        forceRegistration = this->registrations->at(i);
        if (forceRegistration->particle == _particle && forceRegistration->forceGenerator == _forceGenerator) {
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

ForceRegistry * ForceRegistry::getInstance()
{
    if (!ForceRegistry::forceRegistry) {
        ForceRegistry::forceRegistry = new ForceRegistry();
    }
    
    return ForceRegistry::forceRegistry;
}


// ForceGravity
ForceGravity::ForceGravity(Vector3 * _gravity)
{
    this->gravity = _gravity;
}

ForceGravity::~ForceGravity()
{
    if (this->gravity) {
        delete this->gravity;
    }
    
    this->gravity = NULL;
}

void ForceGravity::updateGravity(real _value)
{
    this->gravity->x = _value;
}

void ForceGravity::updateForce(Particle * _particle, real _duration)
{
    if (!_particle->hasFiniteMass()) {
        return;
    }
    
    Vector3 * force = *this->gravity * _particle->getMass();

    _particle->addForce(force);
    
    delete force;
    force = NULL;
}


// ForceDrag
ForceDrag::ForceDrag(real _k1, real _k2)
{
    this->k1 = _k1;
    this->k2 = _k2;
}

ForceDrag::~ForceDrag()
{
    // TODO put your code here
    ForceGenerator::~ForceGenerator();
}

void ForceDrag::updateForce(Particle * _particle, real _duration)
{
    Vector3 * force = MakeVector3(_particle->getVelocity());
    
    // calculate the total drag coefficient.
    real dragCoeff = force->magnitude();
    dragCoeff = this->k1 * dragCoeff + this->k2 * dragCoeff * dragCoeff;
    
    // calculate the final force and apply it.
    force->normalize();
    *force *= -dragCoeff;
    
    _particle->addForce(force);
    
    delete force;
    force = NULL;
}


// ForceSpring
ForceSpring::ForceSpring(Particle * _other, real _springConstant, real _restLength)
{
    this->other = _other;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ForceSpring::~ForceSpring()
{
    // TODO put your code here
    ForceGenerator::~ForceGenerator();
}

void ForceSpring::updateForce(Particle * _particle, real _duration)
{
    // calculate the vector of the spring.
    Vector3 * force = MakeVector3(_particle->getPosition());
    *force -= this->other->getPosition();
    
    // calculate the magnitude of the force.
    real magnitude = force->magnitude();
    magnitude = real_abs(magnitude - this->restLength);
    magnitude *= this->springConstant;
    
    // calculate the final force and apply it.
    force->normalize();
    *force *= -magnitude;
    _particle->addForce(force);
    
    delete force;
    force = NULL;
}


// ForceAnchoredSpring
ForceAnchoredSpring::ForceAnchoredSpring(Vector3 * _anchor, real _springConstant, real _restLength)
{
    this->anchor = _anchor;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ForceAnchoredSpring::~ForceAnchoredSpring()
{
    if (this->anchor) {
        delete this->anchor;
    }
    this->anchor = NULL;
    
    // TODO put your code here
    ForceGenerator::~ForceGenerator();
}

void ForceAnchoredSpring::updateForce(Particle * _particle, real _duration)
{
    Vector3 * force = MakeVector3(_particle->getPosition());
    *force -= this->anchor;
    
    // calculate the magnitude of the force.
    real magnitude = force->magnitude();
    magnitude = real_abs(magnitude - this->restLength);
    magnitude *= this->springConstant;
    
    // calculate the final force and apply it.
    force->normalize();
    *force *= -magnitude;
    _particle->addForce(force);
    
    delete force;
    force = NULL;
}

void ForceAnchoredSpring::setAnchor(Vector3 * _anchor)
{
    this->anchor = _anchor;
}


// ForceBungee
ForceBungee::ForceBungee(Particle * _other, real _springConstant, real _restLength)
{
    this->other = _other;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ForceBungee::~ForceBungee()
{
    // TODO put your code here
    ForceGenerator::~ForceGenerator();
}

void ForceBungee::updateForce(Particle * _particle, real _duration)
{
    Vector3 * force = MakeVector3(_particle->getPosition());
    *force -= other->getPosition();
    
    // check if the bungee is compressed.
    real magnitude = force->magnitude();
    if (magnitude <= this->restLength) {
        delete force;
        force = NULL;
        return;
    }
    
    // calculate the magnitude of the force.
    magnitude = this->springConstant * (this->restLength - magnitude);
    
    // calculate the final force and apply it.
    force->normalize();
    *force *= -magnitude;
    _particle->addForce(force);
    
    delete force;
    force = NULL;
}


// ForceAnchoredBungee
ForceAnchoredBungee::ForceAnchoredBungee(Vector3 * _anchor, real _springConstant, real _restLength)
{
    this->anchor = _anchor;
    this->springConstant = _springConstant;
    this->restLength = _restLength;
}

ForceAnchoredBungee::~ForceAnchoredBungee()
{
    if (this->anchor) {
        delete this->anchor;
    }
    delete this->anchor;
    
    // TODO put your code here
    ForceGenerator::~ForceGenerator();
}

void ForceAnchoredBungee::updateForce(Particle * _particle, real _duration)
{
    Vector3 * force = MakeVector3(_particle->getPosition());
    *force -= this->anchor;
    
    // check if the bungee is compressed.
    real magnitude = force->magnitude();
    if (magnitude <= this->restLength) {
        delete force;
        force = NULL;
        return;
    }
    
    // calculate the magnitude of the force.
    magnitude = this->springConstant * (this->restLength - magnitude);
    
    // calculate the final force and apply it.
    force->normalize();
    *force *= -magnitude;
    _particle->addForce(force);

    delete force;
    force = NULL;
}

void ForceAnchoredBungee::setAnchor(Vector3 * _anchor)
{
    this->anchor = _anchor;
}


// ForceBuoyancy
ForceBuoyancy::ForceBuoyancy(real _maxDepth, real _volume, real _waterHeight, real _liquidDensity)
{
    this->maxDepth = _maxDepth;
    this->volume = _volume;
    this->waterHeight = _waterHeight;
    this->liquidDensity = _liquidDensity;
}

ForceBuoyancy::~ForceBuoyancy()
{
    // TODO put your code here
    ForceGenerator::~ForceGenerator();
}

void ForceBuoyancy::updateForce(Particle * _particle, real _duration)
{
    // calculate the submersion depth.
    real depth = _particle->getPosition()->y;
    
    // check if we’re out of the water.
    if (depth >= this->waterHeight + this->maxDepth) {
        return;
    }
    
    Vector3 * force = MakeVector3(0.0f, 0.0f, 0.0f);
    
    // check if we’re at maximum depth.
    if (depth <= this->waterHeight - this->maxDepth) {
        force->y = this->liquidDensity * this->volume;
        _particle->addForce(force);
        return;
    }
    
    // otherwise we are partly submerged.
    force->y = this->liquidDensity * this->volume * (depth - this->maxDepth - this->waterHeight) / 2 * this->maxDepth;
    _particle->addForce(force);
    
    delete force;
    force = NULL;
}


// ForceFakeStiffSpring
ForceFakeStiffSpring::ForceFakeStiffSpring(Vector3 * _anchor, real _springConstant, real _damping)
{
    this->anchor = _anchor;
    this->springConstant = _springConstant;
    this->damping = _damping;
}

ForceFakeStiffSpring::~ForceFakeStiffSpring()
{
    if (this->anchor) {
        delete this->anchor;
    }
    this->anchor = NULL;
    
    ForceGenerator::~ForceGenerator();
}

void ForceFakeStiffSpring::updateForce(Particle * _particle, real _duration)
{
    // check that we do not have infinite mass.
    if (!_particle->hasFiniteMass()) {
        return;
    }
    
    // calculate the relative position of the particle to the anchor.
    Vector3 * position = _particle->getPosition();
    *position -= anchor;
    
    // calculate the constants and check whether they are in bounds.
    real gamma = 0.5f * real_sqrt(4 * this->springConstant - this->damping*this->damping);
    
    if (gamma == 0.0f) {
        return;
    }
    Vector3 * c = *(*position * (this->damping / (2.0f * gamma))) + (*_particle->getVelocity() * (1.0f / gamma));
    
    // calculate the target position.
    Vector3 * target = *(*position * real_cos(gamma * _duration)) + (*c * real_sin(gamma * _duration));
    *target *= real_exp(-0.5f * _duration * this->damping);

    // calculate the resulting acceleration and therefore the force
    Vector3 * accel = *(*(*target - position) * (1.0f / _duration*_duration)) - (*_particle->getVelocity() * _duration);
    _particle->addForce(*accel * _particle->getMass());
}
