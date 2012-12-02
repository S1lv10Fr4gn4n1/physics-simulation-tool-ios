//
//  QuadTree.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 23/08/12.
//
//

#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <map>
#include "RigidBody.h"

//!Struct to keep the node for QuadTree
struct QuadTreeNode {
    //!identify the center node
    Vector3 center;
    //!indicates a half-size node
    real halfWidth;
    //!stores the children of this node
    QuadTreeNode * child[4];
    //!stores rigid bodies contained in the node
    std::map<char *, RigidBody*> * rigidBodies;
    QuadTreeNode() {
        this->rigidBodies = new std::map<char*, RigidBody *>();
    }
    ~QuadTreeNode() {
        this->rigidBodies->clear();
        delete this->rigidBodies;
        this->rigidBodies = NULL;
    }
};

//!Class responsible for managing the data structure of type quadtree
class QuadTree {
private:
    //!root node
    QuadTreeNode * parent;
    //!list of possible collisions identified
    std::vector<RigidBody *> * possibleCollisions;
    void insertObject(QuadTreeNode * _tree, RigidBody * _body);
    void deleteObject(QuadTreeNode * _tree, RigidBody * _body);
    void cleanLeaves(QuadTreeNode * _tree);
    QuadTreeNode * buildQuadTree(const Vector3 &_center, real _halfWidth, int _stopDepth);
public:
    QuadTree();
    ~QuadTree();
    
    void insertObject(RigidBody * _body);
    void updateObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    
    void cleanLeaves();
    
    std::vector<RigidBody *> * getPossibleCollisions();
};

#endif
