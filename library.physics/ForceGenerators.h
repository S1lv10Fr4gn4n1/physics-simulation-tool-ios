//
//  ForceGenerators.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#ifndef FORCEGENERATORS_H
#define FORCEGENERATORS_H

#include <vector>
#include "RigidBody.h"


class ForceGenerator {
public:
    virtual ~ForceGenerator();
    virtual void updateForce(RigidBody * _body, real _duration) = 0;
};


class ForceRegistry {
protected:
    struct ForceRegistration {
        RigidBody * rigidBody;
        ForceGenerator * forceGenerator;
    };
    std::vector<ForceRegistration *> * registrations;
    
    static ForceRegistry * forceRegistry;
public:
    ForceRegistry();
    ~ForceRegistry();
    void add(RigidBody * _body, ForceGenerator * _forceGenerator);
    void removeObject(RigidBody * _body);
    void removeForceOfObject(RigidBody * _body, ForceGenerator * _forceGenerator);
    void clear();
    void updateForces(real _duration);
    
    static ForceRegistry * getInstance();
};


class Gravity : public ForceGenerator {
    Vector3 gravity;
    bool considerMass;
public:
    Gravity(const Vector3 &_gravity, bool _considerMass=true);
    ~Gravity();
    virtual void updateForce(RigidBody * _body, real _duration);
    void updateGravity(real _gravity);
};


class Spring : public ForceGenerator {
    Vector3 connectionPoint;
    Vector3 otherConnectionPoint;
    RigidBody * other;
    real springConstant;
    real restLength;
public:
    Spring(const Vector3 &_localConnectionPt, RigidBody * _other, const Vector3 &_otherConnectionPt,
           real _springConstant, real _restLength);
    ~Spring();
    virtual void updateForce(RigidBody * _body, real _duration);
};

//class Drag : public ForceGenerator {
//private:
//    //velocity drag coefficient
//    real k1;
//    // velocity squared drag coefficient
//    real k2;
//public:
//    Drag(real _k1, real _k2);
//    ~Drag();
//    virtual void updateForce(RigidBody * _body, real _duration);
//};

// TODO revise TORQUE GENERATORS

#endif
