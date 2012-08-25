//
//  CoarseCollision.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 23/08/12.
//
//

#ifndef COARSECOLLISION_H
#define COARSECOLLISION_H

#include <vector>
#include "RigidBody.h"

struct PotentialContact {
    RigidBody * body[2];
};

template<class BoundingVolumeClass>
class BVHNode {
private:
    // Holds the child nodes of this node.
    BVHNode * children[2];
    
    BVHNode * parent;
    
    // Holds a single bounding volume encompassing all the descendents of this node.
    BoundingVolumeClass volume;
    RigidBody * body;
protected:
    // checks if this node is at the bottom of the hierarchy.
    bool isLeaf() const;
    
    bool overlaps(const BVHNode<BoundingVolumeClass> * _other) const;
    
    unsigned getPotentialContactsWith(const BVHNode<BoundingVolumeClass> * _other,
                                      PotentialContact * _contacts,
                                      unsigned _limit) const;
    
    void recalculateBoundingVolume();
public:
    BVHNode(BVHNode * _parent, const BoundingVolumeClass * _volume, RigidBody * _body=NULL);
    ~BVHNode();
    unsigned getPotentialContacts(PotentialContact * _contacts, unsigned _limit) const;
    void insert(RigidBody * _newBody, const BoundingVolumeClass * _newVolume);
};


class BoundingSphere {
private:
    Vector3 * center;
    real radius;
public:
    BoundingSphere(Vector3 * _center, real _radius);
    BoundingSphere(const BoundingSphere * _one, const BoundingSphere * _two);
    bool overlaps(const BoundingSphere * _other) const;
    real getGrowth(const BoundingSphere * _other) const;
    real getSize() const;
};

struct QuadTreeNode {
    Vector3 * center;
    real halfWidth;
    QuadTreeNode * child[4];
    RigidBody * rigidBody;
    QuadTreeNode() {
        this->center = new Vector3();
    }
    ~QuadTreeNode() {
        delete this->center;
        this->center = NULL;
    }
};

class QuadTree {
private:
    static QuadTree * quadTree;
    QuadTreeNode * parent;
    
    std::vector<RigidBody *> * listObjects;
    void insertObject(QuadTreeNode * _tree, RigidBody * _body);
    void deleteObject(QuadTreeNode * _tree, RigidBody * _body);
    QuadTreeNode * buildQuadTree(Vector3 * _center, real _halfWidth, int _stopDepth);
public:
    QuadTree();
    ~QuadTree();
    
    void buildQuadTree();
    void insertObject(RigidBody * _body);
    void updateObject(RigidBody * _body);
    void deleteObject(RigidBody * _body);
    
    std::vector<RigidBody *> * getPossibleCollisions();
    
    static QuadTree * getInstance();
};

#endif
