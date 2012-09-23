//
//  ContactResolver.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 09/09/12.
//
//

#include "ContactResolver.h"

ContactResolver::ContactResolver(real _positionEpsilon, real _velocityEpsilon)
{
    this->positionEpsilon = _positionEpsilon;
    this->velocityEpsilon = _velocityEpsilon;
}

void ContactResolver::solverContacts(std::vector<Contact *> * _contacts, real _duration)
{
    if (_contacts->size() == 0) {
        return;
    }
    
    if (this->velocityIterations <= 0.0f && this->positionIterations <= 0.0f &&
        this->positionEpsilon <= 0.0f && this->velocityEpsilon <= 0.0f) {
        return;
    }

    this->prepareContacts(_contacts, _duration);
    
    // solver interpenetration
    this->solverPositions(_contacts, _duration);
    
    // solver velocity
    this->solverVelocities(_contacts, _duration);
}

void ContactResolver::prepareContacts(std::vector<Contact *> * _contacts, real _duration)
{
    // generate contact velocity and axis information.
    for (int i=0; i<_contacts->size(); i++) {
        _contacts->at(i)->calculateInternals(_duration);
    }
}

void ContactResolver::solverPositions(std::vector<Contact *> * _contacts, real _duration)
{
    unsigned index;
    Vector3 velocityChange[2];
    Vector3 rotationChange[2];
    real max;
    Vector3 contactPoint;
    
    this->positionIterationsUsed = 0;
    this->positionIterations = _contacts->size() * 8;
    
    // resolve interpenetration in order of severity.
    while (this->positionIterationsUsed < this->positionIterations) {
        max = this->positionEpsilon;
        index = _contacts->size();
        
        for (unsigned i=0; i<_contacts->size(); i++) {
            if (_contacts->at(i)->penetration > max) {
                max = _contacts->at(i)->penetration;
                index = i;
            }
        }
        
        if (index == _contacts->size()) {
            break;
        }
        
        _contacts->at(index)->matchAwakeState();
        
        _contacts->at(index)->applyPositionChange(velocityChange, rotationChange, max);


        // again this action may have changed the penetration of other bodies, so we update contacts
        for (unsigned i=0; i<_contacts->size(); i++) {

            // check each body in the contact
            for (unsigned b = 0; b < 2; b++) {
                if (_contacts->at(i)->body[b]) {
                    // check for a match with each body in the newly resolved contact
                    for (unsigned d = 0; d < 2; d++) {
                        if (_contacts->at(i)->body[b] == _contacts->at(index)->body[d]) {
                            contactPoint = velocityChange[d] + rotationChange[d].vectorProduct(_contacts->at(i)->relativeContactPosition[b]);
                            _contacts->at(i)->penetration += contactPoint.scalarProduct(_contacts->at(i)->contactNormal) * (b ? 1 : -1);
                        }
                    }
                }
            }
        }

        this->positionIterationsUsed++;
    }
}

void ContactResolver::solverVelocities(std::vector<Contact *> * _contacts, real _duration)
{
    Vector3 velocityChange[2];
    Vector3 rotationChange[2];
    Vector3 deltaVel;
    
    this->velocityIterationsUsed = 0;
    this->velocityIterations = _contacts->size() * 8;
    
    // iteratively handle impacts in order of severity.
    while (this->velocityIterationsUsed < this->velocityIterations) {
        // find contact with maximum magnitude of probable velocity change.
        real max = this->velocityEpsilon;
        unsigned index = _contacts->size();
        
        for (unsigned i=0; i < _contacts->size(); i++) {
            if (_contacts->at(i)->desiredDeltaVelocity > max) {
                max = _contacts->at(i)->desiredDeltaVelocity;
                index = i;
            }
        }
        
        if (index == _contacts->size()) {
            break;
        }
        
        // match the awake state at the contact
        _contacts->at(index)->matchAwakeState();
        
        // do the resolution on the contact that came out top.
        _contacts->at(index)->applyVelocityChange(velocityChange, rotationChange);
        
        // with the change in velocity of the two bodies, the update of
        // contact velocities means that some of the relative closing
        // velocities need recomputing.
        for (unsigned i=0; i<_contacts->size(); i++) {
            // check each body in the contact
            for (unsigned b = 0; b < 2; b++) {
                if (_contacts->at(i)->body[b]) {
                    // check for a match with each body in the newly resolved contact
                    for (unsigned d=0; d < 2; d++) {
                        if (_contacts->at(i)->body[b] == _contacts->at(index)->body[d]) {
                            deltaVel = velocityChange[d] + rotationChange[d].vectorProduct(_contacts->at(i)->relativeContactPosition[b]);
                            
                            // the sign of the change is negative if we're dealing
                            // with the second body in a contact.
                            _contacts->at(i)->contactVelocity += _contacts->at(i)->contactToWorld.transformTranspose(deltaVel) * (b ? -1 : 1);
                            _contacts->at(i)->calculateDesiredDeltaVelocity(_duration);
                        }
                    }
                }
            }
        	this->velocityIterationsUsed++;
        }
    }
}
