//
//  ParticleContact.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 20/08/12.
//
//

#include "ParticleContact.h"


ParticleContact::ParticleContact()
{
    // TODO put your code here
}

ParticleContact::~ParticleContact()
{
    // TODO put your code here
}

void ParticleContact::resolve(real _duration)
{
    this->resolveVelocity(_duration);
    this->resolveInterpenetration(_duration);
}

real ParticleContact::calculateSeparatingVelocity() const
{
    Vector3 relativeVelocity = this->particle[0]->getVelocity();
    
    if (this->particle[1]) {
        relativeVelocity -= this->particle[1]->getVelocity();
    }
    
    return relativeVelocity * this->contactNormal;
}

void ParticleContact::resolveVelocity(real _duration)
{
    // find the velocity in the direction of the contact.
    real separatingVelocity = this->calculateSeparatingVelocity();
    
    // check whether it needs to be resolved.
    if (separatingVelocity > 0) {
        // the contact is either separating or stationary - there’s no impulse required
        return;
    }
    
    // calculate the new separating velocity.
    real newSepVelocity = -separatingVelocity * this->restitution;

    // adjustment for RESTING CONTACTS
    // check the velocity build-up due to acceleration only.
    Vector3 accCausedVelocity = this->particle[0]->getAcceleration();
    
    if (particle[1]) {
        accCausedVelocity -= this->particle[1]->getAcceleration();
    }
    
    real accCausedSepVelocity = accCausedVelocity * this->contactNormal * _duration;
    
    // if we’ve got a closing velocity due to acceleration build-up, // remove it from the new separating velocity.
    if (accCausedSepVelocity < 0) {
        newSepVelocity += restitution * accCausedSepVelocity;
        
        // make sure we haven’t removed more than was there to remove.
        if (newSepVelocity < 0) {
            newSepVelocity = 0;
        }
    }

    //adjustment
    real deltaVelocity = newSepVelocity - separatingVelocity;
    
    // we apply the change in velocity to each object in proportion to
    // its inverse mass (i.e., those with lower inverse mass [higher
    // actual mass] get less change in velocity).
    real totalInverseMass = this->particle[0]->getInverseMass();
    
    if (this->particle[1]) {
        totalInverseMass += particle[1]->getInverseMass();
    }
    
    // if all particles have infinite mass, then impulses have no effect.
    if (totalInverseMass <= 0) {
        return;
    }
    
    // calculate the impulse to apply.
    real impulse = deltaVelocity / totalInverseMass;
    
    // find the amount of impulse per unit of inverse mass.
    Vector3 impulsePerIMass = contactNormal * impulse;
    
    // apply impulses: they are applied in the direction of the contact,
    // and are proportional to the inverse mass.
    this->particle[0]->setVelocity(this->particle[0]->getVelocity() + impulsePerIMass * this->particle[0]->getInverseMass());
    
    if (this->particle[1]) {
        // particle 1 goes in the opposite direction.
        this->particle[1]->setVelocity(this->particle[1]->getVelocity() + impulsePerIMass * -this->particle[1]->getInverseMass());
    }
}

void ParticleContact::resolveInterpenetration(real _duration)
{
    // if we don’t have any penetration, skip this step.
    if (this->penetration <= 0) {
        return;
    }
    
    // the movement of each object is based on its inverse mass, total that.
    real totalInverseMass = this->particle[0]->getInverseMass();
    
    if (this->particle[1]) {
        totalInverseMass += this->particle[1]->getInverseMass();
    }
    
    // if all particles have infinite mass, then we do nothing.
    if (totalInverseMass <= 0) {
        return;
    }
    
    // find the amount of penetration resolution per unit of inverse mass.
    Vector3 movePerIMass = this->contactNormal * (-this->penetration / totalInverseMass);
    
    // apply the penetration resolution.
    this->particle[0]->setPosition(this->particle[0]->getPosition() + (movePerIMass * this->particle[0]->getInverseMass()));
    
    if (this->particle[1]) {
        this->particle[1]->setPosition(this->particle[1]->getPosition() + (movePerIMass * this->particle[1]->getInverseMass()));
    }
}


ParticleContactResolver::ParticleContactResolver(unsigned _iterations)
{
    this->iterations = _iterations;
}

void ParticleContactResolver::setIterations(unsigned _iterations)
{
    this->iterations = _iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact * _particleContactArray[], unsigned _numContacts, real _duration)
{
    this->iterationsUsed = 0;

    while (this->iterationsUsed < this->iterations) {
        // dind the contact with the largest closing velocity;
        real max = 0;
        unsigned maxIndex = _numContacts;
        
        for (unsigned i=0; i<_numContacts; i++) {
            real sepVel = _particleContactArray[i]->calculateSeparatingVelocity();
            if (sepVel < max) {
                max = sepVel;
                maxIndex = i;
            }
        }
        
        // resolve this contact.
        _particleContactArray[maxIndex]->resolve(_duration);
        this->iterationsUsed++;
    }
}