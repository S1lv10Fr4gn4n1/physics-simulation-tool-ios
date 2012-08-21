//
//  TorqueGenerators.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 21/08/12.
//
//

#ifndef TORQUEGENERATORS_H
#define TORQUEGENERATORS_H

#include "RigidBody.h"

class TorqueGenerator {
public:
    virtual void updateToque(RigidBody * _body, real _duration) = 0;
};


#endif
