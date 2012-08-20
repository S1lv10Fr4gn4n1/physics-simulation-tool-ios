//
//  ForceGenerators.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 17/08/12.
//
//

#ifndef FORCEGENERATORS_H
#define FORCEGENERATORS_H

#include "SimulatedObject.h"

// interface ForceGenerator
class ForceGenerator {
public:
    virtual ~ForceGenerator();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration) = 0;
};

class ForceRegistry {
protected:
    struct ForceRegistration {
        SimulatedObject * simulatedObject;
        ForceGenerator * forceGenerator;
    };
    std::vector<ForceRegistration *> * registrations;
    
    static ForceRegistry * forceRegistry;
public:
    ForceRegistry();
    ~ForceRegistry();
    void add(SimulatedObject * _simulatedObject, ForceGenerator * _forceGenerator);
    void removeObject(SimulatedObject * _simulatedObject);
    void removeForceOfObject(SimulatedObject * _simulatedObject, ForceGenerator * _forceGenerator);
    void clear();
    void updateForces(real _duration);
    
    static ForceRegistry * getInstance();
};


// force generator - gravitational force
class ForceGravity : public ForceGenerator {
private:
    Vector3 * gravity;
public:
    ForceGravity(Vector3 * _gravity);
    ~ForceGravity();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
    void updateGravity(real _value);
};


// force generator - drag force
class ForceDrag : public ForceGenerator {
private:
    //velocity drag coefficient
    real k1;
    // velocity squared drag coefficient
    real k2;
public:
    ForceDrag(real _k1, real _k2);
    ~ForceDrag();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
};


// force generate - spring force
class ForceSpring : public ForceGenerator {
private:
    SimulatedObject * other;
    real springConstant;
    real restLength;
public:
    ForceSpring(SimulatedObject * _other, real _springConstant, real _restLength);
    ~ForceSpring();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
};


// force generate - anchored spring
class ForceAnchoredSpring : public ForceGenerator {
private:
    Vector3 * anchor;
    real springConstant;
    real restLength;
public:
    ForceAnchoredSpring(Vector3 * _anchor, real _springConstant, real _restLength);
    ~ForceAnchoredSpring();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
    
    void setAnchor(Vector3 * _anchor);
};


// force generate - force bungee
class ForceBungee : public ForceGenerator {
    SimulatedObject * other;
    real springConstant;
    real restLength;
public:
    ForceBungee(SimulatedObject * _other, real _springConstant, real _restLength);
    ~ForceBungee();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
};

// force generate - force anchored bungee
class ForceAnchoredBungee : public ForceGenerator {
    Vector3 * anchor;
    real springConstant;
    real restLength;
public:
    ForceAnchoredBungee(Vector3 * _anchor, real _springConstant, real _restLength);
    ~ForceAnchoredBungee();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
    
    void setAnchor(Vector3 * _anchor);
};


// force generate - force buoyancy
// a force generator that applies a buoyancy force for a plane of liquid parallel to XZ plane
class ForceBuoyancy : public ForceGenerator {
    // maximum submersion depth of the object before it generates its maximum buoyancy force.
    real maxDepth;
    // volume of the object.
    real volume;
    // height of the water plane above y=0. The plane will be parallel to the XZ plane.
    real waterHeight;
    // density of the liquid. Pure water has a density of 1000 kg per cubic meter
    // ocean water has a density of around 1020 to 1030 kg/m3 up to 1250 kg/m3 for the Dead Sea.
    real liquidDensity;
public:
    ForceBuoyancy(real _maxDepth, real _volume, real _waterHeight, real _liquidDensity = 1000.0f);
    ~ForceBuoyancy();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
};


// force generate - force fake stiff spring
// a force generator that fakes a stiff spring force, and where one end is attached to a fixed point in space.
class ForceFakeStiffSpring : public ForceGenerator {
    Vector3 * anchor;
    real springConstant;
    real damping;
public:
    ForceFakeStiffSpring(Vector3 * _anchor, real _springConstant, real _damping);
    ~ForceFakeStiffSpring();
    virtual void updateForce(SimulatedObject * _simulatedObject, real _duration);
};
#endif

