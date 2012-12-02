//
//  ParticleContact.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 20/08/12.
//
//

#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"

class ParticleContact {
private:
    
    void resolveVelocity(real _duration);
    void resolveInterpenetration(real _duration);
public:
    ParticleContact();
    ~ParticleContact();
    
    real calculateSeparatingVelocity() const;
    void resolve(real _duration);
    
    real penetration;
    Particle * particle[2];
    real restitution;
    Vector3 contactNormal;
};

class ParticleContactResolver {
protected:
    unsigned iterations;
    unsigned iterationsUsed;
public:
    ParticleContactResolver(unsigned _iterations);
    void setIterations(unsigned _iterations);
    void resolveContacts(ParticleContact * _particleContactArray[], unsigned _numContacts, real _duration);
};

#endif
