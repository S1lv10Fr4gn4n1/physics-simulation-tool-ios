//
//  ContactResolver.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 09/09/12.

#ifndef CONTACTRESOLVER_H
#define CONTACTRESOLVER_H

#include "CommonStructures.h"
#include "Contact.h"

//!Class responsible for resolving contacts
class ContactResolver {
private:
    //!avoid instability velocities smaller
    real velocityEpsilon;
    
    //!avoid instability penetrations smaller
    real positionEpsilon;
    
    void prepareContacts(std::vector<Contact *> * _contacts, real _duration);
    void solverPositions(std::vector<Contact *> * _contacts, real _duration);
    void solverVelocities(std::vector<Contact *> * _contacts, real _duration);
public:
    ContactResolver(real _positionEpsilon=0.001f, real _velocityEpsilon=0.01f);
    void solverContacts(std::vector<Contact *> * _contacts, real _duration);
};

#endif
