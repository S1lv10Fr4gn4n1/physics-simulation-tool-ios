//
//  ParticleLinks.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 20/08/12.
//
//

#include "ParticleLinks.h"

real ParticleLink::currentLength() const
{
    Vector3 relativePos = this->particle[0]->getPosition() - this->particle[1]->getPosition();
    return relativePos.magnitude();
}

unsigned ParticleCable::addContact(ParticleContact * _contact, unsigned _limit) const
{
    // find the length of the cable.
    real length = this->currentLength();
    
    // check whether we’re overextended.
    if (length < this->maxLength) {
        return 0;
    }
    
    // otherwise return the contact.
    _contact->particle[0] = this->particle[0];
    _contact->particle[1] = this->particle[1];
    
    // calculate the normal.
    Vector3 normal = this->particle[1]->getPosition() - this->particle[0]->getPosition();
    normal.normalize();
    _contact->contactNormal = normal;
    _contact->penetration = length - this->maxLength;
    _contact->restitution = this->restitution;
    
    return 1;
}

unsigned ParticleRod::addContact(ParticleContact * _contact, unsigned _limit) const
{
    // dind the length of the rod.
    real currentLen = this->currentLength();
    
    // check whether we’re overextended.
    if (currentLen == this->length) {
        return 0;
    }
    
    // otherwise return the contact.
    _contact->particle[0] = this->particle[0];
    _contact->particle[1] = this->particle[1];
    
    // calculate the normal.
    Vector3 normal = this->particle[1]->getPosition() - this->particle[0]->getPosition();
    normal.normalize();
    
    // the contact normal depends on whether we’re extending or compressing.
    if (currentLen > this->length) {
        _contact->contactNormal = normal;
        _contact->penetration = currentLen - this->length;
    } else {
        _contact->contactNormal = normal * -1;
        _contact->penetration = this->length - currentLen;
    }
    
    // always use zero restitution (no bounciness).
    _contact->restitution = 0;
    
    return 1;
}

real ParticleConstraint::currentLength() const
{
    Vector3 relativePos = this->particle->getPosition() - this->anchor;
    return relativePos.magnitude();
}

unsigned ParticleCableConstraint::addContact(ParticleContact * _contact, unsigned _limit) const
{
    // find the length of the cable
    real length = this->currentLength();
    
    // check if we're over-extended
    if (length < this->maxLength) {
        return 0;
    }
    
    // otherwise return the contact
    _contact->particle[0] = this->particle;
    _contact->particle[1] = 0;
    
    // calculate the normal
    Vector3 normal = this->anchor - particle->getPosition();
    normal.normalize();
    _contact->contactNormal = normal;
    
    _contact->penetration = length - this->maxLength;
    _contact->restitution = this->restitution;
    
    return 1;
}

unsigned ParticleRodConstraint::addContact(ParticleContact * _contact, unsigned _limit) const
{
    // find the length of the rod
    real currentLen = this->currentLength();
    
    // check if we're over-extended
    if (currentLen == this->length) {
        return 0;
    }
    
    // otherwise return the contact
    _contact->particle[0] = this->particle;
    _contact->particle[1] = 0;
    
    // calculate the normal
    Vector3 normal = this->anchor - this->particle->getPosition();
    normal.normalize();
    
    // the contact normal depends on whether we're extending or compressing
    if (currentLen > this->length) {
        _contact->contactNormal = normal;
        _contact->penetration = currentLen - this->length;
    } else {
        _contact->contactNormal = normal * -1;
        _contact->penetration = this->length - currentLen;
    }
    
    // always use zero restitution (no bounciness)
    _contact->restitution = 0;
    
    return 1;
}
