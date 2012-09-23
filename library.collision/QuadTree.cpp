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

bool less(real k1,real k2) {
    return k1 < k2;
}

bool inPlusPlus(QuadTreeNode * _tree, RigidBody * _body)
{
    if (!less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;

    if (!less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if (!less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if (!less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;

    return false;
}

bool inPlusMinus(QuadTreeNode * _tree, RigidBody * _body)
{
    if (!less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
         less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;

    if (!less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
         less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if (!less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
         less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if (!less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
         less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;
    
    return false;
}

bool inMinusMinus(QuadTreeNode * _tree, RigidBody * _body)
{
    if (less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
        less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;
    if (less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
        less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;

    if (less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
        less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if (less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
        less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;
    
    return false;
}

bool inMinusPlus(QuadTreeNode * _tree, RigidBody * _body)
{
    if ( less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;

    if ( less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if ( less(_body->getPosition().x + _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y - _body->getHalfSize().y, _tree->center.y))
        return true;
    
    if ( less(_body->getPosition().x - _body->getHalfSize().x, _tree->center.x) &&
        !less(_body->getPosition().y + _body->getHalfSize().y, _tree->center.y))
        return true;
 
    return false;
}

bool radiusReachesObject(QuadTreeNode * _tree, RigidBody * _body)
{
    if ((_tree->center.x + _tree->halfWidth >= _body->getPosition().x + _body->getHalfSize().x ||
         _tree->center.x + _tree->halfWidth >= _body->getPosition().x - _body->getHalfSize().x) &&
        
        (_tree->center.x - _tree->halfWidth <= _body->getPosition().x + _body->getHalfSize().x ||
         _tree->center.x - _tree->halfWidth <= _body->getPosition().x - _body->getHalfSize().x) &&
        
        (_tree->center.y + _tree->halfWidth >= _body->getPosition().y + _body->getHalfSize().y ||
         _tree->center.y + _tree->halfWidth >= _body->getPosition().y - _body->getHalfSize().y) &&
        
        (_tree->center.y - _tree->halfWidth <= _body->getPosition().y + _body->getHalfSize().y &&
         _tree->center.y - _tree->halfWidth <= _body->getPosition().y - _body->getHalfSize().y)) {
        return true;
    }

    return false;
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

void QuadTree::updateObject(RigidBody * _body)
{
    this->deleteObject(_body);
    this->insertObject(_body);
}

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