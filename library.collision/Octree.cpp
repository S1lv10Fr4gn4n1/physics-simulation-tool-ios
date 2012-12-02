//
//  Octree.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 16/09/12.
//
//

#include "Octree.h"

typedef std::pair<char *, RigidBody *> mapBody;
typedef std::map<char *, RigidBody *>::iterator mapBodyIter;

#define PLUS true
#define MINUS false

inline bool less(real k1,real k2)
{
    return k1 < k2;
}


inline bool bigger(real k1,real k2)
{
    return k1 > k2;
}

//!Method responsible for identifying the location in which rigid body will be inserted / deleted / found based on your focal point and half-size knot
bool testOctant(OctreeNode * _tree, RigidBody * _body, bool _v1, bool _v2, bool _v3)
{
    //    +++
    //    ++-
    //    +-+
    //    +--
    //    --+
    //    ---
    //    -++
    //    -+-
    if (!(_v1 ^ bigger(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z + _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z - _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z + _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z - _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z + _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z - _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z + _body->getHalfSize().z, _tree->center.z)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y)) &&
        !(_v3 ^ bigger(_body->getPosition().z - _body->getHalfSize().z, _tree->center.z)))
        return true;

    return false;
}

//!Identifies whether the octant is + + +
bool inPlusPlusPlus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, PLUS, PLUS, PLUS);
}

//!Identifies whether the octant is + + -
bool inPlusPlusMinus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, PLUS, PLUS, MINUS);
}

//!Identifies whether the octant is + - +
bool inPlusMinusPlus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, PLUS, MINUS, PLUS);
}

//!Identifies whether the octant is + - -
bool inPlusMinusMinus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, PLUS, MINUS, MINUS);
}

//!Identifies whether the octant is - - +
bool inMinusMinusPlus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, MINUS, MINUS, PLUS);
}

//!Identifies whether the octant is - - -
bool inMinusMinusMinus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, MINUS, MINUS, MINUS);
}

//!Identifies whether the octant is - + +
bool inMinusPlusPlus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, MINUS, PLUS, PLUS);
}

//!Identifies whether the octant is - + -
bool inMinusPlusMinus(OctreeNode * _tree, RigidBody * _body)
{
    return testOctant(_tree, _body, MINUS, PLUS, MINUS);
}

