//
//  World.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "SObject.h"

class World {
private:
    std::vector<SObject *> * sObjects;
protected:
    
public:
    World();
    ~World();
    
    void addObject(SObject * _sObject);
    void deleteObject(SObject * _sObject);
    
    std::vector<SObject *> * getSObjects();
};


#endif
