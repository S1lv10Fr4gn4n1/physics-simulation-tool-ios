//
//  ForceGenerators.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#ifndef FORCEGENERATORS_H
#define FORCEGENERATORS_H

#include "RigidBody.h"

class ForceGenerator {
public:
    virtual void updateForce(RigidBody * _body, real _duration) = 0;
};


class Gravity : public ForceGenerator {
    Vector3 * gravity;
public:
    Gravity(Vector3 * _gravity);
    ~Gravity();
    virtual void updateForce(RigidBody * _body, real _duration);
    void updateGravity(real _gravity);
};


class Spring : public ForceGenerator {
    Vector3 * connectionPoint;
    Vector3 * otherConnectionPoint;
    RigidBody * other;
    real springConstant;
    real restLength;
public:
    Spring(Vector3 * _localConnectionPt, RigidBody * _other, Vector3 * _otherConnectionPt,
           real _springConstant, real _restLength);
    ~Spring();
    virtual void updateForce(RigidBody * _body, real _duration);
};

// TODO revise TORQUE GENERATORS

#endif
