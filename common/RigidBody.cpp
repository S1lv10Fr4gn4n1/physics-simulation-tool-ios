//
//  RigidBody.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#include "RigidBody.h"

#include <stdio.h>

RigidBody::RigidBody()
{
    this->mass = 0.0f;
    this->inverseMass = 0.0f;
    this->angularDamping = 0.0f;
    this->linearDamping = 0.0f;
    this->radius = 0.0f;
    
    this->friction = 0.0f;
    this->restitution = 0.0f;
    
    this->motion = 0.0f;
}

RigidBody::~RigidBody()
{
    delete [] this->id;
    this->id = NULL;
}

void RigidBody::init()
{
    static int countBody = 0;

    if (this->typeObject == PLAN) {
        this->id = new char[8];
        sprintf(this->id, "plan:%03d",countBody++);
    } else if (this->typeObject == SPHERE) {
        this->id = new char[10];
        sprintf(this->id, "sphere:%03d",countBody++);
    } else if (this->typeObject == BOX) {
        this->id = new char[7];
        sprintf(this->id, "box:%03d",countBody++);
    }
}

char * RigidBody::getId()
{
    return this->id;
}

void RigidBody::setId(char *_id)
{
    this->id = _id;
}

//!Method responsible for managing data for the transformation matrices and inertia tensor
void RigidBody::calculateDerivedData()
{
    this->orientation.normalize();
    
    // calculate the transform matrix for the body
    calculateTransformMatrix(this->transformMatrix, this->position, this->orientation);
    
    // calculate the inertiaTensor in world space
    transformInertiaTensor(this->inverseInertiaTensorWorld,
                           this->orientation,
                           this->inverseInertiaTensor,
                           this->transformMatrix);
    
}

void RigidBody::setInertiaTensor(const Matrix3 &_inertiaTensor)
{
    this->inverseInertiaTensor.setInverse(_inertiaTensor);
}

void RigidBody::addForce(const Vector3 &_force, bool _awakeUp)
{
    this->forceAccum += _force;
    
    if (_awakeUp) {
        this->awake = true;
    }
}

void RigidBody::addVelocity(const Vector3 &_velocity, bool _awakeUp)
{
    this->velocity += _velocity;

    if (_awakeUp) {
        this->awake = true;
    }
}

void RigidBody::addRotation(const Vector3 &_rotation)
{
    this->rotation += _rotation;
}

void RigidBody::addForceAtBodyPoint(const Vector3 &_force, const Vector3 &_point)
{
    // convert to coordinates relative to the center of mass.
    Vector3 pt = this->getPointInWorldSpace(_point);
    this->addForceAtPoint(_force, pt);
}

void RigidBody::clearAccumulators()
{
    this->forceAccum.clear();
    this->torqueAccum.clear();
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3 &_point) const
{
    return this->transformMatrix.transform(_point);
}

void RigidBody::addForceAtPoint(const Vector3 &_force, const Vector3 &_point)
{
    // convert to coordinates relative to center of mass.
    Vector3 point(_point);
    point -= this->position;
    
    this->forceAccum += _force;
    this->torqueAccum += point % _force;
}

void RigidBody::addTorque(const Vector3 &_torque, bool _awakeUp)
{
    this->torqueAccum += _torque;
    
    if (_awakeUp) {
        this->awake = true;
    }
}

bool RigidBody::hasFiniteMass()
{
    return this->mass > 0.0f;
}

real RigidBody::getMass()
{
    return this->mass;
}

void RigidBody::setMass(real _mass)
{
    if (_mass <= 0.0f) {
        this->mass = 0.0f;
        this->inverseMass = 0.0f;
    } else {
        this->inverseMass = ((real)1.0)/_mass; // TODO attention
        this->mass = _mass;
    }
}

real RigidBody::getInverseMass()
{
    return this->inverseMass;
}

Vector3 RigidBody::getPosition()
{
    return this->position;
}

void RigidBody::setPosition(const Vector3 &_position)
{
    this->position.x = _position.x;
    this->position.y = _position.y;
    this->position.z = _position.z;
}

