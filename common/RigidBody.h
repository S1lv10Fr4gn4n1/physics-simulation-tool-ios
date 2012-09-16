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
    char * id;
    bool dirty;
    
    real motion;
    bool awake;
    bool canSleep;
    
    real restitution;
    real friction;

    real mass;
    real angularDamping;
    real linearDamping;
    real volume;
    real density;
    real inverseMass;
    real radius;
    Vector3 * halfSize;
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
    
    TypeObject typeObject;
public:
    RigidBody();
    ~RigidBody();
    void calculateDerivedData();
    void addForce(const Vector3 * _force, bool _awakeUp=false);
    void addForceAtBodyPoint(const Vector3 * _force, const Vector3 * _point);
    void addForceAtPoint(const Vector3 * _force, const Vector3 * _point);
    void addTorque(const Vector3 * _torque, bool _awakeUp=false);
    void addVelocity(const Vector3 * _velocity, bool _awakeUp=false);
    void addRotation(const Vector3 * _rotation);
    
    void setInertiaTensor(const Matrix3 * _inertiaTensor);
    
    Vector3 * getPointInWorldSpace(const Vector3 * _point) const;

    void clearAccumulators();
    
    bool hasFiniteMass();
    real getMass();
    void setMass(real _mass);
    real getVolume();
    void setVolume(real _volume);
    real getDensity();
    void setDensity(real _density);
    real getInverseMass();
    Vector3 * getPosition();
    void setPosition(Vector3 * _position);
    void setPosition(real _x, real _y, real _z);
    void setPosition(real _x, real _y);
    Vector3 * getAcceleration();
    void setAcceleration(Vector3 * _acceleration);
    void setAcceleration(real _x, real _y, real _z);
    void setAcceleration(real _x, real _y);
    Vector3 * getVelocity();
    void setVelocity(Vector3 * _velocity);
    void setVelocity(real _x, real _y, real _z);
    void setVelocity(real _x, real _y);
    Vector3 * getForceAccum();
    Vector3 * getTorqueAccum();
    real getAngularDamping();
    void setAngularDamping(real _angularDamping);
    real getLinearDamping();
    void setLinearDamping(real _linearDamping);
    Vector3 * getLastFrameAcceleration();
    void setLastFrameAcceleration(Vector3 * _lastFrameAcceleration);
    void setLastFrameAcceleration(real _x, real _y, real _z);
    void setLastFrameAcceleration(real _x, real _y);
    Matrix3 * getInverseInertiaTensorWorld();
    void setInverseInertiaTensorWorld(Matrix3 * _inverseInertiaTensorWorld);
    Vector3 * getRotation();
    void setRotation(Vector3 * _rotation);
    void setRotation(real _x, real _y, real _z);
    void setRotation(real _x, real _y);
    real getRadius();
    void setRadius(real _radius);
    Quaternion * getOrientation();
    void setOrientation(Quaternion * _orientation);
    void setOrientation(real _r, real _i, real _j, real _k);
    TypeObject getTypeObject();
    void setTypeObject(TypeObject _typeObject);
    Matrix4 * getTransformMatrix();
    void getGLTransform(float matrix[16]) const;
    Vector3 * getHalfSize();
    void setHalfSize(Vector3 * _halfSize);
    void setHalfSize(real _x, real _y, real _z);
    void setHalfSize(real _x, real _y);
    char * getId();
    bool isDirty();
    void setDirty(bool _dirty);
    real getMotion();
    void setMotion(real _motion);
    bool isCanSleep();
    void setCanSleep(bool _canSleep=true);
    bool isAwake();
    void setAwake(bool _awake=true);
    real getRestitution();
    void setRestitution(real _restitution);
    real getFriction();
    void setFriction(real _friction);
};
#endif
