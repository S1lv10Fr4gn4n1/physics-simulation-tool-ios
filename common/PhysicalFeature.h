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

class PhysicalFeature {
private:

protected:
    
public:
    PhysicalFeature();
    ~PhysicalFeature();
    
    real mass;
    real volume;
    real density;
    real inverseMass;
    real damping;
    Vector3 * position;
    Vector3 * acceleration;
    Vector3 * velocity;
    Vector3 * forceAccum;

    static PhysicalFeature * MakePhysicalFeature(real _mass,
                                                 real _volume,
                                                 real _density,
                                                 real _damping,
                                                 Vector3 * _acceleration,
                                                 Vector3 * _position,
                                                 Vector3 * _velocity,
                                                 Vector3 * _forceAccum);
};

#endif
