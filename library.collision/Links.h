//
//  Links.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 20/08/12.
//
//

#ifndef LINKS_H
#define LINKS_H

#include "Particle.h"
#include "Contact.h"


class ContactGenerator {
public:
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const = 0;
};

class Link : public ContactGenerator {
protected:
    real currentLength() const;
public:
    Particle * particle[2];
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const = 0;
};

class Cable : public Link {
private:
    real maxLength;
    real restitution;
public:
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const;
};

class Rod : public Link {
private:
public:
    real length;
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const;
};

class Constraint : public ContactGenerator {
private:
protected:
    Particle * particle;
    Vector3 * anchor;
    real currentLength() const;
public:
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const = 0;
};

class CableConstraint : public Constraint {
private:
    real maxLength;
    real restitution;
public:
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const;
};

class RodConstraint : public Constraint {
private:
    real length;
public:
    virtual unsigned addContact(Contact * _contact, unsigned _limit) const;
};

#endif
