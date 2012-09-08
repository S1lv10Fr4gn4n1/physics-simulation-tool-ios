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
    static int countBody = 0;
    this->id = new char[15];
    sprintf(this->id, "objecto:%d",countBody++);
    
    this->mass = 0.0f;
    this->inverseMass = 0.0f;
    this->angularDamping = 0.0f;
    this->linearDamping = 0.0f;
    this->volume = 0.0f;
    this->density = 0.0f;
    this->radius = 0.0f;
    
    this->position = new Vector3();
    this->acceleration = new Vector3();
    this->lastFrameAcceleration = new Vector3();
    this->velocity = new Vector3();
    this->orientation = new Quaternion();
    this->rotation = new Vector3();
    this->forceAccum = new Vector3();
    this->torqueAccum = new Vector3();
    this->halfSize = new Vector3();
    
    this->transformMatrix = new Matrix4();
    this->inverseInertiaTensor = new Matrix3();
    this->inverseInertiaTensorWorld = new Matrix3();
}

RigidBody::~RigidBody()
{
    if (this->position) {
        delete this->position;
    }
    if (this->orientation) {
        delete this->orientation;
    }
    if (this->velocity) {
        delete this->velocity;
    }
    if (this->rotation) {
        delete this->rotation;
    }
    if (this->acceleration) {
        delete this->acceleration;
    }
    if (this->transformMatrix) {
        delete this->transformMatrix;
    }
    if (this->inverseInertiaTensor) {
        delete this->inverseInertiaTensor;
    }
    if (this->inverseInertiaTensorWorld) {
        delete this->inverseInertiaTensorWorld;
    }
    if (this->forceAccum) {
        delete this->forceAccum;
    }
    if (this->torqueAccum) {
        delete this->torqueAccum;
    }
    if (this->lastFrameAcceleration) {
        delete this->lastFrameAcceleration;
    }
    if (this->halfSize) {
        delete this->halfSize;
    }
    
    this->position = NULL;
    this->orientation = NULL;
    this->velocity = NULL;
    this->rotation = NULL;
    this->acceleration = NULL;
    this->transformMatrix = NULL;
    this->inverseInertiaTensor = NULL;
    this->inverseInertiaTensorWorld = NULL;
    this->forceAccum = NULL;
    this->torqueAccum = NULL;
    this->lastFrameAcceleration = NULL;
    this->halfSize = NULL;
}

char * RigidBody::getId()
{
    return this->id;
}

void RigidBody::calculateDerivedData()
{
    this->orientation->normalize();
    
    // calculate the transform matrix for the body
    _calculateTransformMatrix(this->transformMatrix, this->position, this->orientation);
    
    // calculate the inertiaTensor in world space
    _transformInertiaTensor(inverseInertiaTensorWorld,
                            this->orientation,
                            this->inverseInertiaTensor,
                            this->transformMatrix);
    
}

void RigidBody::setInertiaTensor(const Matrix3 * _inertiaTensor)
{
    this->inverseInertiaTensor->setInverse(_inertiaTensor);
}

void RigidBody::addForce(const Vector3 * _force)
{
    *this->forceAccum += _force;
    this->awake = true;
}

void RigidBody::addVelocity(const Vector3 * _velocity)
{
    *this->velocity += _velocity;
    this->awake = true;
}

void RigidBody::addRotation(const Vector3 * _rotation)
{
    *this->rotation += _rotation;
}

void RigidBody::addForceAtBodyPoint(const Vector3 * _force, const Vector3 * _point)
{
    // convert to coordinates relative to the center of mass.
    Vector3 * pt = this->getPointInWorldSpace(_point);
    this->addForceAtPoint(_force, pt);
}

void RigidBody::clearAccumulators()
{
    this->forceAccum->clear();
    this->torqueAccum->clear();
}

Vector3 * RigidBody::getPointInWorldSpace(const Vector3 * _point) const
{
    return this->transformMatrix->transform(_point);
}

