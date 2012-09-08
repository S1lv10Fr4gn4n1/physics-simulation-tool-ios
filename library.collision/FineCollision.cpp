//
//  FineCollision.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 26/08/12.
//
//

#include "FineCollision.h"

#define CHECK_OVERLAP(axis, index) if (!tryAxis(_one, _two, (axis), toCentre, (index), &pen, &best)) return 0;

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




CollisionPrimitive::CollisionPrimitive() {
    this->transform = new Matrix4();
    this->offset = new Matrix4();
}

CollisionPrimitive::~CollisionPrimitive() {
    delete this->transform;
    delete this->offset;
    
    this->transform = NULL;
    this->offset = NULL;
}

Vector3 * CollisionPrimitive::getAxis(unsigned _index) const
{
    return this->transform->getAxisVector(_index);
}

void CollisionPrimitive::calculateInternals()
{
    this->transform = *this->body->getTransformMatrix() * offset;
}

CollisionSphere::CollisionSphere(RigidBody * _body, real _radius) {
    this->body = _body;
    this->radius = _radius;
    
    this->calculateInternals();
}

CollisionPlane::CollisionPlane(RigidBody * _body, Vector3 * _direction, real _offset) {
    this->body = _body;
    this->direction = _direction;
    this->offset = _offset;
    
    this->calculateInternals();
}

CollisionBox::CollisionBox(RigidBody * _body, Vector3 * _halfSize) {
    this->body = _body;
    this->halfSize = _halfSize;
    
    this->calculateInternals();
}


real transformToAxis(const CollisionBox * _box, const Vector3 * _axis)
{
    return _box->halfSize->x * real_abs(*_axis * _box->getAxis(0)) +
           _box->halfSize->y * real_abs(*_axis * _box->getAxis(1)) +
           _box->halfSize->z * real_abs(*_axis * _box->getAxis(2));
}

bool overlapOnAxis(const CollisionBox * _one, const CollisionBox * _two, const Vector3 * _axis)
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

real penetrationOnAxis(const CollisionBox * _one,
                       const CollisionBox * _two,
                       const Vector3 * _axis,
                       const Vector3 * _toCentre)
{
    // project the half-size of one onto axis
    real oneProject = transformToAxis(_one, _axis);
    real twoProject = transformToAxis(_two, _axis);
    
    // project this onto the axis
    real distance = real_abs(*_toCentre * _axis);
    
    // return the overlap (i.e. positive indicates
    // overlap, negative indicates separation).
    return oneProject + twoProject - distance;
}


bool tryAxis(const CollisionBox * _one, const CollisionBox * _two, Vector3 * _axis, const Vector3 * _toCentre,
             unsigned _index, real * _smallestPenetration, unsigned * _smallestCase)
{
    // make sure we have a normalized axis, and don't check almost parallel axes
    if (_axis->squareMagnitude() < 0.0001) {
    	return true;
    }
    _axis->normalize();
    
    real penetration = penetrationOnAxis(_one, _two, _axis, _toCentre);
    
    if (penetration < 0) {
    	return false;
    }
    
    if (penetration < *_smallestPenetration) {
        *_smallestPenetration = penetration;
        *_smallestCase = _index;
    }
    
    return true;
}

void fillPointFaceBoxBox(const CollisionBox * _one, const CollisionBox * _two, const Vector3 * _toCentre,
                         CollisionData * _data, unsigned _best, real _pen)
{
    // this method is called when we know that a vertex from
    // box two is in contact with box one.
    
    // know which axis the collision is on (i.e. best),
    // but we need to work out which of the two faces on this axis.
    Vector3 * normal = _one->getAxis(_best);
    
    if (*_one->getAxis(_best) * _toCentre > 0) {
        normal = *normal * -1.0f;
    }
    
    // work out which vertex of box two we're colliding with.
    // using toCentre doesn't work!
    Vector3 * vertex = _two->halfSize;
    if (*_two->getAxis(0) * normal < 0) {
    	vertex->x = -vertex->x;
    }
    
    if (*_two->getAxis(1) * normal < 0) {
    	vertex->y = -vertex->y;
    }
    
    if (*_two->getAxis(2) * normal < 0) {
    	vertex->z = -vertex->z;
    }
    
    // Create the contact data
    Contact * contact = new Contact();
    contact->contactNormal = normal;
    contact->penetration = _pen;
    contact->contactPoint = *_two->transform * vertex;
    contact->body[0] = _one->body;
    contact->body[0] = _two->body;
    contact->friction = _data->friction;
    contact->restitution = _data->restitution;
}

