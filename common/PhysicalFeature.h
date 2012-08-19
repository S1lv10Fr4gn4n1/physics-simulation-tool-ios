//
//  PhysicalFeature.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/08/12.
//
//

#ifndef PHYSICALFEATURE_H
#define PHYSICALFEATURE_H

#include <stdlib.h>
#include "Precision.h"
#include "Vector3.h"
#include "CommonStructs.h"

class PhysicalFeature {
private:
    real mass;
    real volume;
    real density;
    real inverseMass;
    real damping;
    Vector3 * position;
    Vector3 * acceleration;
    Vector3 * velocity;
    Vector3 * forceAccum;
protected:
    
public:
    PhysicalFeature();
    ~PhysicalFeature();
    
    static PhysicalFeature * MakePhysicalFeature(real _mass,
                                                 real _volume,
                                                 real _density,
                                                 real _damping,
                                                 Vector3 * _acceleration,
                                                 Vector3 * _position,
                                                 Vector3 * _velocity);
    
    void clearAccumulator();
    void addForce(Vector3 * _force);
    bool hasFiniteMass();
    
    real getMass();
    void setMass(real _mass);
    real getVolume();
    void setVolume(real _volume);
    real getDensity();
    void setDensity(real _density);
    real getInverseMass();
    real getDamping();
    void setDamping(real _damping);
    Vector3 * getPosition();
    void setPosition(Vector3 * _position);
    Vector3 * getAcceleration();
    void setAcceleration(Vector3 * _acceleration);
    Vector3 * getVelocity();
    void setVelocicy(Vector3 * _velocity);
    Vector3 * getForceAccum();
};

#endif
