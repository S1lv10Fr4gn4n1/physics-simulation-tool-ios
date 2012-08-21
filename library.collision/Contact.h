//
//  Contact.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 20/08/12.
//
//

#ifndef CONTACT_H
#define CONTACT_H

#include "Particle.h"

class Contact {
private:
    
    void resolveVelocity(real _duration);
    void resolveInterpenetration(real _duration);
public:
    Contact();
    ~Contact();
    
    real calculateSeparatingVelocity() const;
    void resolve(real _duration);
    
    real penetration;
    Particle * particle[2];
    real restitution;
    Vector3 * contactNormal;
};


class ContactResolver {
protected:
    unsigned iterations;
    unsigned iterationsUsed;
public:
    ContactResolver(unsigned _iterations);
    void setIterations(unsigned _iterations);
    void resolveContacts(Contact * _contactArray[], unsigned _numContacts, real _duration);
};

#endif
