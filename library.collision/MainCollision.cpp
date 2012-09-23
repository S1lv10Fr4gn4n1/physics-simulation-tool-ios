//
//  MainCollision.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#include "MainCollision.h"

using namespace std;

MainCollision::MainCollision()
{
    this->tree = new Octree();
    this->contactResolver = new ContactResolver();
    this->dataContacts = new CollisionData();
}

MainCollision::~MainCollision()
{
    delete this->dataContacts;
    delete this->tree;
    delete this->contactResolver;
    
    this->dataContacts = NULL;
    this->tree = NULL;
    this->contactResolver = NULL;
}

void MainCollision::deleteObject(RigidBody * _body)
{
    this->tree->deleteObject(_body);
}

void MainCollision::insertObject(RigidBody * _body)
{
    this->tree->insertObject(_body);
}

void MainCollision::updateObject(RigidBody * _body, real _duration)
{
    _body->setDirty(false);
    this->tree->insertObject(_body);
}

void MainCollision::generateContact(RigidBody * _body1, RigidBody * _body2)
{
    Vector3 normalPlane(0.0f, 1.0f, 0.0f);
    real offset = 0.0f;

#if defined (_3D_)
    if (_body1->getTypeObject() == SPHERE && _body2->getTypeObject() == SPHERE) {
        CollisionSphere * sphere1 = new CollisionSphere(_body1, _body1->getRadius());
        CollisionSphere * sphere2 = new CollisionSphere(_body2, _body2->getRadius());
        CollisionDetector::sphereAndSphere(sphere1, sphere2, this->dataContacts);
        delete sphere1;
        delete sphere2;
        
    } else if (_body1->getTypeObject() == SPHERE && _body2->getTypeObject() == PLAN) {
        CollisionSphere * sphere = new CollisionSphere(_body1, _body1->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body2, normalPlane, offset);
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
        CollisionPlane * plane = new CollisionPlane(_body2, normalPlane, offset);
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
        CollisionPlane * plane = new CollisionPlane(_body1, normalPlane, offset);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        delete sphere;
        delete plane;
        
    } else if (_body1->getTypeObject() == PLAN && _body2->getTypeObject() == BOX) {
        CollisionPlane * plane = new CollisionPlane(_body1, normalPlane, offset);
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
        Vector3 * normalPlane = new Vector3(0.0f, 1.0f, 0.0f);
        real offset = 0.0f;
        CollisionSphere * sphere = new CollisionSphere(_body1, _body1->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body2, normalPlane, offset);
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
        Vector3 * normalPlane = new Vector3(0.0f, 1.0f, 0.0f);
        real offset = 0.0f;
        CollisionBox * box = new CollisionBox(_body1, _body1->getHalfSize());
        CollisionPlane * plane = new CollisionPlane(_body2, normalPlane, offset);
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
        Vector3 * normalPlane = new Vector3(0.0f, 1.0f, 0.0f);
        real offset = 0.0f;
        CollisionSphere * sphere = new CollisionSphere(_body2, _body2->getRadius());
        CollisionPlane * plane = new CollisionPlane(_body1, normalPlane, offset);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;
        
    } else if (_body1->getTypeObject() == PLAN && _body2->getTypeObject() == SQUARE) {
        Vector3 * normalPlane = new Vector3(0.0f, 1.0f, 0.0f);
        real offset = 0.0f;
        CollisionPlane * plane = new CollisionPlane(_body1, normalPlane, offset);
        CollisionBox * box = new CollisionBox(_body2, _body2->getHalfSize());
        CollisionDetector::boxAndHalfSpace(box, plane, this->dataContacts);
        delete box;
        delete plane;
    }
#endif
}

void MainCollision::generateContacts()
{
    vector<RigidBody *> * listObjects = this->tree->getPossibleCollisions();
    
//    printf("collision: %lu\n", listObjects->size());
    if (listObjects && listObjects->size() > 0) {
        RigidBody * body1 = NULL;
        RigidBody * body2 = NULL;

        for (int i=0; i<listObjects->size(); i++) {
            body1 = listObjects->at(i);
            for (int j=0; j<listObjects->size(); j++) {
                body2 = listObjects->at(j);
//
//                printf("body1: %s, type %s\n", body1->getId(), body1->getTypeObject() == PLAN ? "plan" : body1->getTypeObject() == SPHERE ? "sphere" : "box");
//                printf("body2: %s, type %s\n", body2->getId(), body2->getTypeObject() == PLAN ? "plan" : body2->getTypeObject() == SPHERE ? "sphere" : "box");
//                if (body1 == body2 || body2->isDirty()) {
//                    continue;
//                }

                this->generateContact(body1, body2);
//                body1->setDirty(true);
//                body2->setDirty(true);
            }
        }

        listObjects->clear(); // remember to clean
    }
}

void MainCollision::solverContacts(real _duration)
{
    this->contactResolver->solverContacts(this->dataContacts->contacts, _duration);
    this->dataContacts->clearContacts();
}

void MainCollision::updateContacts(real _duration)
{
    this->generateContacts();
    this->solverContacts(_duration);
}

void MainCollision::cleanCollisions()
{
    this->tree->cleanLeaves();
}