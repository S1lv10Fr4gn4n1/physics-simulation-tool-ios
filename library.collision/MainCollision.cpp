//
//  MainCollision.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#include "MainCollision.h"

using namespace std;

MainCollision * MainCollision::mainCollision = NULL;
MainCollision::MainCollision()
{
    QuadTree::getInstance()->buildQuadTree();
    this->dataContacts = new CollisionData();
    this->dataContacts->reset(MAX_CONTACTS);
}

MainCollision::~MainCollision()
{
    // TODO revise: clean collisions in dataContacs ?
    delete this->dataContacts;
    delete QuadTree::getInstance();
    
    this->dataContacts = NULL;
}

MainCollision * MainCollision::getInstance()
{
    if (!MainCollision::mainCollision) {
        MainCollision::mainCollision = new MainCollision();
    }
    
    return MainCollision::mainCollision;
}

void MainCollision::deleteObject(RigidBody * _body)
{
    QuadTree::getInstance()->deleteObject(_body);
}

void MainCollision::insertObject(RigidBody * _body)
{
    QuadTree::getInstance()->insertObject(_body);
}

void MainCollision::updateObject(RigidBody * _body, real _duration)
{
    _body->setDirty(false);
//    QuadTree::getInstance()->updateObject(_body);
    QuadTree::getInstance()->insertObject(_body);
}

void MainCollision::generateContact(RigidBody * _body1, RigidBody * _body2)
{
#if defined (_3D_)
    if (_body1->getTypeObject() == SPHERE && _body2->getTypeObject() == SPHERE) {
        CollisionSphere * sphere1 = new CollisionSphere(_body1, _body1->getRadius());
        CollisionSphere * sphere2 = new CollisionSphere(_body2, _body2->getRadius());
        CollisionDetector::sphereAndSphere(sphere1, sphere2, this->dataContacts);
        delete sphere1;
        delete sphere2;
        
    } else if (_body1->getTypeObject() == SPHERE && _body2->getTypeObject() == PLAN) {
        CollisionSphere * sphere = new CollisionSphere(_body1, _body1->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body2, _body2->getPosition(), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        delete sphere;
        delete plane;
        
    } else if (_body1->getTypeObject() == SPHERE && _body2->getTypeObject() == BOX) {
        CollisionSphere * sphere = new CollisionSphere(_body1, _body1->getRadius());
        CollisionBox * box = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndSphere(box, sphere, this->dataContacts);
        delete sphere;
        delete box;
        
    } else if (_body1->getTypeObject() == BOX && _body2->getTypeObject() == BOX) {
        CollisionBox * box1 = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionBox * box2 = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndBox(box1, box2, this->dataContacts);
        delete box1;
        delete box2;
        
    } else if (_body1->getTypeObject() == BOX && _body2->getTypeObject() == PLAN) {
        CollisionBox * box = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionPlane * plane = new CollisionPlane(_body2, _body2->getPosition(), 0.0f);
        CollisionDetector::boxAndHalfSpace(box, plane, this->dataContacts);
        delete box;
        delete plane;
        
    } else if (_body1->getTypeObject() == BOX && _body2->getTypeObject() == SPHERE) {
        CollisionBox * box = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionSphere * sphere = new CollisionSphere(_body2, _body2->getRadius());
        CollisionDetector::boxAndSphere(box, sphere, this->dataContacts);
        delete sphere;
        delete box;
        
    } else if (_body1->getTypeObject() == PLAN && _body2->getTypeObject() == SPHERE) {
        CollisionSphere * sphere = new CollisionSphere(_body2, _body2->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body1, _body1->getPosition(), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;
        
    } else if (_body1->getTypeObject() == PLAN && _body2->getTypeObject() == BOX) {
        CollisionPlane * plane = new CollisionPlane(_body1, _body1->getPosition(), 0.0f);
        CollisionBox * box = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndHalfSpace(box, plane, this->dataContacts);
        delete box;
        delete plane;
    }
#else
    if (_body1->getTypeObject() == CIRCLE && _body2->getTypeObject() == CIRCLE) {
        CollisionSphere * sphere1 = new CollisionSphere(_body1, _body1->getRadius());
        CollisionSphere * sphere2 = new CollisionSphere(_body2, _body2->getRadius());
        CollisionDetector::sphereAndSphere(sphere1, sphere2, this->dataContacts);
        delete sphere1;
        delete sphere2;
        
    } else if (_body1->getTypeObject() == CIRCLE && _body2->getTypeObject() == PLAN) {
        CollisionSphere * sphere = new CollisionSphere(_body1, _body1->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body2, _body2->getPosition(), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        delete sphere;
        delete plane;
        
    } else if (_body1->getTypeObject() == CIRCLE && _body2->getTypeObject() == SQUARE) {
        CollisionSphere * sphere = new CollisionSphere(_body1, _body1->getRadius());
        CollisionBox * box = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndSphere(box, sphere, this->dataContacts);
        delete sphere;
        delete box;
        
    } else if (_body1->getTypeObject() == SQUARE && _body2->getTypeObject() == SQUARE) {
        CollisionBox * box1 = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionBox * box2 = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndBox(box1, box2, this->dataContacts);
        delete box1;
        delete box2;
        
    } else if (_body1->getTypeObject() == SQUARE && _body2->getTypeObject() == PLAN) {
        CollisionBox * box = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionPlane * plane = new CollisionPlane(_body2, _body2->getPosition(), 0.0f);
        CollisionDetector::boxAndHalfSpace(box, plane, this->dataContacts);
        delete box;
        delete plane;
        
    } else if (_body1->getTypeObject() == SQUARE && _body2->getTypeObject() == CIRCLE) {
        CollisionBox * box = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionSphere * sphere = new CollisionSphere(_body2, _body2->getRadius());
        CollisionDetector::boxAndSphere(box, sphere, this->dataContacts);
        delete sphere;
        delete box;
        
    } else if (_body1->getTypeObject() == PLAN && _body2->getTypeObject() == CIRCLE) {
        CollisionSphere * sphere = new CollisionSphere(_body2, _body2->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body1, _body1->getPosition(), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;
        
    } else if (_body1->getTypeObject() == PLAN && _body2->getTypeObject() == SQUARE) {
        CollisionPlane * plane = new CollisionPlane(_body1, _body1->getPosition(), 0.0f);
        CollisionBox * box = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndHalfSpace(box, plane, this->dataContacts);
        delete box;
        delete plane;
    }
#endif
}

void MainCollision::generateContacts()
{
    vector<RigidBody *> * listObjects = QuadTree::getInstance()->getPossibleCollisions();
    
    if (listObjects && listObjects->size() > 0) {
        printf("collision: %lu\n", listObjects->size());
        RigidBody * body1 = NULL;
        RigidBody * body2 = NULL;
        
        for (int i=0; i<listObjects->size(); i++) {
            body1 = listObjects->at(i);
            for (int j=0; j<listObjects->size(); j++) {
                body2 = listObjects->at(j);
                if (body1 == body2 || body2->isDirty()) {
                    continue;
                }
                
                this->generateContact(body1, body2);
                body1->setDirty(true);
                body2->setDirty(true);
            }
        }

        listObjects->clear(); // remember to clean
    }
}

void MainCollision::solverContacts(real _duration)
{
    ContactResolver::getInstance()->solverContacts(this->dataContacts->contacts, _duration);
    this->dataContacts->reset(MAX_CONTACTS);
}

void MainCollision::updateContacts(real _duration)
{
    this->generateContacts();
    this->solverContacts(_duration);
}

void MainCollision::cleanCollisions()
{
    QuadTree::getInstance()->cleanLeaves();
}