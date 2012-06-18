//
//  SObject.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SOBJECT_H
#define SOBJECT_H

#include <vector>
#include "CommonStruct.h"

class SObject {
private:
    Pointers * pointers;
    PhysicalFeature * physicalFeature;
    Color * color;
protected:
    
public:
    SObject();
    ~SObject();
    
    void addPointer(Pointer _pointer);
    void deletePointer(Pointer * _pointer);
    Pointers * getPointers();

    Color * getColor();
    void setColor(Color * _color);
    
    PhysicalFeature * getPhysicalFeature();
    void setPhysicalFeature(PhysicalFeature * _physicalFeature);
};

#endif
