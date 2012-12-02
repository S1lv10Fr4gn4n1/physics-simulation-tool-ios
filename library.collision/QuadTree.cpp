//
//  QuadTree.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 23/08/12.
//
//

#include "QuadTree.h"

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
bool testQuad(QuadTreeNode * _tree, RigidBody * _body, bool _v1, bool _v2)
{
    if (!(_v1 ^ bigger(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y)))
        return true;

    if (!(_v1 ^ bigger(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x)) &&
        !(_v2 ^ bigger(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y)))
        return true;

    return false;
}

//!Identifies whether the quadrant is + +
bool inPlusPlus(QuadTreeNode * _tree, RigidBody * _body)
{
    return testQuad(_tree, _body, PLUS, PLUS);
}

//!Identifies whether the quadrant is + -
bool inPlusMinus(QuadTreeNode * _tree, RigidBody * _body)
{
    return testQuad(_tree, _body, PLUS, MINUS);
}

//!Identifies whether the quadrant is - -
bool inMinusMinus(QuadTreeNode * _tree, RigidBody * _body)
{
    return testQuad(_tree, _body, MINUS, MINUS);
}

//!Identifies whether the quadrant is - +
bool inMinusPlus(QuadTreeNode * _tree, RigidBody * _body)
{
    return testQuad(_tree, _body, MINUS, PLUS);
}

//!Identify if the rigid body is achieved by node radius
bool radiusReachesObject(QuadTreeNode * _tree, RigidBody * _body)
{
    for (unsigned i=0; i<2; i++) {
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


QuadTree::QuadTree()
{
    this->possibleCollisions = new std::vector<RigidBody *>();
    this->parent = this->buildQuadTree(Vector3(), WIDTH_SCENE, DEPTH_TREE);
}

QuadTree::~QuadTree()
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
QuadTreeNode * QuadTree::buildQuadTree(const Vector3 &_center, real _halfWidth, int _stopDepth)
{
    if (_stopDepth < 0) {
        return NULL;
    } else {
        QuadTreeNode * node = new QuadTreeNode();
        node->center = _center;
        node->halfWidth = _halfWidth;
        
        Vector3 offset;
        Vector3 offsetAux;
        real step = _halfWidth * 0.5f;
        
        for (int i = 0; i < 4; i++) {
            offset.x = ((i & 1) ? step : -step);
            offset.y = ((i & 2) ? step : -step);
            
            offsetAux = _center + offset;
            node->child[i] = this->buildQuadTree(offsetAux, step, _stopDepth - 1);
        }

        return node;
    }
}

//!Updates the position of the rigid body in the quadtree (call recursive method)
void QuadTree::updateObject(RigidBody * _body)
{
    this->deleteObject(_body);
    this->insertObject(_body);
}

//!Delete rigid body (call recursive method)
void QuadTree::deleteObject(RigidBody * _body)
{
    for (int i=0; i<4; i++) {
        this->deleteObject(this->parent->child[i], _body);
    }
}

RigidBody * getBody(QuadTreeNode * _tree, RigidBody * _body)
{
    mapBodyIter iter;
    
    iter = _tree->rigidBodies->find(_body->getId());
    if (iter != _tree->rigidBodies->end()) {
        return iter->second;
    }
    
    return NULL;
}

void QuadTree::cleanLeaves()
{
    this->cleanLeaves(this->parent);
}

//!Clear leaves of quadtree (call recursive method)
void QuadTree::cleanLeaves(QuadTreeNode * _tree)
{
    if (!_tree) {
        return;
    }
    
    _tree->rigidBodies->clear();

    for (int i=0; i<4; i++) {
        this->cleanLeaves(_tree->child[i]);
    }
}

void QuadTree::deleteObject(QuadTreeNode * _tree, RigidBody * _body)
{
    if (!_tree) {
        return;
    }
    
    if (!radiusReachesObject(_tree, _body)) {
        return;
    }
    
    RigidBody * body = getBody(_tree, _body);
    if (body) {
//        printf("bodies x:%f, y:%f, body: %s -> before delete size: %lu\n", _tree->center.x, _tree->center.y, _body->getId(), _tree->rigidBodies->size());
        _tree->rigidBodies->erase(body->getId());
//        printf("bodies x:%f, y:%f, body: %s -> after delete size: %lu\n", _tree->center.x, _tree->center.y, _body->getId(), _tree->rigidBodies->size());
    }
    
    // 0 - -
    if (inMinusMinus(_tree, _body)) {
        this->deleteObject(_tree->child[0], _body);
    }
    
    // 1 + -
    if (inPlusMinus(_tree, _body)) {
        this->deleteObject(_tree->child[1], _body);
    }
    
    // 2 - +
    if (inMinusPlus(_tree, _body)) {
        this->deleteObject(_tree->child[2], _body);
    }
    
    // 3 + +
    if (inPlusPlus(_tree, _body)) {
        this->deleteObject(_tree->child[3], _body);
    }
}

void QuadTree::insertObject(RigidBody * _body)
{
    for (int i=0; i<4; i++) {
        this->insertObject(this->parent->child[i], _body);
    }
}

void QuadTree::insertObject(QuadTreeNode * _tree, RigidBody * _body) {
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
//        printf("bodies x:%f, y:%f, body: %s -> before insert size: %lu\n", _tree->center.x, _tree->center.y, _body->getId(), _tree->rigidBodies->size());
        _tree->rigidBodies->insert(mapBody(_body->getId(), _body));
//        printf("bodies x:%f, y:%f, body: %s -> after insert size: %lu\n", _tree->center.x, _tree->center.y, _body->getId(), _tree->rigidBodies->size());
    }
    
    // 0 - -
    if (inMinusMinus(_tree, _body)) {
        this->insertObject(_tree->child[0], _body);
    }

    // 1 + -
    if (inPlusMinus(_tree, _body)) {
        this->insertObject(_tree->child[1], _body);
    }
    
    // 2 - +
    if (inMinusPlus(_tree, _body)) {
        this->insertObject(_tree->child[2], _body);
    }
    
    // 3 + +
    if (inPlusPlus(_tree, _body)) {
        this->insertObject(_tree->child[3], _body);
    }
}

std::vector<RigidBody *> * QuadTree::getPossibleCollisions()
{
    return this->possibleCollisions;
}