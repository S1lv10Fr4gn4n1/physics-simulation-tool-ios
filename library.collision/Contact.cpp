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
    // TODO put your code here
}

Contact::~Contact()
{
    // TODO put your code here
}

//!Method responsible for waking a rigid hub asleep
void Contact::matchAwakeState()
{
    // collisions with the world never cause a body to wake up.
    if (!this->body[1]) {
        return;
    }
    
    bool body0awake = body[0]->isAwake();
    bool body1awake = body[1]->isAwake();

    // wake up only the sleeping one
    if (body0awake ^ body1awake) {
        if (body0awake) {
            this->body[1]->setAwake();
        } else {
            this->body[0]->setAwake();
        }
    }
}

//!Method responsible for identifying the base contact
void Contact::calculateContactBasis()
{
    // orthonormal basis for the contact where each vector is a column
    // The matrix transforms contact space into world space.

    Vector3 contactTangent[2];
    
    // check whether the Z axis is nearer to the X or Y axis
    if (real_abs(this->contactNormal.x) > real_abs(this->contactNormal.y)) {
        // scaling factor to ensure the results are normalized.
        const real scale = (real)1.0f / real_sqrt(this->contactNormal.z*this->contactNormal.z +
                                                  this->contactNormal.x*this->contactNormal.x);
        
        // the new X axis is at right angles to the world Y axis.
        contactTangent[0].x = this->contactNormal.z*scale;
        contactTangent[0].y = 0;
        contactTangent[0].z = -this->contactNormal.x*scale;
        
        // the new Y axis is at right angles to the new X and Z axes.
        contactTangent[1].x = this->contactNormal.y*contactTangent[0].x;
        contactTangent[1].y = this->contactNormal.z*contactTangent[0].x - this->contactNormal.x*contactTangent[0].z;
        contactTangent[1].z = -this->contactNormal.y*contactTangent[0].x;
    } else {
        // scaling factor to ensure the results are normalized.
        const real s = (real)1.0/ real_sqrt(this->contactNormal.z*this->contactNormal.z +
                                            this->contactNormal.y*this->contactNormal.y);
        
        // the new X axis is at right angles to the world X axis.
        contactTangent[0].x = 0;
        contactTangent[0].y = -this->contactNormal.z*s;
        contactTangent[0].z = this->contactNormal.y*s;
        
        // the new Y axis is at right angles to the new X and Z axes. 
        contactTangent[1].x = this->contactNormal.y*contactTangent[0].z - this->contactNormal.z * contactTangent[0].y;
        contactTangent[1].y = -this->contactNormal.x*contactTangent[0].z;
        contactTangent[1].z = this->contactNormal.x*contactTangent[0].y;
    }
    
    // make a matrix from the three vectors.
    this->contactToWorld.setComponents(this->contactNormal,
                                       contactTangent[0],
                                       contactTangent[1]);
}

//!Method responsible for calculating the impulse frictionless
Vector3 Contact::calculateFrictionlessImpulse(Matrix3 * _inverseInertiaTensor)
{
    Vector3 impulseContact;
    
    // build a vector that shows the change in velocity in
    // world space for a unit impulse in the direction of the contact normal
    Vector3 deltaVelWorld = this->relativeContactPosition[0] % this->contactNormal;
    deltaVelWorld = _inverseInertiaTensor[0].transform(deltaVelWorld);
    deltaVelWorld = deltaVelWorld % this->relativeContactPosition[0];
    
    // work out the change in velocity in contact coordiantes.
    real deltaVelocity = deltaVelWorld * this->contactNormal;
    
    // add the linear component of velocity change
    deltaVelocity += this->body[0]->getInverseMass();
    
    // check if we need to the second body's data
    if (this->body[1]) {
        // go through the same transformation sequence again
        Vector3 deltaVelWorld = this->relativeContactPosition[1] % this->contactNormal;
        deltaVelWorld = _inverseInertiaTensor[1].transform(deltaVelWorld);
        deltaVelWorld = deltaVelWorld % this->relativeContactPosition[1];
        
        // add the change in velocity due to rotation
        deltaVelocity += deltaVelWorld * this->contactNormal;
        
        // add the change in velocity due to linear motion
        deltaVelocity += this->body[1]->getInverseMass();
    }
    
    // calculate the required size of the impulse
    impulseContact.x = this->desiredDeltaVelocity / deltaVelocity;
    impulseContact.y = 0;
    impulseContact.z = 0;
    
    return impulseContact;
}

