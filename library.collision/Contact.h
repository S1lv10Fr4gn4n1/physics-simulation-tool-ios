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
    
    void matchAwakeState();
    Vector3 * calculateLocalVelocity(unsigned _bodyIndex, real _duration);
    void calculateDesiredDeltaVelocity(real _duration);
    void swapBodies();
    void calculateContactBasis();
    void calculateInternals(real _duration);
    Vector3 * calculateFrictionlessImpulse(Matrix3 * _inverseInertiaTensor[]);
    Vector3 * calculateFrictionImpulse(Matrix3 * _inverseInertiaTensor[]);
    void applyPositionChange(Vector3 * _linearChange[2], Vector3 * _angularChange[2], real _penetration);
    void applyVelocityChange(Vector3 * _velocityChange[2], Vector3 * _rotationChange[2]);
};
#endif
