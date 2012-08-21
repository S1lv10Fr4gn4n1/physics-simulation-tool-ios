//
//  RigidBory.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#ifndef RIGIDBORY_H
#define RIGIDBORY_H

#include "CommonStructures.h"

class RigidBody {
private:
    real mass;
    real angularDamping;
    real linearDamping;
    
    real inverseMass;
    Vector3 * position;
    Quaternion * orientation;
    Vector3 * velocity;
    Vector3 * rotation;
    Vector3 * acceleration;
    Matrix4 * transformMatrix;
    Matrix3 * inverseInertiaTensor;
    Matrix3 * inverseInertiaTensorWorld;
    
    Vector3 * forceAccum;
    Vector3 * torqueAccum;
    Vector3 * lastFrameAcceleration;
    
    void clearAccumulators();
public:
    RigidBody();
    ~RigidBody();
    void calculateDerivedData();
    void addForce(const Vector3 * _force);
    void addForceAtBodyPoint(const Vector3 * _force, const Vector3 * _point);
    void addForceAtPoint(const Vector3 * _force, const Vector3 * _point);
    void addTorque(const Vector3 * _torque);
    
    void integrate(real _duration);
    void setInertiaTensor(const Matrix3 * _inertiaTensor);
    
    bool hasFiniteMass();
    real getMass();
    void setMass(real _mass);
    
    Vector3 * getPointInWorldSpace(const Vector3 * _point) const;
};
#endif
