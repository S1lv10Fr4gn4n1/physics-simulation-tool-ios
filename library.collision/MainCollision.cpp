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
static const unsigned MAX_CONTACTS = 256;

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
    QuadTree::getInstance()->updateObject(_body);
}

void MainCollision::generateContact(RigidBody * _bodies[2])
{
    
#if defined (_3D_)
    if (_bodies[0]->getTypeObject() == SPHERE && _bodies[1]->getTypeObject() == SPHERE) {
        Sphere * sphere1 = new Sphere(_bodies[0], _bodies[0]->getRadius());
        Sphere * sphere2 = new Sphere(_bodies[1], _bodies[1]->getRadius());
        sphere1->calculateInternals();
        sphere2->calculateInternals();
        CollisionDetector::sphereAndSphere(sphere1, sphere2, this->dataContacts);
        
        delete sphere1;
        delete sphere2;
        
    } else if (_bodies[0]->getTypeObject() == SPHERE && _bodies[1]->getTypeObject() == PLAN) {
        Sphere * sphere = new Sphere(_bodies[0], _bodies[0]->getRadius());
        Plane * plane = new Plane(_bodies[1], new Vector3(0.0f, 1.0f, 0.0f), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;

    } else if (_bodies[0]->getTypeObject() == PLAN && _bodies[1]->getTypeObject() == SPHERE) {
        Sphere * sphere = new Sphere(_bodies[1], _bodies[1]->getRadius());
        Plane * plane = new Plane(_bodies[0], new Vector3(0.0f, -0.9f, 0.0f), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;
        
    } else if (_bodies[0]->getTypeObject() == BOX && _bodies[1]->getTypeObject() == BOX) {
        
    } else if (_bodies[0]->getTypeObject() == BOX && _bodies[1]->getTypeObject() == PLAN) {
        
    } else if ((_bodies[0]->getTypeObject() == SPHERE && _bodies[1]->getTypeObject() == BOX) &&
               (_bodies[0]->getTypeObject() == BOX &&_bodies[1]->getTypeObject() == SPHERE)) {
    }
#else
    if (_bodies[0]->getTypeObject() == CIRCLE && _bodies[1]->getTypeObject() == CIRCLE) {
        Sphere * sphere1 = new Sphere(_bodies[0], _bodies[0]->getRadius());
        Sphere * sphere2 = new Sphere(_bodies[1], _bodies[1]->getRadius());
        sphere1->calculateInternals();
        sphere2->calculateInternals();
        CollisionDetector::sphereAndSphere(sphere1, sphere2, this->dataContacts);
        
        delete sphere1;
        delete sphere2;
        
    } else if (_bodies[0]->getTypeObject() == CIRCLE && _bodies[1]->getTypeObject() == PLAN) {
        Sphere * sphere = new Sphere(_bodies[0], _bodies[0]->getRadius());
        Plane * plane = new Plane(_bodies[1], new Vector3(0.0f, 1.0f, 0.0f), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;
        
    } else if (_bodies[0]->getTypeObject() == PLAN && _bodies[1]->getTypeObject() == CIRCLE) {
        Sphere * sphere = new Sphere(_bodies[1], _bodies[1]->getRadius());
        Plane * plane = new Plane(_bodies[0], new Vector3(0.0f, -0.9f, 0.0f), 0.0f);
        CollisionDetector::sphereAndHalfSpace(sphere, plane, this->dataContacts);
        
        delete sphere;
        delete plane;
        
    } else if (_bodies[0]->getTypeObject() == SQUARE && _bodies[1]->getTypeObject() == SQUARE) {
        
    } else if (_bodies[0]->getTypeObject() == SQUARE && _bodies[1]->getTypeObject() == PLAN) {
        
    } else if ((_bodies[0]->getTypeObject() == CIRCLE && _bodies[1]->getTypeObject() == SQUARE) &&
               (_bodies[0]->getTypeObject() == SQUARE && _bodies[1]->getTypeObject() == CIRCLE)) {
    }
#endif
    
    delete _bodies;
    _bodies = NULL;
}

void MainCollision::generateContacts()
{
    vector<RigidBody **> * listObjects = QuadTree::getInstance()->getPossibleCollisions();
    
    if (listObjects && listObjects->size() > 0) {
        RigidBody ** bodies = NULL;
        for (int i=0; i<listObjects->size(); i++) {
            bodies = listObjects->at(i);
            if (bodies[0] && bodies[1]) {
                this->generateContact(bodies);
            } else {
                printf("ja se foi :D \n");
            }
        }
//        delete bodies;
        listObjects->clear(); // remember to clean
    }
}

void MainCollision::solverContacts(real _duration)
{
    ContactResolver::getInstance()->solverContacts(this->dataContacts->contacts, _duration);
    this->dataContacts->reset(MAX_CONTACTS); // TODO revise: is a good value ?
}

void MainCollision::updateContacts(real _duration)
{
    this->generateContacts();
    this->solverContacts(_duration);
}