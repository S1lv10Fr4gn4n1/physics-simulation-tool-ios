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
#include "FineCollision.h"

class MainCollision {
private:
    static MainCollision * mainCollision;
    
    CollisionData * dataContacts;

    void generateContact(RigidBody ** _bodies);
    void generateContacts();
    void solverContacts(real _duration);
public:
    MainCollision();
    ~MainCollision();
    
    static MainCollision * getInstance();

    void insertObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    void updateObject(RigidBody * _body, real _duration);
    void updateContacts(real _duration);
};

#endif