//!Method responsible for calculating the impulse friction
Vector3 Contact::calculateFrictionImpulse(Matrix3 * _inverseInertiaTensor)
{
    real inverseMass = this->body[0]->getInverseMass();
    Vector3 impulseContact;
    
    // the equivalent of a cross product in matrices is multiplication
    // by a skew-symmetric matrix - we build the matrix for converting
    // between linear and angular quantities.
    Matrix3 impulseToTorque;
    impulseToTorque.setSkewSymmetric(this->relativeContactPosition[0]);
    
    // build the matrix to convert contact impulse to change in velocity in world coordinates.
    Matrix3 deltaVelWorld = impulseToTorque;
    deltaVelWorld *= _inverseInertiaTensor[0];
    deltaVelWorld *= impulseToTorque;
    deltaVelWorld *= -1;
    
    // check whether we need to add body 2’s data.
    if (this->body[1]) {
        // find the inertia tensor for this body.
        _inverseInertiaTensor[1] = this->body[1]->getInverseInertiaTensorWorld();
        // set the cross product matrix.
        impulseToTorque.setSkewSymmetric(this->relativeContactPosition[1]);
        // calculate the velocity change matrix.
        Matrix3 deltaVelWorld2 = impulseToTorque;
        deltaVelWorld2 *= _inverseInertiaTensor[1];
        deltaVelWorld2 *= impulseToTorque;
        deltaVelWorld2 *= -1;
        // add to the total delta velocity.
        deltaVelWorld += deltaVelWorld2;
        // add to the inverse mass.
        inverseMass += this->body[1]->getInverseMass();
    }
    // do a change of basis to convert into contact coordinates.
    Matrix3 deltaVelocity = this->contactToWorld.transpose();
    deltaVelocity *= deltaVelWorld;
    deltaVelocity *= this->contactToWorld;
    
    // add in the linear velocity change.
    deltaVelocity.data[0] += inverseMass;
    deltaVelocity.data[4] += inverseMass;
    deltaVelocity.data[8] += inverseMass;
    
    // invert to get the impulse needed per unit velocity.
    Matrix3 impulseMatrix = deltaVelocity.inverse();
    
    // find the target velocities to kill.
    Vector3 velKill(this->desiredDeltaVelocity, -this->contactVelocity.y, -this->contactVelocity.z);
    // find the impulse to kill target velocities.
    impulseContact = impulseMatrix.transform(velKill);
    // check for exceeding friction.
    real planarImpulse = real_sqrt(impulseContact.y*impulseContact.y +
                                   impulseContact.z*impulseContact.z);
    
    if (planarImpulse > impulseContact.x * this->friction) {
        // we need to use dynamic friction.
        impulseContact.y /= planarImpulse;
        impulseContact.z /= planarImpulse;
        impulseContact.x = deltaVelocity.data[0] + deltaVelocity.data[1] * this->friction * impulseContact.y +
                           deltaVelocity.data[2] * this->friction * impulseContact.z;
        impulseContact.x = this->desiredDeltaVelocity / impulseContact.x;
        impulseContact.y *= this->friction * impulseContact.x;
        impulseContact.z *= this->friction * impulseContact.x;
    }
    
    return impulseContact;
}

//!Method responsible for generating information for the contact, such as velovidade desired relative position and contact
void Contact::calculateInternals(real _duration)
{
    // check if the first object is NULL, and swap if it is.
    if (!this->body[0]) {
        this->swapBodies();
    }
    
    // calculate a set of axes at the contact point.
    this->calculateContactBasis();
    
    // store the relative position of the contact relative to each body.
    this->relativeContactPosition[0] = this->contactPoint - this->body[0]->getPosition();
    
    if (this->body[1]) {
        this->relativeContactPosition[1] = this->contactPoint - this->body[1]->getPosition();
    }
    // find the relative velocity of the bodies at the contact point.
    this->contactVelocity = this->calculateLocalVelocity(0, _duration);
    
    if (this->body[1]) {
        this->contactVelocity -= this->calculateLocalVelocity(1, _duration);
    }
    
    // calculate the desired change in velocity for resolution.
    this->calculateDesiredDeltaVelocity(_duration);
}

