//
//  ContactResolver.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 09/09/12.
//
//

#ifndef CONTACTRESOLVER_H
#define CONTACTRESOLVER_H

#include "CommonStructures.h"
#include "Contact.h"

class ContactResolver {
private:
    // stores the number of position iterations used in the last call to resolve contacts
    unsigned positionIterationsUsed;
    
    // number of iterations to perform when resolving position
    unsigned positionIterations;
    
    // stores the number of velocity iterations used in the last call to resolve contacts
    unsigned velocityIterationsUsed;
    
    // number of iterations to perform when resolving velocity
    unsigned velocityIterations;
    
    // avoid instability velocities smaller
    real velocityEpsilon;
    
    // avoid instability penetrations smaller than this value are considered to be not interpenetrating
    real positionEpsilon;
    
    void prepareContacts(std::vector<Contact *> * _contacts, real _duration);
    void solverPositions(std::vector<Contact *> * _contacts, real _duration);
    void solverVelocities(std::vector<Contact *> * _contacts, real _duration);
public:
    ContactResolver(real _positionEpsilon=0.001f, real _velocityEpsilon=0.01f);
    void solverContacts(std::vector<Contact *> * _contacts, real _duration);
};

#endif
