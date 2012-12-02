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

//!Struct to keep the node for Octree
struct OctreeNode {
    //!identify the center node
    Vector3 center;
    //!indicates a half-size node
    real halfWidth;
    //!stores the children of this node
    OctreeNode * child[8];
    //!stores rigid bodies contained in the node
    std::map<char *, RigidBody*> * rigidBodies;
    OctreeNode() {
        this->rigidBodies = new std::map<char*, RigidBody *>();
    }
    ~OctreeNode() {
        this->rigidBodies->clear();
        delete this->rigidBodies;
        this->rigidBodies = NULL;
    }
};

//!Class responsible for managing the data structure of type octree
class Octree {
private:
    //!root node
    OctreeNode * parent;
    //!list of possible collisions identified
    std::vector<RigidBody *> * possibleCollisions;
    void insertObject(OctreeNode * _tree, RigidBody * _body);
    void deleteObject(OctreeNode * _tree, RigidBody * _body);
    void cleanLeaves(OctreeNode * _tree);
    OctreeNode * buildOctree(const Vector3 &_center, real _halfWidth, int _depth);
public:
    Octree();
    ~Octree();
    
    void insertObject(RigidBody * _body);
    void updateObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    
    void cleanLeaves();
    
    std::vector<RigidBody *> * getPossibleCollisions();
};


#endif
