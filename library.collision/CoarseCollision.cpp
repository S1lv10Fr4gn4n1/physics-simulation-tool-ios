//
//  CoarseCollision.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 23/08/12.
//
//

// TODO revise: depth of tree and width/height of scene
#define DEPTH_TREE 5
#define WIDTH_SCENE 4.0f

#include "CoarseCollision.h"

bool less(real k1,real k2) {
    return k1 < k2;
}

template<class BoundingVolumeClass>
BVHNode<BoundingVolumeClass>::BVHNode(BVHNode * _parent, const BoundingVolumeClass * _volume, RigidBody * _body)
{
    this->parent = _parent;
    this->volume = _volume;
    this->body = _body;
    this->children[0] = this->children[1] = NULL;
}

template<class BoundingVolumeClass>
BVHNode<BoundingVolumeClass>::~BVHNode<BoundingVolumeClass>()
{
    // ff we don’t have a parent, then we ignore the sibling processing.
    if (this->parent) {
        // find our sibling.
        BVHNode<BoundingVolumeClass> * sibling;
        
        if (this->parent->children[0] == this) {
            sibling = this->parent->children[1];
        } else {
            sibling = this->parent->children[0];
        }
        
        // write its data to our parent.
        this->parent->volume = sibling->volume;
        this->parent->body = sibling->body;
        this->parent->children[0] = sibling->children[0];
        this->parent->children[1] = sibling->children[1];
        
        // delete the sibling (we blank its parent and
        // children to avoid processing/deleting them).
        sibling->parent = NULL;
        sibling->body = NULL;
        sibling->children[0] = NULL;
        sibling->children[1] = NULL;
        delete sibling;
        
        // recalculate the parent’s bounding volume.
        this->parent->recalculateBoundingVolume();
    }
    
    // delete our children (again we remove their parent data so
    // we don’t try to process their siblings as they are deleted).
    if (this->children[0]) {
        this->children[0]->parent = NULL;
        delete this->children[0];
    }
    
    if (this->children[1]) {
        this->children[1]->parent = NULL;
        delete this->children[1];
    }
}

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::isLeaf() const
{
    return (this->body != NULL);
}

template<class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass> * _other) const
{
    return this->volume->overlaps(_other->volume);
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact * _contacts, unsigned _limit) const
{
    // early out if we don’t have the room for contacts, or if we’re a leaf node.
    if (this->isLeaf() || _limit == 0) {
        return 0;
    }
    
    // get the potential contacts of one of our children with the other.
    return this->children[0]->getPotentialContactsWith(this->children[1], _contacts, _limit);
}

