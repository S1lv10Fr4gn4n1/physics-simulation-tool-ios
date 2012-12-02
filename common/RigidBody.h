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

//!Class responsible for keeping information of a rigid body
class RigidBody {
private:
    //!unique identifier of the object
    char * id;
    //!keeps the amount of kinetic energy
    real motion;
    //!check if object is awake
    bool awake;
    //!indicates that the object can sleep
    bool canSleep;
    //!coefficient of restitution
    real restitution;
    //!coefficient of friction
    real friction;
    //!mass of the object
    real mass;
    //!inverse of the mass of the object
    real inverseMass;
    //!drag coefficient angular
    real angularDamping;
    //!drag coefficient linear
    real linearDamping;
    //!radius of the object
    real radius;
    //!information on the thickness of the object
    Vector3 halfSize;
    Vector3 position;
    Quaternion orientation;
    Vector3 velocity;
    Vector3 rotation;
    Vector3 acceleration;
    Matrix4 transformMatrix;
    Matrix3 inverseInertiaTensor;
    Matrix3 inverseInertiaTensorWorld;
    //!forces accumulated by the principle of D'Alembert
    Vector3 forceAccum;
    //!torque accumulated by the principle of D'Alembert
    Vector3 torqueAccum;
    Vector3 lastFrameAcceleration;
    Vector3 accelerationGravity;

    TypeObject typeObject;
protected:
    void setId(char * _id);
public:
    RigidBody();
    ~RigidBody();

    void init();
    
    void calculateDerivedData();
    void addForce(const Vector3 &_force, bool _awakeUp=false);
    void addForceAtBodyPoint(const Vector3 &_force, const Vector3 &_point);
    void addForceAtPoint(const Vector3 &_force, const Vector3 &_point);
    void addTorque(const Vector3 &_torque, bool _awakeUp=false);
    void addVelocity(const Vector3 &_velocity, bool _awakeUp=false);
    void addRotation(const Vector3 &_rotation);
    void setInertiaTensor(const Matrix3 &_inertiaTensor);
    Vector3 getPointInWorldSpace(const Vector3 &_point) const;
    void clearAccumulators();
    bool hasFiniteMass();

    real getMass();
    void setMass(real _mass);
    real getInverseMass();
    Vector3 getPosition();
    void setPosition(const Vector3 &_position);
    void setPosition(real _x, real _y, real _z);
    void setPosition(real _x, real _y);
    Vector3 getAcceleration();
    void setAcceleration(const Vector3 &_acceleration);
    void setAcceleration(real _x, real _y, real _z);
    void setAcceleration(real _x, real _y);
    Vector3 getVelocity();
    void setVelocity(const Vector3 &_velocity);
    void setVelocity(real _x, real _y, real _z);
    void setVelocity(real _x, real _y);
    Vector3 getForceAccum();
    Vector3 getTorqueAccum();
    real getAngularDamping();
    void setAngularDamping(real _angularDamping);
    real getLinearDamping();
    void setLinearDamping(real _linearDamping);
    Vector3 getLastFrameAcceleration();
    void setLastFrameAcceleration(const Vector3 &_lastFrameAcceleration);
    void setLastFrameAcceleration(real _x, real _y, real _z);
    void setLastFrameAcceleration(real _x, real _y);
    Matrix3 getInverseInertiaTensorWorld();
    void setInverseInertiaTensorWorld(const Matrix3 &_inverseInertiaTensorWorld);
    Vector3 getRotation();
    void setRotation(const Vector3 &_rotation);
    void setRotation(real _x, real _y, real _z);
    void setRotation(real _x, real _y);
    real getRadius();
    void setRadius(real _radius);
    Quaternion getOrientation();
    void setOrientation(const Quaternion &_orientation);
    void setOrientation(real _r, real _i, real _j, real _k);
    TypeObject getTypeObject();
    void setTypeObject(TypeObject _typeObject);
    Matrix4 getTransformMatrix();
    void setTransformMatrixIndex(unsigned _index, real _value);
    Matrix4x4 getGLTransform();
    Vector3 getHalfSize();
    void setHalfSize(const Vector3 &_halfSize);
    void setHalfSize(real _x, real _y, real _z);
    void setHalfSize(real _x, real _y);
    char * getId();
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
    Vector3 getAccelerationGravity();
    void setAccelerationGravity(const Vector3 &_gravity);
    void setAccelerationGravity(real _x, real _y, real _z);
};
#endif
