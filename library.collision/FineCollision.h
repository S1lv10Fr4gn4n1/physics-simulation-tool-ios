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

class Primitive {
public:
    
    // the resultant transform of the primitive. This is
    // calculated by combining the offset of the primitive
    // with the transform of the rigid body.
    Matrix4 * transform;
    RigidBody * body;
    Matrix4 * offset;
    
    Primitive();
    ~Primitive();
    Vector3 * getAxis(unsigned _index) const;
    void calculateInternals();
};

class Sphere : public Primitive {
public:
    real radius;
    
    Sphere(RigidBody * _body, real _radius) {
        this->body = _body;
        this->radius = _radius;
    }
};

class Plane : public Primitive {
public:
    Vector3 * direction;
    real offset;
    
    Plane(RigidBody * _body, Vector3 * _direction, real _offset) {
        this->body = _body;
        this->direction = _direction;
        this->offset = _offset;
    }
};

class Box : public Primitive {
public:
    Vector3 * halfSize;
    
    Box(RigidBody * _body, Vector3 * _halfSize) {
        this->body = _body;
        this->halfSize = _halfSize;
    }
};

class CollisionDetector {
public:
    static unsigned sphereAndSphere(const Sphere * _one, const Sphere * _two, CollisionData * _data);
    static unsigned sphereAndHalfSpace(const Sphere * _sphere, const Plane * _plane, CollisionData * _data);
    static unsigned sphereAndTruePlane(const Sphere * _sphere, const Plane * _plane, CollisionData * _data);
    static unsigned boxAndSphere(const Box * _box, const Sphere * _sphere, CollisionData * _data);
    static unsigned boxAndPoint(const Box * _box, Vector3 * _point,  CollisionData * _data);
};

#endif
