//
//  FineCollision.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 26/08/12.
//
//

#include "FineCollision.h"


CollisionData::CollisionData()
{
    this->contacts = new std::vector<Contact *>();
    
    this->contactsCount = 0;
    this->contactsLeft = 0;
    this->restitution = 0.0f;
    this->friction = 0.0f;
}

CollisionData::~CollisionData()
{
    this->clearContacts();
    delete this->contacts;
    this->contacts = NULL;
}

void CollisionData::clearContacts()
{
    for (int i=0; i<this->contacts->size(); i++) {
        delete this->contacts->at(i);
    }
    
    this->contacts->clear();
}

void CollisionData::addContacts(unsigned _num)
{
    this->contactsLeft -= _num;
    this->contactsCount += _num;
}

void CollisionData::reset(unsigned _maxContacts)
{
    this->contactsLeft = _maxContacts;
    this->contactsCount = 0;
        
    this->clearContacts();
}




Primitive::Primitive() {
    this->transform = new Matrix4();
    this->offset = new Matrix4();
}

Primitive::~Primitive() {
    delete this->transform;
    delete this->offset;
    
    this->transform = NULL;
    this->offset = NULL;
}

Vector3 * Primitive::getAxis(unsigned _index) const
{
    return this->transform->getAxisVector(_index);
}

void Primitive::calculateInternals()
{
    this->transform = *this->body->getTransformMatrix() * offset;
}


unsigned CollisionDetector::sphereAndSphere(const Sphere * _one, const Sphere * _two, CollisionData * _data)
{
    if (_data->contactsLeft <= 0) {
        return 0;
    }
        
    // sphere positions
    Vector3 * positionOne = _one->getAxis(3);
    Vector3 * positionTwo = _two->getAxis(3);
    
    // find the vector between the objects
    Vector3 * midline = *positionOne - positionTwo;
    real size = midline->magnitude();
    
    // see if it is large enough.
    if (size <= 0.0f || size >= _one->radius + _two->radius) {
        return 0;
    }
    
    // manually create the normal, because we have the size to hand.
    Vector3 * normal = *midline * (((real)1.0)/size);
    
    Contact * contact = new Contact();
    contact->contactNormal = normal;
    contact->contactPoint = *positionOne + (*midline * (real)0.5);
    contact->penetration = (_one->radius + _two->radius - size);
    contact->body[0] = _one->body;
    contact->body[1] = _two->body;
    contact->restitution = _data->restitution;
    contact->friction = _data->friction;
    
    _data->contacts->push_back(contact);
    _data->addContacts(1);
    
    return 1;
}

unsigned CollisionDetector::sphereAndHalfSpace(const Sphere * _sphere, const Plane * _plane, CollisionData * _data)
{
    if (_data->contactsLeft <= 0) {
        return 0;
    }
    
    // sphere position
    Vector3 * position = _sphere->getAxis(3);
    
    // find the distance from the plane.
    real distance = *_plane->direction * position - _sphere->radius - _plane->offset;
    if (distance >= 0) {
        return 0;
    }
    
    Contact * contact = new Contact();
    contact->contactNormal = _plane->direction; // it has a normal in the plane direction.
    contact->penetration = -distance;
    contact->contactPoint = *position - *_plane->direction * (distance + _sphere->radius);
    contact->body[0] = _sphere->body;
    contact->body[1] = NULL;
    contact->restitution = _data->restitution;
    contact->friction = _data->friction;
    
    _data->contacts->push_back(contact);
    _data->addContacts(1);
    
    return 1;
}

unsigned CollisionDetector::sphereAndTruePlane(const Sphere * _sphere, const Plane * _plane, CollisionData * _data)
{
    if (_data->contactsLeft <= 0) {
        return 0;
    }
    
    // sphere position.
    Vector3 * position = _sphere->getAxis(3);
    
    // find the distance from the plane.
    real centerDistance = *_plane->direction * position - _plane->offset;
    
    // check if we’re within radius.
    if (centerDistance * centerDistance > _sphere->radius * _sphere->radius) {
        return 0;
    }
    
    // check which side of the plane we’re on.
    Vector3 * normal = _plane->direction;
    real penetration = -centerDistance;
    
    if (centerDistance < 0) {
        *normal *= -1.0f;
        penetration = -penetration;
    }
    
    penetration += _sphere->radius;
    
    Contact * contact = new Contact();
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = *position - *_plane->direction * centerDistance;
    contact->body[0] = _sphere->body;
    contact->body[1] = NULL;
    contact->restitution = _data->restitution;
    contact->friction = _data->friction;
    
    _data->contacts->push_back(contact);
    _data->addContacts(1);
    
    return 1;
}