Vector3 * contactPoint(Vector3 * _pOne, Vector3 * _dOne, real oneSize, Vector3 * _pTwo,
                       Vector3 * _dTwo, real _twoSize, bool _useOne)
{
    Vector3 * toSt, * cOne, * cTwo;
    real dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    real denom, mua, mub;
    
    smOne = _dOne->squareMagnitude();
    smTwo = _dTwo->squareMagnitude();
    dpOneTwo = *_dTwo * _dOne;
    
    toSt = *_pOne - _pTwo;
    dpStaOne = *_dOne * toSt;
    dpStaTwo = *_dTwo * toSt;
    
    denom = smOne * smTwo - dpOneTwo * dpOneTwo;
    
    // zero denominator indicates parrallel lines
    if (real_abs(denom) < 0.0001f) {
        return _useOne ? _pOne : _pTwo;
    }
    
    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;
    
    // if either of the edges has the nearest point out
    // of bounds, then the edges aren't crossed, we have
    // an edge-face contact. Our point is on the edge, which
    // we know from the useOne parameter.
    if (mua > oneSize || mua < -oneSize || mub > _twoSize || mub < -_twoSize) {
        return _useOne ? _pOne : _pTwo;
    } else {
        cOne = *_pOne + (*_dOne * mua);
        cTwo = *_pTwo + (*_dTwo * mub);
        
        return *(*cOne * 0.5) + (*cTwo * 0.5);
    }
}

unsigned CollisionDetector::sphereAndSphere(const CollisionSphere * _one, const CollisionSphere * _two, CollisionData * _data)
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

unsigned CollisionDetector::sphereAndHalfSpace(const CollisionSphere * _sphere, const CollisionPlane * _plane, CollisionData * _data)
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

unsigned CollisionDetector::sphereAndTruePlane(const CollisionSphere * _sphere, const CollisionPlane * _plane, CollisionData * _data)
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

unsigned CollisionDetector::boxAndHalfSpace(const CollisionBox * _box, const CollisionPlane * _plane, CollisionData * _data)
{
    if (_data->contactsLeft <= 0) {
        return 0;
    }
    
    // work out the projected radius of the box onto the plane direction
    real projectedRadius = transformToAxis(_box, _plane->direction);
    
    // work out how far the box is from the origin
    real boxDistance = *_plane->direction * _box->getAxis(3) - projectedRadius;
    
    // Check for the intersection
    if (!boxDistance <= _plane->offset) {
        return 0;
    }

    // go through each combination of + and - for each half-size
    static real mults[8][3] = {{1,1,1}, {-1,1,1}, {1,-1,1}, {-1,-1,1},
                               {1,1,-1}, {-1,1,-1}, {1,-1,-1}, {-1,-1,-1}};
    
    Contact * contact = new Contact();
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++) {
        
        // calculate the position of each vertex
        Vector3 * vertexPos = new Vector3(mults[i][0], mults[i][1], mults[i][2]);
        vertexPos->componentProductUpdate(_box->halfSize);
        vertexPos = _box->transform->transform(vertexPos);
        
        // calculate the distance from the plane
        real vertexDistance = *vertexPos * _plane->direction;
        
        // compare this to the plane's distance
        if (vertexDistance <= _plane->offset) {
            // create the contact data.
            
            // the contact point is halfway between the vertex and the
            // plane - we multiply the direction by half the separation
            // distance and add the vertex location.
            contact->contactPoint = _plane->direction;
            *contact->contactPoint *= (vertexDistance - _plane->offset);
            contact->contactPoint = vertexPos;
            contact->contactNormal = _plane->direction;
            contact->penetration = _plane->offset - vertexDistance;
            
            // write the appropriate data
            contact->body[0] = _box->body;
            contact->body[1] = NULL;
            contact->friction = _data->friction;
            contact->restitution = _data->restitution;
            
            // move onto the next contact
            contactsUsed++;
            
            if (contactsUsed == _data->contactsLeft) {
                return contactsUsed;
            }
        }
    }
    
    _data->addContacts(contactsUsed);
    
    return contactsUsed;
}


