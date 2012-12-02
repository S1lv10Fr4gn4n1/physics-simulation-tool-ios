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

//!Class responsible for maintaining the list of contacts generated
class CollisionData {
public:
    //!contacts list
    std::vector<Contact *> * contacts;
    
    CollisionData();
    ~CollisionData();
    
    void clearContacts();
};

//!Primitive class to maintain information on the type of object to identify collisions
class CollisionPrimitive {
public:
    //!calculated by combining the offset of the primitive with the transform of the rigid body
    Matrix4 transform;
    //!Rigid body collision
    RigidBody * body;
    //!matrix offset
    Matrix4 offset;

    Vector3 getAxis(unsigned _index) const;
    void calculateInternals();
};

//!Specialized class for collision of spherical objects
class CollisionSphere : public CollisionPrimitive {
public:
    //!radius of object
    real radius;
    CollisionSphere(RigidBody * _body, real _radius);
};

//!Specialized class for collision of plane objects
class CollisionPlane : public CollisionPrimitive {
public:
    //!normal of plane
    Vector3 normal;
    real offset;
    CollisionPlane(RigidBody * _body, const Vector3 &_normal, real _offset);
};

//!Specialized class for collision of box objects
class CollisionBox : public CollisionPrimitive {
public:
    //!half-size of of object
    Vector3 halfSize;
    CollisionBox(RigidBody * _body, const Vector3 &_halfSize);
};

//!Class responsible for identifying and generating a collision contact of said collision
class CollisionDetector {
public:
    static unsigned sphereAndSphere(const CollisionSphere * _one, const CollisionSphere * _two, CollisionData * _data);
    static unsigned sphereAndHalfSpace(const CollisionSphere * _sphere, const CollisionPlane * _plane, CollisionData * _data);
    static unsigned sphereAndTruePlane(const CollisionSphere * _sphere, const CollisionPlane * _plane, CollisionData * _data);
    static unsigned boxAndHalfSpace(const CollisionBox * _box, const CollisionPlane * _plane, CollisionData * _data);
    static unsigned boxAndBox(const CollisionBox * _one, const CollisionBox * _two, CollisionData * _data);
    static unsigned boxAndSphere(const CollisionBox * _box, const CollisionSphere * _sphere, CollisionData * _data);
    static unsigned boxAndPoint(const CollisionBox * _box, const Vector3 &_point,  CollisionData * _data);
};

#endif
