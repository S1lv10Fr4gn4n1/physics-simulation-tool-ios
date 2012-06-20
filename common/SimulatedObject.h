//
//  SimulatedObject.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SIMULATEDOBJECT_H
#define SIMULATEDOBJECT_H

#include <vector>
#include "CommonStruct.h"

class SimulatedObject {
private:
    Pointers * pointers;
    std::vector<Pointer *> * pointersAux;
    PhysicalFeature * physicalFeature;
    Color * color;
    unsigned int mode; // GLenum
protected:
    
public:
    SimulatedObject();
    ~SimulatedObject();
    
    void addPointer(Pointer * _pointer);
    void deletePointer(Pointer * _pointer);
    Pointers * getPointers();

    Color * getColor();
    void setColor(Color * _color);
    
    PhysicalFeature * getPhysicalFeature();
    void setPhysicalFeature(PhysicalFeature * _physicalFeature);
    
    void setMode(unsigned int _mode);
    unsigned int getMode();
};

#endif