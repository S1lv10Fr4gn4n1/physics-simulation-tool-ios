//
//  Contact.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 25/08/12.
//
//

#include "Contact.h"

Contact::Contact()
{
    this->contactToWorld = new Matrix3();
}

Contact::~Contact()
{
    delete this->contactToWorld;
    this->contactToWorld = NULL;
}


// orthonormal basis for the contact where each vector is a column.
// The matrix transforms contact space into world space.
void Contact::calculateContactBasis()
{
    Vector3 * contactTangent[2];
    MakeArrayVector3(contactTangent, 2); // TODO delete
    
    // check whether the Z axis is nearer to the X or Y axis
    if (real_abs(this->contactNormal->x) > real_abs(this->contactNormal->y)) {
        // scaling factor to ensure the results are normalized.
        const real scale = (real)1.0f / real_sqrt(this->contactNormal->z*this->contactNormal->z +
                                                  this->contactNormal->x*this->contactNormal->x);
        
        // the new X axis is at right angles to the world Y axis.
        contactTangent[0]->x = this->contactNormal->z*scale;
        contactTangent[0]->y = 0;
        contactTangent[0]->z = -this->contactNormal->x*scale;
        
        // the new Y axis is at right angles to the new X and Z axes.
        contactTangent[1]->x = this->contactNormal->y*contactTangent[0]->x;
        contactTangent[1]->y = this->contactNormal->z*contactTangent[0]->x - this->contactNormal->x*contactTangent[0]->z;
        contactTangent[1]->z = -this->contactNormal->y*contactTangent[0]->x;
    } else {
        // scaling factor to ensure the results are normalized.
        const real s = (real)1.0/ real_sqrt(this->contactNormal->z*this->contactNormal->z +
                                            this->contactNormal->y*this->contactNormal->y);
        
        // the new X axis is at right angles to the world X axis.
        contactTangent[0]->x = 0;
        contactTangent[0]->y = -this->contactNormal->z*s;
        contactTangent[0]->z = this->contactNormal->y*s;
        
        // the new Y axis is at right angles to the new X and Z axes. 
        contactTangent[1]->x = this->contactNormal->y*contactTangent[0]->z - this->contactNormal->z * contactTangent[0]->y;
        contactTangent[1]->y = -this->contactNormal->x*contactTangent[0]->z;
        contactTangent[1]->z = this->contactNormal->x*contactTangent[0]->y;
    }
    
    // make a matrix from the three vectors.
    this->contactToWorld->setComponents(this->contactNormal,
                                        contactTangent[0],
                                        contactTangent[1]);
    
    DeleteArrayVector3(contactTangent, 2);
}

Vector3 * Contact::calculateFrictionlessImpulse(Matrix3 * _inverseInertiaTensor[])
{
    Vector3 * impulseContact = new Vector3();
    
    // build a vector that shows the change in velocity in
    // world space for a unit impulse in the direction of the contact normal
    Vector3 * deltaVelWorld = *this->relativeContactPosition[0] % this->contactNormal;
    deltaVelWorld = _inverseInertiaTensor[0]->transform(deltaVelWorld);
    deltaVelWorld = *deltaVelWorld % this->relativeContactPosition[0];
    
    // work out the change in velocity in contact coordiantes.
    real deltaVelocity = *deltaVelWorld * this->contactNormal;
    
    // add the linear component of velocity change
    deltaVelocity += this->body[0]->getInverseMass();
    
    // check if we need to the second body's data
    if (this->body[1]) {
        // go through the same transformation sequence again
        Vector3 * deltaVelWorld = *this->relativeContactPosition[1] % this->contactNormal;
        deltaVelWorld = _inverseInertiaTensor[1]->transform(deltaVelWorld);
        deltaVelWorld = *deltaVelWorld % this->relativeContactPosition[1];
        
        // add the change in velocity due to rotation
        deltaVelocity += *deltaVelWorld * this->contactNormal;
        
        // add the change in velocity due to linear motion
        deltaVelocity += this->body[1]->getInverseMass();
    }
    
    // calculate the required size of the impulse
    impulseContact->x = this->desiredDeltaVelocity / deltaVelocity;
    impulseContact->y = 0;
    impulseContact->z = 0;
    
    return impulseContact;
}