//!Method responsible to calculate the desired speed from the difference of the accelerations of the rigid bodies
void Contact::calculateDesiredDeltaVelocity(real _duration)
{
    const static real velocityLimit = (real)0.25f;

    // calculate the acceleration-induced velocity accumulated this frame.
    real velocityFromAcc = 0;
    if (this->body[0]->isAwake()) {
        this->body[0]->getLastFrameAcceleration() * _duration * this->contactNormal;
    }
    if (this->body[1] && this->body[1]->isAwake()) {
        velocityFromAcc -= this->body[1]->getLastFrameAcceleration() * _duration * this->contactNormal;
    }
    
    // if the velocity is very slow, limit the restitution.
    real appliedRestitution = this->restitution;
    
    //if (real_abs(this->contactVelocity->x) < velocityLimit) {
    if (this->contactVelocity.magnitude() < velocityLimit) {
        appliedRestitution = (real)0.0f;
    }
    
    // combine the bounce velocity with the removed acceleration velocity.
    this->desiredDeltaVelocity = -this->contactVelocity.x -appliedRestitution * (this->contactVelocity.x - velocityFromAcc);
}

void Contact::swapBodies()
{
    this->contactNormal *= -1;
    
    RigidBody * temp = this->body[0];
    this->body[0] = this->body[1];
    this->body[1] = temp;
}

//!Method responsible to find the relative velocity of the rigid body from point of contact
Vector3 Contact::calculateLocalVelocity(unsigned _bodyIndex, real _duration)
{
    RigidBody * body = this->body[_bodyIndex];
    
    // work out the velocity of the contact point.
    Vector3 velocity = body->getRotation() % this->relativeContactPosition[_bodyIndex];
    velocity += body->getVelocity();
    
    // turn the velocity into contact coordinates.
    Vector3 contactVelocity = this->contactToWorld.transformTranspose(velocity);
    
    // calculate the amount of velocity that is due to forces without reactions
    Vector3 accVelocity = body->getLastFrameAcceleration() * _duration;
    
    // calculate the velocity in contact coordinates.
    accVelocity = this->contactToWorld.transformTranspose(accVelocity);
    
    // we ignore any component of acceleration in the contact normal direction
    // are only interested in planar acceleration
    accVelocity.x = 0;
    
    // add the planar velocities - if there’s enough friction they will
    // be removed during velocity resolution
    contactVelocity += accVelocity;
    
    return contactVelocity;
}

