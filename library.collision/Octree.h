//
//  Octree.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 16/09/12.
//
//

#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <map>
#include "RigidBody.h"

struct OctreeNode {
    Vector3 * center;
    real halfWidth;
    OctreeNode * child[8];
    std::map<char *, RigidBody*> * rigidBodies;
    OctreeNode() {
        this->center = new Vector3();
        this->rigidBodies = new std::map<char*, RigidBody *>();
    }
    ~OctreeNode() {
        delete this->center;
        this->center = NULL;
        
        this->rigidBodies->clear();
        delete this->rigidBodies;
        this->rigidBodies = NULL;
    }
};

class Octree {
private:
    OctreeNode * parent;
    
    std::vector<RigidBody *> * possibleCollisions;
    void insertObject(OctreeNode * _tree, RigidBody * _body);
    void deleteObject(OctreeNode * _tree, RigidBody * _body);
    void cleanLeaves(OctreeNode * _tree);
    OctreeNode * buildOctree(Vector3 * _center, real _halfWidth, int _stopDepth);
public:
    Octree();
    ~Octree();
    
    void buildOctree();
    void insertObject(RigidBody * _body);
    void updateObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    
    void cleanLeaves();
    
    std::vector<RigidBody *> * getPossibleCollisions();
};


#endif
