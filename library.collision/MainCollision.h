//
//  MainCollision.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#ifndef MAINCOLLISION_H
#define MAINCOLLISION_H

#include "Commons.h"
#include "CoarseCollision.h"

class MainCollision {
private:
    static MainCollision * mainCollision;
    
    void generateContacts();
    void solverContacts();
public:
    MainCollision();
    ~MainCollision();
    
    static MainCollision * getInstance();

    void insertObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    void updateObject(RigidBody * _body, real _duration);
    void updateContacts();
};

#endif
