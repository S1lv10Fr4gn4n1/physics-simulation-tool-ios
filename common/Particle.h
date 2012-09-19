//
//  Particle.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 19/08/12.
//
//

#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdlib.h>
#include "CommonStructures.h"

class Particle {
private:
    real mass;
    real inverseMass;
    real damping;
    Vector3 position;
    Vector3 acceleration;
    Vector3 velocity;
    Vector3 forceAccum;
public:
    Particle();
    ~Particle();
    
    void clearAccumulator();
    void addForce(const Vector3 &_force);
    bool hasFiniteMass();
    
    real getMass();
    void setMass(real _mass);
    real getInverseMass();
    real getDamping();
    void setDamping(real _damping);
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
};
#endif