template<class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(const BVHNode<BoundingVolumeClass> * _other,
                                                                PotentialContact * _contacts,
                                                                unsigned _limit) const
{
    // early-out if we don’t overlap or if we have no room to report contacts.
    if (!this->overlaps(_other) || _limit == 0) {
        return 0;
    }
    
    // if we’re both at leaf nodes, then we have a potential contact.
    if (this->isLeaf() && _other->isLeaf()) {
        _contacts->body[0] = this->body;
        _contacts->body[1] = _other->body;
        return 1;
    }
    
    // determine which node to descend into. If either is a leaf, then we descend the other.
    // If both are branches, then we use the one with the largest size.
    if (_other->isLeaf() || (!this->isLeaf() && this->volume->getSize() >= _other->volume->getSize())) {
        // recurse into ourself.
        unsigned count = this->children[0]->getPotentialContactsWith(_other, _contacts, _limit);
        
        // check whether we have enough slots to do the other side too.
        if (_limit > count) {
            return count + this->children[1]->getPotentialContactsWith(_other, _contacts + count, _limit - count);
        } else {
            return count;
        }
    } else {
        // recurse into the other node.
        unsigned count = this->getPotentialContactsWith(_other->children[0], _contacts, _limit);
        
        // check whether we have enough slots to do the other side too.
        if (_limit > count) {
            return count + this->getPotentialContactsWith(_other->children[1], _contacts+count, _limit-count);
        } else {
            return count;
        }
    }
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::insert(RigidBody * _newBody, const BoundingVolumeClass * _newVolume)
{
    // if we are a leaf, then the only option is to spawn two
    // new children and place the new body in one.
    if (this->isLeaf()) {
        // child one is a copy of us.
        this->children[0] = new BVHNode<BoundingVolumeClass>(this, this->volume, this->body);
        
        // child two holds the new body
        this->children[1] = new BVHNode<BoundingVolumeClass>(this, _newVolume, _newBody);
        
        // and we now loosen the body (we’re no longer a leaf).
        this->body = NULL;
        
        // we need to recalculate our bounding volume.
        this->recalculateBoundingVolume();
        
    } else {
        // otherwise we need to work out which child gets to keep
        // the inserted body. We give it to whoever would grow the
        // least to incorporate it.
        if (this->children[0]->volume->getGrowth(_newVolume) < this->children[1]->volume->getGrowth(_newVolume)) {
            this->children[0]->insert(_newBody, _newVolume);
        } else {
            this->children[1]->insert(_newBody, _newVolume);
        }
    }
}

template<class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume()
{
    if (this->isLeaf()) {
        return;
    }
    
    // use the bounding volume combining constructor.
    this->volume = BoundingVolumeClass(this->children[0]->volume, this->children[1]->volume);
                                 
    // recurse up the tree
    if (this->parent) {
        this->parent->recalculateBoundingVolume(true);
    }
}




BoundingSphere::BoundingSphere(Vector3 * _center, real _radius)
{
    this->center = _center;
    this->radius = _radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere * _one, const BoundingSphere * _two)
{
    Vector3 * centerOffset = *_two->center - _one->center;
    real distance = centerOffset->squareMagnitude();
    real radiusDiff = _two->radius - _one->radius;
    
    // check if the larger sphere encloses the small one
    if (radiusDiff*radiusDiff >= distance) {
        if (_one->radius > _two->radius) {
            this->center = _one->center;
            this->radius = _one->radius;
        } else {
            this->center = _two->center;
            this->radius = _two->radius;
        }
    } else {
        // otherwise we need to work with partially overlapping spheres
        distance = real_sqrt(distance);
        this->radius = (distance + _one->radius + _two->radius) * ((real)0.5);
        
        // The new centre is based on one's centre, moved towards
        // two's centre by an ammount proportional to the spheres'
        // radii.
        this->center = _one->center;
        if (distance > 0) {
            *this->center += *centerOffset * ((this->radius - _one->radius)/distance);
        }
    }
}

bool BoundingSphere::overlaps(const BoundingSphere * _other) const
{
    Vector3 * diff = *this->center - _other->center;
    real distanceSquared = diff->squareMagnitude();
    return distanceSquared < (this->radius + _other->radius) * (this->radius + _other->radius);
}

real BoundingSphere::getGrowth(const BoundingSphere * _other) const
{
    BoundingSphere * newSphere = new BoundingSphere(this, _other);
    
    // We return a value proportional to the change in surface
    // area of the sphere.
    return newSphere->radius*newSphere->radius - radius*radius;
}

real BoundingSphere::getSize() const
{
    return ((real)1.333333) * R_PI * radius * radius * radius;
}



// TODO revise: if working 3D, making octree
QuadTree * QuadTree::quadTree = NULL;
QuadTree * QuadTree::getInstance()
{
    if (!QuadTree::quadTree) {
        QuadTree::quadTree = new QuadTree();
    }
    
    return QuadTree::quadTree;
}

QuadTree::QuadTree()
{
    this->listObjects = new std::vector<RigidBody **>();
}

QuadTree::~QuadTree()
{
    if (this->parent) {
        delete this->parent;
    }
    
    this->parent = NULL;
}

void QuadTree::buildQuadTree()
{
    Vector3 * center = Vector3::MakeVector3(0.0f, 0.0f);
    this->parent = this->buildQuadTree(center, WIDTH_SCENE, DEPTH_TREE);
    delete center;
    center = NULL;
}

QuadTreeNode * QuadTree::buildQuadTree(Vector3 * _center, real _halfWidth, int _stopDepth)
{
    if (_stopDepth < 0) {
        return NULL;
    } else {
        QuadTreeNode * node = new QuadTreeNode();
        *node->center = _center;
        node->halfWidth = _halfWidth;
        node->rigidBody = NULL;
        
        Vector3 * offset = NULL;
        Vector3 * offsetAux = NULL;
        float step = _halfWidth * 0.5f;
        
        for (int i = 0; i < 4; i++) {
            offset = new Vector3();
            offset->x = ((i & 1) ? step : -step);
            offset->y = ((i & 2) ? step : -step);
            
            offsetAux = *_center + offset;
            node->child[i] = this->buildQuadTree(offsetAux, step, _stopDepth - 1);
            
            delete offset;
            offset = NULL;
            delete offsetAux;
            offsetAux = NULL;
        }

        return node;
    }
}

void QuadTree::updateObject(RigidBody * _body)
{
    this->deleteObject(this->parent, _body);
    this->insertObject(this->parent, _body);
}

void QuadTree::deleteObject(RigidBody * _body)
{
    this->deleteObject(this->parent, _body);
}

void QuadTree::deleteObject(QuadTreeNode * _tree, RigidBody * _body)
{
    if (!_tree) {
        return;
    }
    
    if (_tree->rigidBody && _tree->rigidBody == _body) {
        _tree->rigidBody = NULL;
    } else if (less(_body->getPosition()->x, _tree->center->x) &&  less( _body->getPosition()->y, _tree->center->y)) {
        this->deleteObject(_tree->child[0], _body);
    } else if (!less(_body->getPosition()->x, _tree->center->x) && less(_body->getPosition()->y, _tree->center->y)) {
        this->deleteObject(_tree->child[1], _body);
    } else if (less(_body->getPosition()->x, _tree->center->x) &&  !less(_body->getPosition()->y, _tree->center->y)) {
        this->deleteObject(_tree->child[2], _body);
    } else if (!less(_body->getPosition()->x, _tree->center->x) && !less(_body->getPosition()->y, _tree->center->y)) {
        this->deleteObject(_tree->child[3], _body);
    }
}

void QuadTree::insertObject(RigidBody * _body)
{
    this->insertObject(this->parent, _body);
}

RigidBody ** getBodies(RigidBody * _body1, RigidBody * _body2)
{
    RigidBody ** bodies = new RigidBody*[2];
    bodies[0] = _body1;
    bodies[1] = _body2;
    
    return bodies;
}

void QuadTree::insertObject(QuadTreeNode * _tree, RigidBody * _body) {
    // TODO revise: consider the radius?  (_tree->halfWidth : _body->radius)
    if (!_tree) {
        return;
    } else if (less(_body->getPosition()->x, _tree->center->x) && less(_body->getPosition()->y, _tree->center->y)) {
        this->insertObject(_tree->child[0], _body);
        if (!_tree->child[0]) {
            if (_tree->rigidBody) {
                this->listObjects->push_back(getBodies(_tree->rigidBody, _body));
            }
            _tree->rigidBody = _body;
        }
    } else if (!less(_body->getPosition()->x, _tree->center->x) && less(_body->getPosition()->y, _tree->center->y)) {
        this->insertObject(_tree->child[1], _body);
        if (!_tree->child[1]) {
            if (_tree->rigidBody) {
                this->listObjects->push_back(getBodies(_tree->rigidBody, _body));
            }
            _tree->rigidBody = _body;
        }
    } else if (less(_body->getPosition()->x, _tree->center->x) &&  !less(_body->getPosition()->y, _tree->center->y)) {
        this->insertObject(_tree->child[2], _body);
        if (!_tree->child[2]) {
            if (_tree->rigidBody) {
                this->listObjects->push_back(getBodies(_tree->rigidBody, _body));
            }
            _tree->rigidBody = _body;
        }
    } else if (!less(_body->getPosition()->x, _tree->center->x) && !less(_body->getPosition()->y, _tree->center->y)) {
        this->insertObject(_tree->child[3], _body);
        if (!_tree->child[3]) {
            if (_tree->rigidBody) {
                this->listObjects->push_back(getBodies(_tree->rigidBody, _body));
            }
            _tree->rigidBody = _body;
        }
    }
}

std::vector<RigidBody **> * QuadTree::getPossibleCollisions()
{
    return this->listObjects;
}