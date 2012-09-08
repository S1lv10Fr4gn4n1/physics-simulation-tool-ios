//
//  FineCollision.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 26/08/12.
//
//

#ifndef FINECOLLISION_H
#define FINECOLLISION_H

#include "Contact.h"

class CollisionData {
public:
    std::vector<Contact *> * contacts;
    unsigned contactsCount;
    unsigned contactsLeft;
    // restitution value to write into any collisions
    real restitution;
    // friction value to write into any collisions
    real friction;
    
    CollisionData();
    ~CollisionData();
    
    void clearContacts();
    void addContacts(unsigned _num);
    void reset(unsigned _maxContacts);
};

class CollisionPrimitive {
public:
    
    // the resultant transform of the primitive. This is
    // calculated by combining the offset of the primitive
    // with the transform of the rigid body.
    Matrix4 * transform;
    RigidBody * body;
    Matrix4 * offset;
    
    CollisionPrimitive();
    ~CollisionPrimitive();
    Vector3 * getAxis(unsigned _index) const;
    void calculateInternals();
};

class CollisionSphere : public CollisionPrimitive {
public:
    real radius;
    
    CollisionSphere(RigidBody * _body, real _radius);
};

class CollisionPlane : public CollisionPrimitive {
public:
    Vector3 * direction;
    real offset;
    
    CollisionPlane(RigidBody * _body, Vector3 * _direction, real _offset);
};

class CollisionBox : public CollisionPrimitive {
public:
    Vector3 * halfSize;
    
    CollisionBox(RigidBody * _body, Vector3 * _halfSize);
};

class CollisionDetector {
public:
    static unsigned sphereAndSphere(const CollisionSphere * _one, const CollisionSphere * _two, CollisionData * _data);
    static unsigned sphereAndHalfSpace(const CollisionSphere * _sphere, const CollisionPlane * _plane, CollisionData * _data);
    static unsigned sphereAndTruePlane(const CollisionSphere * _sphere, const CollisionPlane * _plane, CollisionData * _data);
    static unsigned boxAndHalfSpace(const CollisionBox * _box, const CollisionPlane * _plane, CollisionData * _data);
    static unsigned boxAndBox(const CollisionBox * _one, const CollisionBox * _two, CollisionData * _data);
    static unsigned boxAndSphere(const CollisionBox * _box, const CollisionSphere * _sphere, CollisionData * _data);
    static unsigned boxAndPoint(const CollisionBox * _box, Vector3 * _point,  CollisionData * _data);
};

#endif
