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
}

MainCollision::~MainCollision()
{
    delete QuadTree::getInstance();
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

void MainCollision::generateContacts()
{
    
}

void MainCollision::solverContacts()
{
    
}

void MainCollision::updateContacts()
{
    vector<RigidBody *> * listObjects = QuadTree::getInstance()->getPossibleCollisions();
    if (listObjects) {
        printf("objects collision: %lu\n", listObjects->size());
        listObjects->clear(); // TODO remember to clean
    }
}