void RigidBody::setPosition(real _x, real _y, real _z)
{
    this->position.x = _x;
    this->position.y = _y;
    this->position.z = _z;
}

void RigidBody::setPosition(real _x, real _y)
{
    this->position.x = _x;
    this->position.y = _y;
}

Vector3 RigidBody::getAcceleration()
{
    return this->acceleration;
}

void RigidBody::setAcceleration(const Vector3 &_acceleration)
{
    this->acceleration.x = _acceleration.x;
    this->acceleration.y = _acceleration.y;
    this->acceleration.z = _acceleration.z;
}

void RigidBody::setAcceleration(real _x, real _y, real _z)
{
    this->acceleration.x = _x;
    this->acceleration.y = _y;
    this->acceleration.z = _z;
}

void RigidBody::setAcceleration(real _x, real _y)
{
    this->acceleration.x = _x;
    this->acceleration.y = _y;
}

Vector3 RigidBody::getVelocity()
{
    return this->velocity;
}

void RigidBody::setVelocity(const Vector3 &_velocity)
{
    this->velocity.x = _velocity.x;
    this->velocity.y = _velocity.y;
    this->velocity.z = _velocity.z;
}

void RigidBody::setVelocity(real _x, real _y, real _z)
{
    this->velocity.x = _x;
    this->velocity.y = _y;
    this->velocity.z = _z;
}

void RigidBody::setVelocity(real _x, real _y)
{
    this->velocity.x = _x;
    this->velocity.y = _y;
}

Vector3 RigidBody::getForceAccum()
{
    return this->forceAccum;
}

Vector3 RigidBody::getTorqueAccum()
{
    return this->torqueAccum;
}

real RigidBody::getAngularDamping()
{
    return this->angularDamping;
}

void RigidBody::setAngularDamping(real _angularDamping)
{
    this->angularDamping = _angularDamping;
}

real RigidBody::getLinearDamping()
{
    return this->linearDamping;
}

void RigidBody::setLinearDamping(real _linearDamping)
{
    this->linearDamping = _linearDamping;
}

Vector3 RigidBody::getLastFrameAcceleration()
{
    return this->lastFrameAcceleration;
}

void RigidBody::setLastFrameAcceleration(const Vector3 &_lastFrameAcceleration)
{
    this->lastFrameAcceleration.x = _lastFrameAcceleration.x;
    this->lastFrameAcceleration.y = _lastFrameAcceleration.y;
    this->lastFrameAcceleration.z = _lastFrameAcceleration.z;
}

void RigidBody::setLastFrameAcceleration(real _x, real _y, real _z)
{
    this->lastFrameAcceleration.x = _x;
    this->lastFrameAcceleration.y = _y;
    this->lastFrameAcceleration.z = _z;
}

void RigidBody::setLastFrameAcceleration(real _x, real _y)
{
    this->lastFrameAcceleration.x = _x;
    this->lastFrameAcceleration.y = _y;
}

Matrix3 RigidBody::getInverseInertiaTensorWorld()
{
    return this->inverseInertiaTensorWorld;
}

void RigidBody::setInverseInertiaTensorWorld(const Matrix3 &_inverseInertiaTensorWorld)
{
    this->inverseInertiaTensorWorld = _inverseInertiaTensorWorld;
}

Vector3 RigidBody::getRotation()
{
    return this->rotation;
}

void RigidBody::setRotation(const Vector3 &_rotation)
{
    this->rotation.x = _rotation.x;
    this->rotation.y = _rotation.y;
    this->rotation.z = _rotation.z;
}

void RigidBody::setRotation(real _x, real _y, real _z)
{
    this->rotation.x = _x;
    this->rotation.y = _y;
    this->rotation.z = _z;
}

void RigidBody::setRotation(real _x, real _y)
{
    this->rotation.x = _x;
    this->rotation.y = _y;
}

real RigidBody::getRadius()
{
    return this->radius;
}

void RigidBody::setRadius(real _radius)
{
    this->radius = _radius;
}


Quaternion RigidBody::getOrientation()
{
    return this->orientation;
}