void Contact::calculateInternals(real _duration)
{
    // check if the first object is NULL, and swap if it is.
    if (!this->body[0]) {
        this->swapBodies();
    }
    
    // calculate a set of axes at the contact point.
    this->calculateContactBasis();
    
    // store the relative position of the contact relative to each body.
    this->relativeContactPosition[0] = *this->contactPoint - this->body[0]->getPosition();
    
    if (this->body[1]) {
        this->relativeContactPosition[1] = *this->contactPoint - this->body[1]->getPosition();
    }
    // find the relative velocity of the bodies at the contact point.
    this->contactVelocity = this->calculateLocalVelocity(0, _duration);
    
    if (this->body[1]) {
        *this->contactVelocity -= this->calculateLocalVelocity(1, _duration);
    }
    
    // calculate the desired change in velocity for resolution.
    this->calculateDesiredDeltaVelocity(_duration);
}


void Contact::calculateDesiredDeltaVelocity(real _duration)
{
    const static real velocityLimit = (real)0.25f;
    
    // calculate the acceleration-induced velocity accumulated this frame.
    real velocityFromAcc = *(*this->body[0]->getLastFrameAcceleration() * _duration) * this->contactNormal;
    if (this->body[1]) {
        velocityFromAcc -= *(*this->body[1]->getLastFrameAcceleration() * _duration) * this->contactNormal;
    }
    
    // if the velocity is very slow, limit the restitution.
    real thisRestitution = this->restitution;
    
    if (real_abs(contactVelocity->x) < velocityLimit) {
        thisRestitution = (real)0.0f;
    }
    // combine the bounce velocity with the removed acceleration velocity.
    this->desiredDeltaVelocity = -this->contactVelocity->x -thisRestitution * (this->contactVelocity->x - velocityFromAcc);
}

void Contact::swapBodies()
{
    *this->contactNormal *= -1;
    
    RigidBody * temp = this->body[0];
    this->body[0] = this->body[1];
    this->body[1] = temp;
}

Vector3 * Contact::calculateLocalVelocity(unsigned _bodyIndex, real _duration)
{
    RigidBody * thisBody = this->body[_bodyIndex];
    
    // work out the velocity of the contact point.
    Vector3 * velocity = *thisBody->getRotation() % this->relativeContactPosition[_bodyIndex];
    *velocity += thisBody->getVelocity();
    
    // turn the velocity into contact coordinates
    return this->contactToWorld->transformTranspose(velocity);
}

