//
//  MainPhysics.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#ifndef MAINPHYSICS_H
#define MAINPHYSICS_H

#include "Commons.h"

class MainPhysics {
private:
public:
    MainPhysics();
    ~MainPhysics();
    
    void updateFeatures(SimulatedObject * _simulatedObject, real _duration);
};

#endif
