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

struct QuadTreeNode {
    Vector3 * center;
    real halfWidth;
    QuadTreeNode * child[4];
    std::map<char *, RigidBody*> * rigidBodies;
    QuadTreeNode() {
        this->center = new Vector3();
        this->rigidBodies = new std::map<char*, RigidBody *>();
    }
    ~QuadTreeNode() {
        delete this->center;
        this->center = NULL;

        this->rigidBodies->clear();
        delete this->rigidBodies;
        this->rigidBodies = NULL;
    }
};

class QuadTree {
private:
    QuadTreeNode * parent;
    
    std::vector<RigidBody *> * possibleCollisions;
    void insertObject(QuadTreeNode * _tree, RigidBody * _body);
    void deleteObject(QuadTreeNode * _tree, RigidBody * _body);
    void cleanLeaves(QuadTreeNode * _tree);
    QuadTreeNode * buildQuadTree(Vector3 * _center, real _halfWidth, int _stopDepth);
public:
    QuadTree();
    ~QuadTree();
    
    void buildQuadTree();
    void insertObject(RigidBody * _body);
    void updateObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    
    void cleanLeaves();
    
    std::vector<RigidBody *> * getPossibleCollisions();
};

#endif
