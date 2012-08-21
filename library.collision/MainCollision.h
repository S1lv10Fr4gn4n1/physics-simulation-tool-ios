//
//  MainCollision.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#ifndef MAINCOLLISION_H
#define MAINCOLLISION_H

#include "SimulatedObject.h"

#include "Links.h"

class MainCollision {
private:
    
public:
    MainCollision();
    ~MainCollision();
    
    void update(SimulatedObject * _simulatedObject, real _duration);
};

#endif
