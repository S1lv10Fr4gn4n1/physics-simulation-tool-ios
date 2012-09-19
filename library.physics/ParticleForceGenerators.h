//
//  ParticleForceGenerators.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 17/08/12.
//
//

#ifndef PARTICLEFORCEGENERATORS_H
#define PARTICLEFORCEGENERATORS_H

#include <vector>
#include "Particle.h"

// interface ForceGenerator
class ParticleForceGenerator {
public:
    virtual ~ParticleForceGenerator();
    virtual void updateForce(Particle * _particle, real _duration) = 0;
};

class ParticleForceRegistry {
protected:
    struct ParticleForceRegistration {
        Particle * particle;
        ParticleForceGenerator * particleForceGenerator;
    };
    std::vector<ParticleForceRegistration *> * registrations;
    
    static ParticleForceRegistry * particleForceRegistry;
public:
    ParticleForceRegistry();
    ~ParticleForceRegistry();
    void add(Particle * _particle, ParticleForceGenerator * _particleForceGenerator);
    void removeObject(Particle * _particle);
    void removeForceOfObject(Particle * _particle, ParticleForceGenerator * _particleForceGenerator);
    void clear();
    void updateForces(real _duration);
    
    static ParticleForceRegistry * getInstance();
};


class ParticleGravity : public ParticleForceGenerator {
private:
    Vector3 gravity;
public:
    ParticleGravity(const Vector3 &_gravity);
    ~ParticleGravity();
    virtual void updateForce(Particle * _particle, real _duration);
    void updateGravity(real _value);
};


class ParticleDrag : public ParticleForceGenerator {
private:
    //velocity drag coefficient
    real k1;
    // velocity squared drag coefficient
    real k2;
public:
    ParticleDrag(real _k1, real _k2);
    ~ParticleDrag();
    virtual void updateForce(Particle * _particle, real _duration);
};


class ParticleSpring : public ParticleForceGenerator {
private:
    Particle * other;
    real springConstant;
    real restLength;
public:
    ParticleSpring(Particle * _other, real _springConstant, real _restLength);
    ~ParticleSpring();
    virtual void updateForce(Particle * _particle, real _duration);
};


class ParticleAnchoredSpring : public ParticleForceGenerator {
private:
    Vector3 anchor;
    real springConstant;
    real restLength;
public:
    ParticleAnchoredSpring(const Vector3 &_anchor, real _springConstant, real _restLength);
    ~ParticleAnchoredSpring();
    virtual void updateForce(Particle * _particle, real _duration);
    
    void setAnchor(const Vector3 &_anchor);
};


class ParticleBungee : public ParticleForceGenerator {
    Particle * other;
    real springConstant;
    real restLength;
public:
    ParticleBungee(Particle * _other, real _springConstant, real _restLength);
    ~ParticleBungee();
    virtual void updateForce(Particle * _particle, real _duration);
};


class ParticleAnchoredBungee : public ParticleForceGenerator {
    Vector3 anchor;
    real springConstant;
    real restLength;
public:
    ParticleAnchoredBungee(const Vector3 &_anchor, real _springConstant, real _restLength);
    ~ParticleAnchoredBungee();
    virtual void updateForce(Particle * _particle, real _duration);
    
    void setAnchor(const Vector3 &_anchor);
};


// a force generator that applies a buoyancy force for a plane of liquid parallel to XZ plane
class ParticleBuoyancy : public ParticleForceGenerator {
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
    ParticleBuoyancy(real _maxDepth, real _volume, real _waterHeight, real _liquidDensity = 1000.0f);
    ~ParticleBuoyancy();
    virtual void updateForce(Particle * _particle, real _duration);
};


// a force generator that fakes a stiff spring force, and where one end is attached to a fixed point in space.
class ParticleFakeStiffSpring : public ParticleForceGenerator {
    Vector3 anchor;
    real springConstant;
    real damping;
public:
    ParticleFakeStiffSpring(const Vector3 &_anchor, real _springConstant, real _damping);
    ~ParticleFakeStiffSpring();
    virtual void updateForce(Particle * _particle, real _duration);
};
#endif

