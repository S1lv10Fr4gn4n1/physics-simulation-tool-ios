//
//  Selection.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 03/07/12.
//  
//

#ifndef SELECTION_H
#define SELECTION_H

#include "Commons.h"

class Selection {
private:
public:
    static SimulatedObject * selectSimulatedObject(World * _world, const Vector3 &_vector);
};

#endif
