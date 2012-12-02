//
//  MainCollision.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  
//

#include "MainCollision.h"

using namespace std;

MainCollision::MainCollision(bool _useTree)
{
    this->tree = NULL;
    
    if (_useTree) {
        this->tree = new Octree();
    }
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

//!Delete rigid body in tree
void MainCollision::deleteObject(RigidBody * _body)
{
    if (this->tree) {
        this->tree->deleteObject(_body);
    }
}

//!Insert rigid body in tree
void MainCollision::insertObject(RigidBody * _body)
{
    if (this->tree) {
        this->tree->insertObject(_body);
    }
}

//!Update rigid body in tree
void MainCollision::updateObject(RigidBody * _body, real _duration)
{
    if (this->tree) {
        this->tree->insertObject(_body);
    //    this->tree->updateObject(_body);
    }
}

//!Identifies and generates contact between two rigidbody
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

//!Method responsible for identifying and generating contact between rigid bodies from the list of possible collisions
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
                if (body1 == body2) {
                    continue;
                }

                this->generateContact(body1, body2);
            }
        }

        listObjects->clear(); // remember to clean
    }
}

//!Method responsible solve the contacts generated
void MainCollision::solverContacts(real _duration)
{
    // solver contacts
    this->contactResolver->solverContacts(this->dataContacts->contacts, _duration);
    // clean contact list
    this->dataContacts->clearContacts();
}

//!Method responsible to update the system crash
void MainCollision::updateContacts(real _duration)
{
    this->generateContacts();
//    this->solverContacts(_duration);
}

//!Method responsible to update the system crash. If using tree, the method will be executed caught updateContacts, else will face all the objects to see if there are bumps
void MainCollision::updateContacts(std::vector<SimulatedObject *> * _listBody, SimulatedObject * _plan, real _duration)
{
    if (this->tree) {
        this->updateContacts(_duration);
    } else {
        RigidBody * body1 = NULL;
        RigidBody * body2 = NULL;

        for (int i=0; i<_listBody->size(); i++) {
            body1 = _listBody->at(i);
            if (body1->getTypeObject() == PLAN || !_plan) {
                continue;
            }
            this->generateContact(body1, _plan);
        }

        for (int i=0; i <_listBody->size(); i++) {
            body1 = _listBody->at(i);

            if (body1->getTypeObject() == PLAN) {
                continue;
            }

            for (int j=0; j<_listBody->size(); j++) {
                body2 = _listBody->at(j);
                if (body2->getTypeObject() == PLAN) {
                    continue;
                }
                if (body1 == body2) {
                    continue;
                }
                if (!body1->hasFiniteMass() && !body2->hasFiniteMass()) {
                    continue;
                }
                this->generateContact(body1, body2);
            }
        }
    }
    this->solverContacts(_duration);
}

//!Method responsible to clean tree
void MainCollision::cleanCollisions()
{
    if (this->tree) {
        this->tree->cleanLeaves();
    }
}