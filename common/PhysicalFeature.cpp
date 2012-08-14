//
//  PhysicalFeature.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 14/08/12.
//
//

#include "PhysicalFeature.h"

PhysicalFeature::PhysicalFeature()
{
    this->position = NULL;
    this->acceleration = NULL;
    this->velocity = NULL;
    this->forceAccum = NULL;
    this->mass = 0;
    this->volume = 0;
    this->density = 0;
    this->inverseMass = 0;
    this->damping = 0;
}

PhysicalFeature::~PhysicalFeature()
{
    if (this->position) {
        delete this->position;
    }
    if (this->acceleration) {
        delete this->acceleration;
    }
    if (this->velocity) {
        delete this->velocity;
    }
    if (this->forceAccum) {
        delete this->forceAccum;
    }
    this->position = NULL;
    this->acceleration = NULL;
    this->velocity = NULL;
    this->forceAccum = NULL;
}

PhysicalFeature * PhysicalFeature::MakePhysicalFeature(real _mass,
                                                       real _volume,
                                                       real _density,
                                                       real _damping,
                                                       Vector3 * _acceleration,
                                                       Vector3 * _position,
                                                       Vector3 * _velocity,
                                                       Vector3 * _forceAccum)
{
    PhysicalFeature * pf = new PhysicalFeature();
    pf->mass = _mass;
    pf->volume = _volume;
    pf->density = _density;
    pf->inverseMass = ((real)1.0)/_mass; // TODO attention
    pf->damping = _damping;
    pf->acceleration = _acceleration;
    pf->velocity = _velocity;
    pf->position = _position;
    pf->forceAccum = _forceAccum;
    
    return pf;
}