void RigidBody::addForceAtPoint(const Vector3 * _force, const Vector3 * _point)
{
    // convert to coordinates relative to center of mass.
    Vector3 * point = new Vector3(_point);
    *point -= this->position;
    
    *this->forceAccum += _force;
    *this->torqueAccum += *point % _force;
    
    delete point;
    point = NULL;
}

void RigidBody::addTorque(const Vector3 * _torque)
{
    *this->torqueAccum += _torque;
    this->awake = true;
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

real RigidBody::getVolume()
{
    return this->volume;
}

void RigidBody::setVolume(real _volume)
{
    this->volume = _volume;
}

real RigidBody::getDensity()
{
    return this->density;
}

void RigidBody::setDensity(real _density)
{
    this->density = _density;
}

real RigidBody::getInverseMass()
{
    return this->inverseMass;
}

Vector3 * RigidBody::getPosition()
{
    return this->position;
}

void RigidBody::setPosition(Vector3 * _position)
{
    this->position->x = _position ? _position->x : 0;
    this->position->y = _position ? _position->y : 0;
    this->position->z = _position ? _position->z : 0;
}

void RigidBody::setPosition(real _x, real _y, real _z)
{
    this->position->x = _x;
    this->position->y = _y;
    this->position->z = _z;
}

void RigidBody::setPosition(real _x, real _y)
{
    this->position->x = _x;
    this->position->y = _y;
}

Vector3 * RigidBody::getAcceleration()
{
    return this->acceleration;
}

void RigidBody::setAcceleration(Vector3 * _acceleration)
{
    this->acceleration->x = _acceleration ? _acceleration->x : 0;
    this->acceleration->y = _acceleration ? _acceleration->y : 0;
    this->acceleration->z = _acceleration ? _acceleration->z : 0;
}

void RigidBody::setAcceleration(real _x, real _y, real _z)
{
    this->acceleration->x = _x;
    this->acceleration->y = _y;
    this->acceleration->z = _z;
}

void RigidBody::setAcceleration(real _x, real _y)
{
    this->acceleration->x = _x;
    this->acceleration->y = _y;
}

Vector3 * RigidBody::getVelocity()
{
    return this->velocity;
}

void RigidBody::setVelocity(Vector3 * _velocity)
{
    this->velocity->x = _velocity ? _velocity->x : 0;
    this->velocity->y = _velocity ? _velocity->y : 0;
    this->velocity->z = _velocity ? _velocity->z : 0;
}

void RigidBody::setVelocity(real _x, real _y, real _z)
{
    this->velocity->x = _x;
    this->velocity->y = _y;
    this->velocity->z = _z;
}

void RigidBody::setVelocity(real _x, real _y)
{
    this->velocity->x = _x;
    this->velocity->y = _y;
}

Vector3 * RigidBody::getForceAccum()
{
    return this->forceAccum;
}

Vector3 * RigidBody::getTorqueAccum()
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

Vector3 * RigidBody::getLastFrameAcceleration()
{
    return this->lastFrameAcceleration;
}

void RigidBody::setLastFrameAcceleration(Vector3 * _lastFrameAcceleration)
{
    this->lastFrameAcceleration->x = _lastFrameAcceleration ? _lastFrameAcceleration->x : 0;
    this->lastFrameAcceleration->y = _lastFrameAcceleration ? _lastFrameAcceleration->y : 0;
    this->lastFrameAcceleration->z = _lastFrameAcceleration ? _lastFrameAcceleration->z : 0;
}

void RigidBody::setLastFrameAcceleration(real _x, real _y, real _z)
{
    this->lastFrameAcceleration->x = _x;
    this->lastFrameAcceleration->y = _y;
    this->lastFrameAcceleration->z = _z;
}

void RigidBody::setLastFrameAcceleration(real _x, real _y)
{
    this->lastFrameAcceleration->x = _x;
    this->lastFrameAcceleration->y = _y;
}

Matrix3 * RigidBody::getInverseInertiaTensorWorld()
{
    return this->inverseInertiaTensorWorld;
}

void RigidBody::setInverseInertiaTensorWorld(Matrix3 * _inverseInertiaTensorWorld)
{
    this->inverseInertiaTensorWorld = _inverseInertiaTensorWorld;
}

Vector3 * RigidBody::getRotation()
{
    return this->rotation;
}

void RigidBody::setRotation(Vector3 * _rotation)
{
    this->rotation->x = _rotation ? _rotation->x : 0;
    this->rotation->y = _rotation ? _rotation->y : 0;
    this->rotation->z = _rotation ? _rotation->z : 0;
}

void RigidBody::setRotation(real _x, real _y, real _z)
{
    this->rotation->x = _x;
    this->rotation->y = _y;
    this->rotation->z = _z;
}

void RigidBody::setRotation(real _x, real _y)
{
    this->rotation->x = _x;
    this->rotation->y = _y;
}

real RigidBody::getRadius()
{
    return this->radius;
}

void RigidBody::setRadius(real _radius)
{
    this->radius = _radius;
}


Quaternion * RigidBody::getOrientation()
{
    return this->orientation;
}

void RigidBody::setOrientation(Quaternion * _orientation)
{
    this->orientation->r = _orientation->r;
    this->orientation->i = _orientation->i;
    this->orientation->j = _orientation->j;
    this->orientation->k = _orientation->k;
}

void RigidBody::setOrientation(real _r, real _i, real _j, real _k)
{
    this->orientation->r = _r;
    this->orientation->i = _i;
    this->orientation->j = _j;
    this->orientation->k = _k;
}

TypeObject RigidBody::getTypeObject()
{
    return this->typeObject;
}

void RigidBody::setTypeObject(TypeObject _typeObject)
{
    this->typeObject = _typeObject;
}

Matrix4 * RigidBody::getTransformMatrix()
{
    return this->transformMatrix;
}

void RigidBody::getGLTransform(float matrix[16]) const
{
    matrix[0] = this->transformMatrix->data[0];
    matrix[1] = this->transformMatrix->data[4];
    matrix[2] = this->transformMatrix->data[8];
    matrix[3] = 0;
    
    matrix[4] = this->transformMatrix->data[1];
    matrix[5] = this->transformMatrix->data[5];
    matrix[6] = this->transformMatrix->data[9];
    matrix[7] = 0;
    
    matrix[8] = this->transformMatrix->data[2];
    matrix[9] = this->transformMatrix->data[6];
    matrix[10] = this->transformMatrix->data[10];
    matrix[11] = 0;
    
    matrix[12] = this->transformMatrix->data[3];
    matrix[13] = this->transformMatrix->data[7];
    matrix[14] = this->transformMatrix->data[11];
    matrix[15] = 1;
}

Vector3 * RigidBody::getHalfSize()
{
    return this->halfSize;
}

void RigidBody::setHalfSize(Vector3 * _halfSize)
{
    this->halfSize->x = _halfSize->x;
    this->halfSize->y = _halfSize->y;
    this->halfSize->z = _halfSize->z;
}

void RigidBody::setHalfSize(real _x, real _y, real _z)
{
    this->halfSize->x = _x;
    this->halfSize->y = _y;
    this->halfSize->z = _z;
}

void RigidBody::setHalfSize(real _x, real _y)
{
    this->halfSize->x = _x;
    this->halfSize->y = _y;
}

bool RigidBody::isDirty()
{
    return this->dirty;
}

void RigidBody::setDirty(bool _dirty)
{
    this->dirty = _dirty;
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
        // add a bit of motion to avoid it falling asleep immediately.
        this->motion = sleepEpsilon*2.0f;
    } else {
        this->awake = false;
        this->velocity->clear();
        this->rotation->clear();
    }
}
