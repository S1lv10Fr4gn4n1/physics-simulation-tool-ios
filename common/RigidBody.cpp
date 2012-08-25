//
//  RigidBody.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#include "RigidBody.h"

static inline void _calculateTransformMatrix(Matrix4 * _transformMatrix, const Vector3 * _position,
                                             const Quaternion * _orientation)
{
    _transformMatrix->data[0] = 1 - 2*_orientation->j*_orientation->j - 2*_orientation->k*_orientation->k;
    _transformMatrix->data[1] = 2*_orientation->i*_orientation->j - 2*_orientation->r*_orientation->k;
    _transformMatrix->data[2] = 2*_orientation->i*_orientation->k + 2*_orientation->r*_orientation->j;
    _transformMatrix->data[3] = _position->x;
    _transformMatrix->data[4] = 2*_orientation->i*_orientation->j + 2*_orientation->r*_orientation->k;
    _transformMatrix->data[5] = 1 - 2*_orientation->i*_orientation->i - 2*_orientation->k*_orientation->k;
    _transformMatrix->data[6] = 2*_orientation->j*_orientation->k - 2*_orientation->r*_orientation->i;
    _transformMatrix->data[7] = _position->y;
    _transformMatrix->data[8] = 2*_orientation->i*_orientation->k - 2*_orientation->r*_orientation->j;
    _transformMatrix->data[9] = 2*_orientation->j*_orientation->k + 2*_orientation->r*_orientation->i;
    _transformMatrix->data[10] = 1 - 2*_orientation->i*_orientation->i-2*_orientation->j*_orientation->j;
    _transformMatrix->data[11] = _position->z;
}

static inline void _transformInertiaTensor(Matrix3 * _iitWorld, const Quaternion * _quaternion,
                                           const Matrix3 * _iitBody, const Matrix4 * _rotmat)
{
    real t4 = _rotmat->data[0]*_iitBody->data[0]+
              _rotmat->data[1]*_iitBody->data[3]+
              _rotmat->data[2]*_iitBody->data[6];
    real t9 = _rotmat->data[0]*_iitBody->data[1]+
              _rotmat->data[1]*_iitBody->data[4]+
              _rotmat->data[2]*_iitBody->data[7];
    real t14 = _rotmat->data[0]*_iitBody->data[2]+
               _rotmat->data[1]*_iitBody->data[5]+
               _rotmat->data[2]*_iitBody->data[8];
    real t28 = _rotmat->data[4]*_iitBody->data[0]+
               _rotmat->data[5]*_iitBody->data[3]+
               _rotmat->data[6]*_iitBody->data[6];
    real t33 = _rotmat->data[4]*_iitBody->data[1]+
               _rotmat->data[5]*_iitBody->data[4]+
               _rotmat->data[6]*_iitBody->data[7];
    real t38 = _rotmat->data[4]*_iitBody->data[2]+
               _rotmat->data[5]*_iitBody->data[5]+
               _rotmat->data[6]*_iitBody->data[8];
    real t52 = _rotmat->data[8]*_iitBody->data[0]+
               _rotmat->data[9]*_iitBody->data[3]+
               _rotmat->data[10]*_iitBody->data[6];
    real t57 = _rotmat->data[8]*_iitBody->data[1]+
               _rotmat->data[9]*_iitBody->data[4]+
               _rotmat->data[10]*_iitBody->data[7];
    real t62 = _rotmat->data[8]*_iitBody->data[2]+
               _rotmat->data[9]*_iitBody->data[5]+
               _rotmat->data[10]*_iitBody->data[8];
    _iitWorld->data[0] = t4*_rotmat->data[0]+
                         t9*_rotmat->data[1]+
                         t14*_rotmat->data[2];
    _iitWorld->data[1] = t4*_rotmat->data[4]+
                         t9*_rotmat->data[5]+
                         t14*_rotmat->data[6];
    _iitWorld->data[2] = t4*_rotmat->data[8]+
                         t9*_rotmat->data[9]+
                         t14*_rotmat->data[10];
    _iitWorld->data[3] = t28*_rotmat->data[0]+
                        t33*_rotmat->data[1]+
                        t38*_rotmat->data[2];
    _iitWorld->data[4] = t28*_rotmat->data[4]+
                         t33*_rotmat->data[5]+
                         t38*_rotmat->data[6];
    _iitWorld->data[5] = t28*_rotmat->data[8]+
                        t33*_rotmat->data[9]+
                        t38*_rotmat->data[10];
    _iitWorld->data[6] = t52*_rotmat->data[0]+
                         t57*_rotmat->data[1]+
                         t62*_rotmat->data[2];
    _iitWorld->data[7] = t52*_rotmat->data[4]+
                         t57*_rotmat->data[5]+
                         t62*_rotmat->data[6];
    _iitWorld->data[8] = t52*_rotmat->data[8]+
                         t57*_rotmat->data[9]+
                         t62*_rotmat->data[10];
}