void Contact::applyPositionChange(Vector3 * _linearChange[2], Vector3 * _angularChange[2], real _penetration)
{
    const real angularLimit = (real)0.2f;
    real angularMove[2];
    real linearMove[2];
    
    real totalInertia = 0;
    real linearInertia[2];
    real angularInertia[2];
    
    // need to work out the inertia of each object in the direction
    // of the contact normal, due to angular inertia only.
    for (unsigned i = 0; i < 2; i++)
        if (this->body[i]) {
            Matrix3 * inverseInertiaTensor = this->body[i]->getInverseInertiaTensorWorld();
            
            // use the same procedure as for calculating frictionless
            // velocity change to work out the angular inertia.
            Vector3 * angularInertiaWorld = *this->relativeContactPosition[i] % this->contactNormal;
            angularInertiaWorld = inverseInertiaTensor->transform(angularInertiaWorld);
            angularInertiaWorld = *angularInertiaWorld % this->relativeContactPosition[i];
            angularInertia[i] = *angularInertiaWorld * this->contactNormal;
            
            // the linear component is simply the inverse mass
            linearInertia[i] = this->body[i]->getInverseMass();
            
            // keep track of the total inertia from all components
            totalInertia += linearInertia[i] + angularInertia[i];
            
            // we break the loop here so that the totalInertia value is
            // completely calculated (by both iterations) before
            // continuing.
    }
    
    // Loop through again calculating and applying the changes
    for (unsigned i = 0; i < 2; i++)
        if (this->body[i]) {
            // the linear and angular movements required are in proportion to the two inverse inertias.
            real sign = (i == 0) ? 1 : -1;
            angularMove[i] = sign * this->penetration * (angularInertia[i] / totalInertia);
            linearMove[i] = sign * this->penetration * (linearInertia[i] / totalInertia);
            
            // to avoid angular projections that are too great (when mass is large
            // but inertia tensor is small) limit the angular move.
            Vector3 * projection = this->relativeContactPosition[i];
            projection->addScaledVector(this->contactNormal, -this->relativeContactPosition[i]->scalarProduct(this->contactNormal));
            
            // use the small angle approximation for the sine of the angle (i.e.
            // the magnitude would be sine(angularLimit) * projection.magnitude
            // but we approximate sine(angularLimit) to angularLimit).
            real maxMagnitude = angularLimit * projection->magnitude();
            
            if (angularMove[i] < -maxMagnitude) {
                real totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = -maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
                
            } else if (angularMove[i] > maxMagnitude) {
                real totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }
            
            // have the linear amount of movement required by turning
            // the rigid body (in angularMove[i]). We now need to
            // calculate the desired rotation to achieve that.
            if (angularMove[i] == 0) {
                // easy case - no angular movement means no rotation.
                _angularChange[i]->clear();
                
            } else{
                // work out the direction we'd like to rotate in.
                Vector3 * targetAngularDirection = this->relativeContactPosition[i]->vectorProduct(this->contactNormal);
                
                Matrix3 * inverseInertiaTensor = this->body[i]->getInverseInertiaTensorWorld();
                
                // work out the direction did need to rotate to achieve that
                _angularChange[i] = *inverseInertiaTensor->transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
            }
            
            // velocity change is easier - it is just the linear movement along the contact normal
            _linearChange[i] = *this->contactNormal * linearMove[i];
            
            // now we can start to apply the values we've calculated.
            // apply the linear movement
            Vector3 * position = this->body[i]->getPosition();

            position->addScaledVector(this->contactNormal, linearMove[i]);
            this->body[i]->setPosition(position); // TODO how to use pointer, eh not so accurate
            
            // and the change in orientation
            Quaternion * quaternion = this->body[i]->getOrientation();

            quaternion->addScaledVector(_angularChange[i], ((real)1.0));
            this->body[i]->setOrientation(quaternion); // TODO how to use pointer, eh not so accurate
            
            // need to calculate the derived data for any body that is
            // asleep, so that the changes are reflected in the object's
            // data. Otherwise the resolution will not change the position
            // of the object, and the next collision detection round will
            // have the same penetration.
            
            // TODO revise
//            if (!this->body[i]->getAwake()) {
//                this->body[i]->calculateDerivedData();
//            }
    }
}
void Contact::applyVelocityChange(Vector3 * _velocityChange[2], Vector3 * _rotationChange[2])
{
    // get hold of the inverse mass and inverse inertia tensor, both in world coordinates.
    Matrix3 * inverseInertiaTensor[2];
    MakeArrayMatrix3(inverseInertiaTensor, 2); // TODO delete
    
    inverseInertiaTensor[0] = this->body[0]->getInverseInertiaTensorWorld();

    if (this->body[1]) {
        inverseInertiaTensor[1] = this->body[1]->getInverseInertiaTensorWorld();
    }
    
    // calculate the impulse for each contact axis
    Vector3 * impulseContact;
    
    if (this->friction == 0.0f) {
        // use the short format for frictionless contacts
        impulseContact = this->calculateFrictionlessImpulse(inverseInertiaTensor);
    } else {
        
        // otherwise we may have impulses that aren't in the direction of the
        // contact, so we need the more complex version.
        // TODO with friction
        // impulseContact = this->calculateFrictionImpulse(inverseInertiaTensor);
    }
    
    // convert impulse to world coordinates
    Vector3 * impulse = this->contactToWorld->transform(impulseContact);
    
    // split in the impulse into linear and rotational components
    Vector3 * impulsiveTorque = *this->relativeContactPosition[0] % impulse;
    _rotationChange[0] = inverseInertiaTensor[0]->transform(impulsiveTorque);
    _velocityChange[0]->clear();
    _velocityChange[0]->addScaledVector(impulse, this->body[0]->getInverseMass());
    
    // apply the changes
    this->body[0]->addVelocity(_velocityChange[0]);
    this->body[0]->addRotation(_rotationChange[0]);
    
    if (this->body[1]) {
        // work out body one's linear and angular changes
        Vector3 * impulsiveTorque = *impulse % this->relativeContactPosition[1];
        _rotationChange[1] = inverseInertiaTensor[1]->transform(impulsiveTorque);
        _velocityChange[1]->clear();
        _velocityChange[1]->addScaledVector(impulse, -this->body[1]->getInverseMass());
        
        // and apply them
        this->body[1]->addVelocity(_velocityChange[1]);
        this->body[1]->addRotation(_rotationChange[1]);
    }
    
    // TODO revise
//    DeleteArrayMatrix3(inverseInertiaTensor, 2);
}