unsigned CollisionDetector::boxAndBox(const CollisionBox * _one, const CollisionBox * _two, CollisionData * _data)
{
    // find the vector between the two centres
    Vector3 * toCentre = *_two->getAxis(3) - _one->getAxis(3);
    
    // start assuming there is no contact
    real pen = REAL_MAX;
    unsigned best = 0xffffff;
    
    // check each axes, returning if it gives us
    // a separating axis, and keeping track of the axis with
    // the smallest penetration otherwise.
    CHECK_OVERLAP(_one->getAxis(0), 0);
    CHECK_OVERLAP(_one->getAxis(1), 1);
    CHECK_OVERLAP(_one->getAxis(2), 2);
    
    CHECK_OVERLAP(_two->getAxis(0), 3);
    CHECK_OVERLAP(_two->getAxis(1), 4);
    CHECK_OVERLAP(_two->getAxis(2), 5);
    
    // Store the best axis-major, in case we run into almost
    // parallel edge collisions later
    unsigned bestSingleAxis = best;
    
    CHECK_OVERLAP(*_one->getAxis(0) % _two->getAxis(0), 6);
    CHECK_OVERLAP(*_one->getAxis(0) % _two->getAxis(1), 7);
    CHECK_OVERLAP(*_one->getAxis(0) % _two->getAxis(2), 8);
    CHECK_OVERLAP(*_one->getAxis(1) % _two->getAxis(0), 9);
    CHECK_OVERLAP(*_one->getAxis(1) % _two->getAxis(1), 10);
    CHECK_OVERLAP(*_one->getAxis(1) % _two->getAxis(2), 11);
    CHECK_OVERLAP(*_one->getAxis(2) % _two->getAxis(0), 12);
    CHECK_OVERLAP(*_one->getAxis(2) % _two->getAxis(1), 13);
    CHECK_OVERLAP(*_one->getAxis(2) % _two->getAxis(2), 14);
    
    // now know there's a collision, and we know which
    // of the axes gave the smallest penetration. We now
    // can deal with it in different ways depending on the case.
    if (best < 3) {
        // we've got a vertex of box two on a face of box one.
        fillPointFaceBoxBox(_one, _two, toCentre, _data, best, pen);
        _data->addContacts(1);
        return 1;
    
    } else if (best < 6) {
        // We've got a vertex of box one on a face of box two.
        // We use the same algorithm as above, but swap around
        // one and two (and therefore also the vector between their
        // centres).
        fillPointFaceBoxBox(_two, _one, *toCentre * -1.0f, _data, best-3, pen);
        _data->addContacts(1);
        return 1;
        
    } else {
        // we've got an edge-edge contact. Find out which axes
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        Vector3 * oneAxis = _one->getAxis(oneAxisIndex);
        Vector3 * twoAxis = _two->getAxis(twoAxisIndex);
        Vector3 * axis = *oneAxis % twoAxis;
        axis->normalize();
        
        // the axis should point from box one to box two.
        if (*axis * toCentre > 0) {
            axis = *axis * -1.0f;
        }
        
        // we have the axes, but not the edges: each axis has 4 edges parallel
        // to it, we need to find which of the 4 for each object. We do
        // that by finding the point in the centre of the edge. We know
        // its component in the direction of the box's collision axis is zero
        // (its a mid-point) and we determine which of the extremes in each
        // of the other axes is closest.
        Vector3 * ptOnOneEdge = _one->halfSize;
        Vector3 * ptOnTwoEdge = _two->halfSize;
        
        for (unsigned i = 0; i < 3; i++) {
            if (i == oneAxisIndex) {
                ptOnOneEdge->set(i, 0);
            } else if (*_one->getAxis(i) * axis > 0) {
                ptOnOneEdge->set(i, -ptOnOneEdge->get(i));
            }
            
            if (i == twoAxisIndex) {
                ptOnTwoEdge->set(i, 0);
            } else if (*_two->getAxis(i) * axis < 0) {
                ptOnTwoEdge->set(i, -ptOnTwoEdge->get(i));
            }
        }
        
        // move them into world coordinates (they are already oriented
        // correctly, since they have been derived from the axes).
        ptOnOneEdge = *_one->transform * ptOnOneEdge;
        ptOnTwoEdge = *_two->transform * ptOnTwoEdge;
        
        // so we have a point and a direction for the colliding edges.
        // we need to find out point of closest approach of the two line-segments.
        Vector3 * vertex = contactPoint(ptOnOneEdge, oneAxis, _one->halfSize->get(oneAxisIndex),
                                        ptOnTwoEdge, twoAxis, _two->halfSize->get(twoAxisIndex),
                                        bestSingleAxis > 2);

        Contact * contact = new Contact();
        contact->penetration = pen;
        contact->contactNormal = axis;
        contact->contactPoint = vertex;
        contact->body[0] = _one->body;
        contact->body[1] = _two->body;
        contact->friction = _data->friction;
        contact->restitution = _data->restitution;
        _data->addContacts(1);
        
        return 1;
    }
    
    return 0;
}

unsigned CollisionDetector::boxAndSphere(const CollisionBox * _box, const CollisionSphere * _sphere, CollisionData * _data)
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

unsigned CollisionDetector::boxAndPoint(const CollisionBox * _box, Vector3 * _point,  CollisionData * _data)
{
    // transform the point into box coordinates.
    Vector3 * relPoint = _box->transform->transformInverse(_point);
    
    // check each axis, looking for the axis on which the penetration is least deep.
    real minDepth = _box->halfSize->x - real_abs(relPoint->x);
    
    if (minDepth < 0) {
        return 0;
    }
    
    Vector3 * normal = *_box->getAxis(0) * ((relPoint->x < 0) ? -1 : 1);
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