RigidBody::RigidBody()
{
    this->mass = 0.0f;
    this->inverseMass = 0.0f;
    this->angularDamping = 0.0f;
    this->linearDamping = 0.0f;
    this->volume = 0.0f;
    this->density = 0.0f;

    this->position = Vector3::MakeVector3(0.0f, 0.0f);
    this->acceleration = Vector3::MakeVector3(0.0f, 0.0f);
    this->lastFrameAcceleration = Vector3::MakeVector3(0.0f, 0.0f);
    this->velocity = Vector3::MakeVector3(0.0f, 0.0f);
    this->orientation = new Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
    this->rotation = Vector3::MakeVector3(0.0f, 0.0f);
    this->forceAccum = Vector3::MakeVector3(0.0f, 0.0f);
    this->torqueAccum = Vector3::MakeVector3(0.0f, 0.0f);

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

}

void RigidBody::calculateDerivedData()
{
    this->orientation->normalize();
    
    // calculate the transform matrix for the body.
    _calculateTransformMatrix(this->transformMatrix, this->position, this->orientation);
    
    // calculate the inertiaTensor in world space.
    _transformInertiaTensor(inverseInertiaTensorWorld,
                            this->orientation,
                            this->inverseInertiaTensor,
                            this->transformMatrix);
    
}

void RigidBody::setInertiaTensor(const Matrix3 * _inertiaTensor)
{
    inverseInertiaTensor->setInverse(_inertiaTensor);
//    _checkInverseInertiaTensor(inverseInertiaTensor);
}

void RigidBody::addForce(const Vector3 * _force)
{
    *this->forceAccum += _force;
}

void RigidBody::addForceAtBodyPoint(const Vector3 * _force, const Vector3 * _point)
{
    // convert to coordinates relative to the center of mass.
    Vector3 * pt = this->getPointInWorldSpace(_point);
    this->addForceAtPoint(_force, pt);
}

//void RigidBody::integrate(real _duration)
//{
//    if (this->inverseMass <= 0.0f) {
//        return;
//    }
//    
//    // calculate linear acceleration from force inputs.
//    if (this->lastFrameAcceleration) {
//        delete this->lastFrameAcceleration;
//        this->lastFrameAcceleration = NULL;
//    }
//    this->lastFrameAcceleration = Vector3::MakeVector3(this->acceleration);
//    this->lastFrameAcceleration->addScaledVector(this->forceAccum, this->inverseMass);
//    
//    // calculate angular acceleration from torque inputs.
//    Vector3 * angularAcceleration = this->inverseInertiaTensorWorld->transform(this->torqueAccum);
//    
//    // adjust velocities
//    // update linear velocity from both acceleration and impulse.
//    this->velocity->addScaledVector(this->lastFrameAcceleration, _duration);
//    
//    // update angular velocity from both acceleration and impulse.
//    this->rotation->addScaledVector(angularAcceleration, _duration);
//    
//    // impose drag.
//    *this->velocity *= real_pow(this->linearDamping, _duration);
//    *this->rotation *= real_pow(this->angularDamping, _duration);
//    
//    // adjust positions
//    // update linear position.
//    this->position->addScaledVector(this->velocity, _duration);
//    
//    // update angular position.
//    // impose drag.
//    *this->velocity *= real_pow(this->linearDamping, _duration);
//    *this->rotation *= real_pow(this->angularDamping, _duration);
//    
//    // normalize the orientation, and update the matrices with the new
//    // position and orientation.
//    this->calculateDerivedData();
//    
//    // clear accumulators.
//    this->clearAccumulators();
//    
//    delete angularAcceleration;
//    angularAcceleration = NULL;
//}

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
    Vector3 * point = Vector3::MakeVector3(_point);
    *point -= this->position;
    
    *this->forceAccum += _force;
    *this->torqueAccum += *point % _force;
    
    delete point;
    point = NULL;
}

void RigidBody::addTorque(const Vector3 * _torque)
{
    *this->torqueAccum += _torque;
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
