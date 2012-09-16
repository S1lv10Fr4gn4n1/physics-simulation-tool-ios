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
#include "Quadtree.h"
//#include "Octree.h"
#include "FineCollision.h"
#include "ContactResolver.h"

class MainCollision {
private:
    CollisionData * dataContacts;
    ContactResolver * contactResolver;
    QuadTree * tree;
//    Octree * tree;

    void generateContact(RigidBody * _body1, RigidBody * _body2);
    void generateContacts();
    void solverContacts(real _duration);
public:
    MainCollision();
    ~MainCollision();
    
    void insertObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    void updateObject(RigidBody * _body, real _duration);
    void updateContacts(real _duration);
    
    void cleanCollisions();
};

#endif