//!Identify if the rigid body is achieved by node radius
bool radiusReachesObject(OctreeNode * _tree, RigidBody * _body)
{
    for (unsigned i=0; i<3; i++) {
        if ((_tree->center[i] + _tree->halfWidth >= _body->getPosition()[i] + _body->getHalfSize()[i] ||
             _tree->center[i] + _tree->halfWidth >= _body->getPosition()[i] - _body->getHalfSize()[i]) &&
            (_tree->center[i] - _tree->halfWidth <= _body->getPosition()[i] + _body->getHalfSize()[i] ||
             _tree->center[i] - _tree->halfWidth <= _body->getPosition()[i] - _body->getHalfSize()[i])) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

Octree::Octree()
{
    this->possibleCollisions = new std::vector<RigidBody *>();
    this->parent = this->buildOctree(Vector3(0.0f, 2.0f, 0.0f), WIDTH_SCENE, DEPTH_TREE);
}

Octree::~Octree()
{
    if (this->parent) {
        delete this->parent;
    }
    
    this->parent = NULL;
    
    this->possibleCollisions->clear();
    delete this->possibleCollisions;
    this->possibleCollisions = NULL;
}

//!Method responsible for creating the tree as the size of the desired scene and profunidade
OctreeNode * Octree::buildOctree(const Vector3 &_center, real _halfWidth, int _depth)
{
    if (_depth < 0) {
        return NULL;
    } else {
        OctreeNode * node = new OctreeNode();
        node->center = _center;
        node->halfWidth = _halfWidth;
        
        Vector3 offset;
        Vector3 offsetAux;
        real step = _halfWidth * 0.5f;

        for (int i = 0; i < 8; i++) {
            offset.x = ((i & 1) ? step : -step);
            offset.y = ((i & 2) ? step : -step);
            offset.z = ((i & 4) ? step : -step);
            
            offsetAux = _center + offset;
            node->child[i] = this->buildOctree(offsetAux, step, _depth - 1);
        }

        return node;
    }
}

//!Updates the position of the rigid body in the octree (call recursive method)
void Octree::updateObject(RigidBody * _body)
{
    this->deleteObject(_body);
    this->insertObject(_body);
}

//!Delete rigid body (call recursive method)
void Octree::deleteObject(RigidBody * _body)
{
    for (int i=0; i<8; i++) {
        this->deleteObject(this->parent->child[i], _body);
    }
}

RigidBody * getBody(OctreeNode * _tree, RigidBody * _body)
{
    mapBodyIter iter;
    
    iter = _tree->rigidBodies->find(_body->getId());
    if (iter != _tree->rigidBodies->end()) {
        return iter->second;
    }
    
    return NULL;
}

void Octree::cleanLeaves()
{
    this->cleanLeaves(this->parent);
}

//!Clear leaves of octree (call recursive method)
void Octree::cleanLeaves(OctreeNode * _tree)
{
    if (!_tree) {
        return;
    }
    
    _tree->rigidBodies->clear();
    
    for (unsigned i=0; i<8; i++) {
        this->cleanLeaves(_tree->child[i]);
    }
}

void Octree::deleteObject(OctreeNode * _tree, RigidBody * _body)
{
    if (!_tree) {
        return;
    }
    
    if (!radiusReachesObject(_tree, _body)) {
        return;
    }
    
    RigidBody * body = getBody(_tree, _body);
    if (body) {
//        if (body->hasFiniteMass()) {
//        printf("bodies x:%f, y:%f, body: %s -> before delete size: %lu\n", _tree->center.x, _tree->center.y, _body->getId(), _tree->rigidBodies->size());
        _tree->rigidBodies->erase(body->getId());
//        printf("bodies x:%f, y:%f, body: %s -> after delete size: %lu\n", _tree->center.x, _tree->center.y, _body->getId(), _tree->rigidBodies->size());
//        }
    }
    
    if (inMinusMinusMinus(_tree, _body)) {
        this->deleteObject(_tree->child[0], _body);
    }

    if (inPlusMinusMinus(_tree, _body)) {
        this->deleteObject(_tree->child[1], _body);
    }

    if (inMinusPlusMinus(_tree, _body)) {
        this->deleteObject(_tree->child[2], _body);
    }

    if (inPlusPlusMinus(_tree, _body)) {
        this->deleteObject(_tree->child[3], _body);
    }

    if (inMinusMinusPlus(_tree, _body)) {
        this->deleteObject(_tree->child[4], _body);
    }

    if (inPlusMinusPlus(_tree, _body)) {
        this->deleteObject(_tree->child[5], _body);
    }

    if (inMinusPlusPlus(_tree, _body)) {
        this->deleteObject(_tree->child[6], _body);
    }

    if (inPlusPlusPlus(_tree, _body)) {
        this->deleteObject(_tree->child[7], _body);
    }
}

void Octree::insertObject(RigidBody * _body)
{
    for (unsigned i=0; i<8; i++) {
        this->insertObject(this->parent->child[i], _body);
    }
}

void Octree::insertObject(OctreeNode * _tree, RigidBody * _body) {
    if (!_tree) {
        return;
    }

    if (!radiusReachesObject(_tree, _body)) {
        return;
    }
    
    if (!_tree->child[0]) {
        if (_tree->rigidBodies->size() > 0) {
            mapBodyIter iter;
            for(iter = _tree->rigidBodies->begin(); iter != _tree->rigidBodies->end(); iter++) {
                this->possibleCollisions->push_back(iter->second);
            }
            
            this->possibleCollisions->push_back(_body);
        }
//        printf("body: %s, tree x:%f, y:%f, z:%f\n", _body->getId(), _tree->center.x, _tree->center.y, _tree->center.z);
        _tree->rigidBodies->insert(mapBody(_body->getId(), _body));
    }
    
    if (inMinusMinusMinus(_tree, _body)) {
        this->insertObject(_tree->child[0], _body);
    }

    if (inPlusMinusMinus(_tree, _body)) {
        this->insertObject(_tree->child[1], _body);
    }

    if (inMinusPlusMinus(_tree, _body)) {
        this->insertObject(_tree->child[2], _body);
    }

    if (inPlusPlusMinus(_tree, _body)) {
        this->insertObject(_tree->child[3], _body);
    }

    if (inMinusMinusPlus(_tree, _body)) {
        this->insertObject(_tree->child[4], _body);
    }

    if (inPlusMinusPlus(_tree, _body)) {
        this->insertObject(_tree->child[5], _body);
    }

    if (inMinusPlusPlus(_tree, _body)) {
        this->insertObject(_tree->child[6], _body);
    }

    if (inPlusPlusPlus(_tree, _body)) {
        this->insertObject(_tree->child[7], _body);
    }
}

std::vector<RigidBody *> * Octree::getPossibleCollisions()
{
    return this->possibleCollisions;
}