//
//  ParticleLinks.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 20/08/12.
//
//

#ifndef PARTICLELINKS_H
#define PARTICLELINKS_H

#include "Particle.h"
#include "ParticleContact.h"

class ParticleContactGenerator {
public:
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const = 0;
};

class ParticleLink : public ParticleContactGenerator {
protected:
    real currentLength() const;
public:
    Particle * particle[2];
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const = 0;
};

class ParticleCable : public ParticleLink {
private:
    real maxLength;
    real restitution;
public:
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const;
};

class ParticleRod : public ParticleLink {
private:
public:
    real length;
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const;
};

class ParticleConstraint : public ParticleContactGenerator {
private:
protected:
    Particle * particle;
    Vector3 anchor;
    real currentLength() const;
public:
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const = 0;
};

class ParticleCableConstraint : public ParticleConstraint {
private:
    real maxLength;
    real restitution;
public:
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const;
};

class ParticleRodConstraint : public ParticleConstraint {
private:
    real length;
public:
    virtual unsigned addContact(ParticleContact * _contact, unsigned _limit) const;
};

#endif