unsigned CollisionDetector::boxAndSphere(const Box * _box, const Sphere * _sphere, CollisionData * _data)
{
    // transform the center of the sphere into box coordinates
    Vector3 * center = _sphere->getAxis(3);
    Vector3 * relCenter = _box->transform->transformInverse(center);
    
    // early-out check to see if we can exclude the contact
    if (real_abs(relCenter->x) - _sphere->radius > _box->halfSize->x ||
        real_abs(relCenter->y) - _sphere->radius > _box->halfSize->y ||
        real_abs(relCenter->z) - _sphere->radius > _box->halfSize->z) {
        return 0;
    }
    
    Vector3 * closestPoint = new Vector3();
    real distance;
    
    // clamp each coordinate to the box.
    distance = relCenter->x;
    if (distance > _box->halfSize->x) {
        distance = _box->halfSize->x;
    }
    if (distance < -_box->halfSize->x) {
        distance = -_box->halfSize->x;
    }
    closestPoint->x = distance;
    distance = relCenter->y;
    if (distance > _box->halfSize->y) {
        distance = _box->halfSize->y;
    }
    if (distance < -_box->halfSize->y) {
        distance = -_box->halfSize->y;
    }
    closestPoint->y = distance;
    distance = relCenter->z;
    if (distance > _box->halfSize->z) {
        distance = _box->halfSize->z;
    }
    if (distance < -_box->halfSize->z) {
        distance = -_box->halfSize->z;
    }
    closestPoint->z = distance;
    
    // check we’re in contact.
    distance = (*closestPoint - relCenter)->squareMagnitude();
    if (distance > _sphere->radius * _sphere->radius) {
        return 0;
    }
    
    Vector3 * closestPointWorld = _box->transform->transform(closestPoint);
    
    Contact * contact = new Contact();
    contact->contactNormal = (*center - closestPointWorld);
    contact->contactNormal->normalize();
    contact->contactPoint = closestPointWorld;
    contact->penetration = _sphere->radius - real_sqrt(distance);
    contact->body[0] = _box->body;
    contact->body[1] = _sphere->body;
    contact->restitution = _data->restitution;
    contact->friction = _data->friction;
    
    _data->contacts->push_back(contact);
    _data->addContacts(1);
    
    return 1;
}

real transformToAxis(const Box * _box, const Vector3 * _axis)
{
    return _box->halfSize->x * real_abs(*_axis * _box->getAxis(0)) +
           _box->halfSize->y * real_abs(*_axis * _box->getAxis(1)) +
           _box->halfSize->z * real_abs(*_axis * _box->getAxis(2));
}

bool overlapOnAxis(const Box * _one, const Box * _two, const Vector3 * _axis)
{
    // project the half-size of one onto axis.
    real oneProject = transformToAxis(_one, _axis);
    real twoProject = transformToAxis(_two, _axis);
    
    // find the vector between the two centers.
    Vector3 * toCenter = *_two->getAxis(3) - _one->getAxis(3);
    
    // project this onto the axis.
    real distance = real_abs(*toCenter * _axis);
    
    // check for overlap.
    return (distance < oneProject + twoProject);
}

unsigned CollisionDetector::boxAndPoint(const Box * _box, Vector3 * _point,  CollisionData * _data)
{
    // transform the point into box coordinates.
    Vector3 * relPoint = _box->transform->transformInverse(_point);
    Vector3 * normal = new Vector3();
    
    // check each axis, looking for the axis on which the penetration is least deep.
    real minDepth = _box->halfSize->x - real_abs(relPoint->x);
    
    if (minDepth < 0) {
        return 0;
    }
    
    normal = *_box->getAxis(0) * ((relPoint->x < 0) ? -1 : 1);
    real depth = _box->halfSize->y - real_abs(relPoint->y);
    
    if (depth < 0) {
        return 0;
    } else if (depth < minDepth) {
        minDepth = depth;
        normal = *_box->getAxis(1) * ((relPoint->y < 0) ? -1 : 1);
    }
    
    depth = _box->halfSize->z - real_abs(relPoint->z);
    
    if (depth < 0) {
        return 0;
    } else if (depth < minDepth) {
        minDepth = depth;
        normal = *_box->getAxis(2) * ((relPoint->z < 0)?-1:1);
    }
    
    Contact * contact = new Contact();
    contact->contactNormal = normal;
    contact->contactPoint = _point;
    contact->penetration = minDepth;
    contact->body[0] = _box->body;
    contact->body[1] = NULL;
    contact->restitution = _data->restitution;
    contact->friction = _data->friction;
    
    _data->contacts->push_back(contact);
    _data->addContacts(1);
    
    return 1;
}