void RigidBody::setOrientation(const Quaternion &_orientation)
{
    this->orientation.r = _orientation.r;
    this->orientation.i = _orientation.i;
    this->orientation.j = _orientation.j;
    this->orientation.k = _orientation.k;
}

void RigidBody::setOrientation(real _r, real _i, real _j, real _k)
{
    this->orientation.r = _r;
    this->orientation.i = _i;
    this->orientation.j = _j;
    this->orientation.k = _k;
}

TypeObject RigidBody::getTypeObject()
{
    return this->typeObject;
}

void RigidBody::setTypeObject(TypeObject _typeObject)
{
    this->typeObject = _typeObject;
}

Matrix4 RigidBody::getTransformMatrix()
{
    return this->transformMatrix;
}

void RigidBody::setTransformMatrixIndex(unsigned _index, real _value)
{
    this->transformMatrix.data[_index] = _value;
}

Matrix4x4 RigidBody::getGLTransform()
{
    Matrix4x4 matrix;
    matrix.data[0] = this->transformMatrix.data[0];
    matrix.data[1] = this->transformMatrix.data[4];
    matrix.data[2] = this->transformMatrix.data[8];
    matrix.data[3] = 0;
    
    matrix.data[4] = this->transformMatrix.data[1];
    matrix.data[5] = this->transformMatrix.data[5];
    matrix.data[6] = this->transformMatrix.data[9];
    matrix.data[7] = 0;
    
    matrix.data[8] = this->transformMatrix.data[2];
    matrix.data[9] = this->transformMatrix.data[6];
    matrix.data[10] = this->transformMatrix.data[10];
    matrix.data[11] = 0;
    
    matrix.data[12] = this->transformMatrix.data[3];
    matrix.data[13] = this->transformMatrix.data[7];
    matrix.data[14] = this->transformMatrix.data[11];
    matrix.data[15] = 1;
    
    return matrix;
}

Vector3 RigidBody::getHalfSize()
{
    return this->halfSize;
}

void RigidBody::setHalfSize(const Vector3 &_halfSize)
{
    this->halfSize.x = _halfSize.x;
    this->halfSize.y = _halfSize.y;
    this->halfSize.z = _halfSize.z;
}

void RigidBody::setHalfSize(real _x, real _y, real _z)
{
    this->halfSize.x = _x;
    this->halfSize.y = _y;
    this->halfSize.z = _z;
}

void RigidBody::setHalfSize(real _x, real _y)
{
    this->halfSize.x = _x;
    this->halfSize.y = _y;
}

real RigidBody::getMotion()
{
    return this->motion;
}

void RigidBody::setMotion(real _motion)
{
    this->motion = _motion;
}

bool RigidBody::isCanSleep()
{
    return this->canSleep;
}

void RigidBody::setCanSleep(bool _canSleep)
{
    this->canSleep = _canSleep;
    
    if (!this->canSleep && !this->awake) {
        this->setAwake();
    }
}

bool RigidBody::isAwake()
{
    return this->awake;
}

void RigidBody::setAwake(bool _awake)
{
    if (_awake) {
        this->awake = true;
        // add a bit of motion to avoid it falling asleep immediately
        this->motion = SLEEP_EPSILON*1.5f;
    } else {
        this->awake = false;
        this->velocity.clear();
        this->rotation.clear();
    }
}

real RigidBody::getRestitution()
{
    return this->restitution;
}

void RigidBody::setRestitution(real _restitution)
{
    this->restitution = _restitution;
}

real RigidBody::getFriction()
{
    return this->friction;
}

void RigidBody::setFriction(real _friction)
{
    this->friction = _friction;
}

Vector3 RigidBody::getAccelerationGravity()
{
    return this->accelerationGravity;
}

void RigidBody::setAccelerationGravity(const Vector3 &_gravity)
{
    this->accelerationGravity.x = _gravity.x;
    this->accelerationGravity.y = _gravity.y;
    this->accelerationGravity.z = _gravity.z;
}

void RigidBody::setAccelerationGravity(real _x, real _y, real _z)
{
    this->accelerationGravity.x = _x;
    this->accelerationGravity.y = _y;
    this->accelerationGravity.z = _z;
}
