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
    static MainPhysics * mainPhysics;
public:
    MainPhysics();
    ~MainPhysics();
    
    static MainPhysics * getInstance();
    
    void updateFeatures(RigidBody * _body, real _duration);
    void updateFeatures(Particle * _particle, real _duration);
};

#endif
