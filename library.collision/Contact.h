//
//  Contact.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 25/08/12.
//
//

#ifndef CONTACT_H
#define CONTACT_H

#include "Commons.h"

class Contact {
public:
    // first contact in the array. This is used so that the contact pointer (below)
    // can be incremented each time a contact is detected, while this pointer points to the first contact found.
//    Contact * contactArray;

    // position of the contact in world coordinates
    Vector3 * contactPoint;
    
    // direction of the contact in world coordinates
    Vector3 * contactNormal;
    
    // depth of penetration at the contact point
    real penetration;
    
    // bodies that are involved in the contact.
    // the second of these can be NULL, for contacts with the scenery.
    RigidBody * body[2];
    
    // lateral friction coefficient at the contact.
    real friction;
    
    // normal restitution coefficient at the contact.
    real restitution;
    
    // matrix orhonormal
    Matrix3 * contactToWorld;
    
    // closing velocity at the point of contact
    Vector3 * contactVelocity;
    
    // desired velocity change for contact resolved
    real desiredDeltaVelocity;
    
    // world space position of the contact point relative to centre of each body
    Vector3 * relativeContactPosition[2];
    
    Contact();
    ~Contact();
    
    Vector3 * calculateLocalVelocity(unsigned _bodyIndex, real _duration);
    void calculateDesiredDeltaVelocity(real _duration);
    void swapBodies();
    void calculateContactBasis();
    void calculateInternals(real _duration);
    Vector3 * calculateFrictionlessImpulse(Matrix3 * _inverseInertiaTensor[]);
    void applyPositionChange(Vector3 * _linearChange[2], Vector3 * _angularChange[2], real _penetration);
    void applyVelocityChange(Vector3 * _velocityChange[2], Vector3 * _rotationChange[2]);
};

class ContactResolver {
private:
    static ContactResolver * contactResolver;

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
    ContactResolver(real _positionEpsilon=0.01f, real _velocityEpsilon=0.01f);
    void solverContacts(std::vector<Contact *> * _contacts, real _duration);
    
    static ContactResolver * getInstance();
};

#endif