ContactResolver * ContactResolver::contactResolver;

ContactResolver * ContactResolver::getInstance()
{
    if (!ContactResolver::contactResolver) {
        ContactResolver::contactResolver = new ContactResolver();
    }
    
    return ContactResolver::contactResolver;
}

ContactResolver::ContactResolver(real _positionEpsilon, real _velocityEpsilon)
{
    this->positionEpsilon = _positionEpsilon;
    this->velocityEpsilon = _velocityEpsilon;
}

void ContactResolver::solverContacts(std::vector<Contact *> * _contacts, real _duration)
{
    if (_contacts->size() == 0) {
        return;
    }
    
    this->prepareContacts(_contacts, _duration);
    
    // solver interpenetration
    this->solverPositions(_contacts, _duration);
    
    // solver velocity
    this->solverVelocities(_contacts, _duration);
}

void ContactResolver::prepareContacts(std::vector<Contact *> * _contacts, real _duration)
{
    // generate contact velocity and axis information.
    for (int i=0; i<_contacts->size(); i++) {
        _contacts->at(i)->calculateInternals(_duration);
    }
}

void ContactResolver::solverPositions(std::vector<Contact *> * _contacts, real _duration)
{
    unsigned i;
    unsigned index;
    Vector3 * velocityChange[2];
    Vector3 * rotationChange[2];
    real rotationAmount[2];
    real max;
    Vector3 * contactPoint;

    MakeArrayVector3(velocityChange, 2); // TODO delete
    MakeArrayVector3(rotationChange, 2); // TODO delete
    
    this->positionIterationsUsed = 0;
    this->positionIterations = _contacts->size() * 4; // TODO revise
    
    // iteratively resolve interpenetration in order of severity.
    while (this->positionIterationsUsed < this->positionIterations) {
        // find biggest penetration.
        max = positionEpsilon;
        index = _contacts->size();
        
        for (i=0; i<_contacts->size(); i++) {
            if (_contacts->at(i)->penetration > max) {
                max = _contacts->at(i)->penetration;
                index = i;
            }
        }
        
        if (index == _contacts->size()) {
            break;
        }
        
        // match the awake state at the contact.
        //_contacts[index]->matchAwakeState();
        
        // resolve the penetration.
        _contacts->at(index)->applyPositionChange(velocityChange, rotationChange, max);//rotationAmount, max, -positionEpsilon);
        
        // again this action may have changed the penetration of other bodies, so we update contacts
        for (i=0; i<_contacts->size(); i++) {
            if (_contacts->at(i)->body[0]) {
                if (_contacts->at(i)->body[0] == _contacts->at(index)->body[0]) {
                    contactPoint = rotationChange[0]->vectorProduct(_contacts->at(i)->relativeContactPosition[0]);
                    *contactPoint += velocityChange[0];
                    _contacts->at(i)->penetration -= rotationAmount[0] * contactPoint->scalarProduct(_contacts->at(i)->contactNormal);
                
                } else if (_contacts->at(i)->body[0] == _contacts->at(index)->body[1]) {
                    contactPoint = rotationChange[1]->vectorProduct(_contacts->at(i)->relativeContactPosition[0]);
                    *contactPoint += velocityChange[1];
                    _contacts->at(i)->penetration -= rotationAmount[1] * contactPoint->scalarProduct(_contacts->at(i)->contactNormal);
                }
            }
            
            if (_contacts->at(i)->body[1]) {
                if (_contacts->at(i)->body[1] == _contacts->at(index)->body[0]) {
                    contactPoint = rotationChange[0]->vectorProduct(_contacts->at(i)->relativeContactPosition[1]);
                    *contactPoint += velocityChange[0];
                    _contacts->at(i)->penetration += rotationAmount[0] * contactPoint->scalarProduct(_contacts->at(i)->contactNormal);
                
                } else if (_contacts->at(i)->body[1] == _contacts->at(index)->body[1]) {
                    contactPoint = rotationChange[1]->vectorProduct(_contacts->at(i)->relativeContactPosition[1]);
                    *contactPoint += velocityChange[1];
                    _contacts->at(i)->penetration += rotationAmount[1] * contactPoint->scalarProduct(_contacts->at(i)->contactNormal);
                }
            }
        }
        
        this->positionIterationsUsed++;
    }
    
//    DeleteArrayVector3(velocityChange, 2);
//    DeleteArrayVector3(rotationChange, 2);
}