//!Method responsible for resolving the interpenetration of the rigid body
void Contact::applyPositionChange(Vector3 _linearChange[2], Vector3 _angularChange[2], real _penetration)
{
    const real angularLimit = (real)0.2f;
    real angularMove[2] = {0.0f, 0.0f};
    real linearMove[2] = {0.0f, 0.0f};
    
    real totalInertia = 0;
    real linearInertia[2] = {0.0f, 0.0f};
    real angularInertia[2] = {0.0f, 0.0f};
    
    // need to work out the inertia of each object in the direction
    // of the contact normal, due to angular inertia only
    for (unsigned i = 0; i < 2; i++) {
        if (this->body[i]) {
            Matrix3 inverseInertiaTensor = this->body[i]->getInverseInertiaTensorWorld();
            
            // use the same procedure as for calculating frictionless
            // velocity change to work out the angular inertia.
            Vector3 angularInertiaWorld = this->relativeContactPosition[i] % this->contactNormal;
            angularInertiaWorld = inverseInertiaTensor.transform(angularInertiaWorld);
            angularInertiaWorld = angularInertiaWorld % this->relativeContactPosition[i];
            angularInertia[i] = angularInertiaWorld * this->contactNormal;
            
            // the linear component is simply the inverse mass
            linearInertia[i] = this->body[i]->getInverseMass();
            
            // keep track of the total inertia from all components
            totalInertia += linearInertia[i] + angularInertia[i];
            
            // we break the loop here so that the totalInertia value is
            // completely calculated (by both iterations) before continuing
        }
    }

    // Loop through again calculating and applying the changes
    for (unsigned i = 0; i < 2; i++) {
        if (this->body[i]) {
            // the linear and angular movements required are in proportion to the two inverse inertias.
            real sign = (i == 0) ? 1 : -1;
            angularMove[i] = sign * this->penetration * (angularInertia[i] / totalInertia);
            linearMove[i] = sign * this->penetration * (linearInertia[i] / totalInertia);
            
            // to avoid angular projections that are too great (when mass is large
            // but inertia tensor is small) limit the angular move.
            Vector3 projection = this->relativeContactPosition[i];
            projection.addScaledVector(this->contactNormal, -this->relativeContactPosition[i].scalarProduct(this->contactNormal));
            
            // use the small angle approximation for the sine of the angle (i.e.
            // the magnitude would be sine(angularLimit) * projection.magnitude
            // but we approximate sine(angularLimit) to angularLimit).
            real maxMagnitude = angularLimit * projection.magnitude();
            
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
                _angularChange[i].clear();
            } else{
                // work out the direction we'd like to rotate in.
                Vector3 targetAngularDirection = this->relativeContactPosition[i].vectorProduct(this->contactNormal);
                
                Matrix3 inverseInertiaTensor = this->body[i]->getInverseInertiaTensorWorld();
                
                // work out the direction did need to rotate to achieve that
                _angularChange[i] = inverseInertiaTensor.transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
            }
            
            // velocity change is easier - it is just the linear movement along the contact normal
            _linearChange[i] = this->contactNormal * linearMove[i];
            
            // now we can start to apply the values we've calculated.
            // apply the linear movement
            Vector3 position = this->body[i]->getPosition();

            position.addScaledVector(this->contactNormal, linearMove[i]);
            this->body[i]->setPosition(position); // TODO how to use pointer, eh not so accurate
            
            // and the change in orientation
            Quaternion quaternion = this->body[i]->getOrientation();

            quaternion.addScaledVector(_angularChange[i], ((real)1.0));
            this->body[i]->setOrientation(quaternion); // TODO how to use pointer, eh not so accurate
            
            if (!this->body[i]->isAwake()) {
                this->body[i]->calculateDerivedData();
            }
        }
    }
}

//!Method responsible for adjusting the linear and angular velocities
void Contact::applyVelocityChange(Vector3 _velocityChange[2], Vector3 _rotationChange[2])
{
    // get hold of the inverse mass and inverse inertia tensor, both in world coordinates.
    Matrix3 inverseInertiaTensor[2];
    
    inverseInertiaTensor[0] = this->body[0]->getInverseInertiaTensorWorld();

    if (this->body[1]) {
        inverseInertiaTensor[1] = this->body[1]->getInverseInertiaTensorWorld();
    }
    
    // calculate the impulse for each contact axis
    Vector3 impulseContact;
    
    if (this->friction == 0.0f) {
        // use the short format for frictionless contacts
        impulseContact = this->calculateFrictionlessImpulse(inverseInertiaTensor);
    } else {
        // otherwise we may have impulses that aren't in the direction of the
        // contact, so we need the more complex version.        
        impulseContact = this->calculateFrictionImpulse(inverseInertiaTensor);
    }
    
    // convert impulse to world coordinates
    Vector3 impulse = this->contactToWorld.transform(impulseContact);
    
    // split in the impulse into linear and rotational components
    Vector3 impulsiveTorque = this->relativeContactPosition[0] % impulse;
    _rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
    _velocityChange[0].clear();
    _velocityChange[0].addScaledVector(impulse, this->body[0]->getInverseMass());
    
    // apply the changes
    this->body[0]->addVelocity(_velocityChange[0]);
    this->body[0]->addRotation(_rotationChange[0]);
    
    if (this->body[1]) {
        // work out body one's linear and angular changes
        Vector3 impulsiveTorque = impulse % this->relativeContactPosition[1];
        _rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
        _velocityChange[1].clear();
        _velocityChange[1].addScaledVector(impulse, -this->body[1]->getInverseMass());
        
        // and apply them
        this->body[1]->addVelocity(_velocityChange[1]);
        this->body[1]->addRotation(_rotationChange[1]);
    }
}