void ContactResolver::solverVelocities(std::vector<Contact *> * _contacts, real _duration)
{
    Vector3 * velocityChange[2];
    Vector3 * rotationChange[2];
    Vector3 * deltaVel = new Vector3();
    
    MakeArrayVector3(velocityChange, 2); // TODO delete
    MakeArrayVector3(rotationChange, 2); // TODO delete
    
    this->velocityIterationsUsed = 0;
    this->velocityIterations = _contacts->size() * 4; // TODO revise
    
    // iteratively handle impacts in order of severity.
    while (this->velocityIterationsUsed < this->velocityIterations) {
        // find contact with maximum magnitude of probable velocity change.
        real max = this->velocityEpsilon;
        unsigned index = _contacts->size();
        
        for (unsigned i=0; i < _contacts->size(); i++) {
            if (_contacts->at(i)->desiredDeltaVelocity > max) {
                max = _contacts->at(i)->desiredDeltaVelocity;
                index = i;
            }
        }
        
        if (index == _contacts->size()) {
            break;
        }
        
        // match the awake state at the contact
        //_contacts[index]->matchAwakeState();
        
        // do the resolution on the contact that came out top.
        _contacts->at(index)->applyVelocityChange(velocityChange, rotationChange);
        
        // With the change in velocity of the two bodies, the update of
        // contact velocities means that some of the relative closing
        // velocities need recomputing.
        for (unsigned i=0; i<_contacts->size(); i++) {
            // check each body in the contact
            for (unsigned b = 0; b < 2; b++) {
                if (_contacts->at(i)->body[b]) {
                    // check for a match with each body in the newly resolved contact
                    for (unsigned d=0; d < 2; d++) {
                        if (_contacts->at(i)->body[b] == _contacts->at(index)->body[d]) {
                            deltaVel = *velocityChange[d] + rotationChange[d]->vectorProduct(_contacts->at(i)->relativeContactPosition[b]);
                            
                            // the sign of the change is negative if we're dealing
                            // with the second body in a contact.
                            *_contacts->at(i)->contactVelocity += *_contacts->at(i)->contactToWorld->transformTranspose(deltaVel) * (b ? -1 : 1);
                            _contacts->at(i)->calculateDesiredDeltaVelocity(_duration);
                        }
                    }
                }
            }
        	this->velocityIterationsUsed++;
        }
    }
    
//    delete deltaVel;
//    deltaVel = NULL;
//    DeleteArrayVector3(velocityChange, 2);
//    DeleteArrayVector3(rotationChange, 2);
}
