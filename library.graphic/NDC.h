//
//  NDC.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 14/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NDC_H
#define NDC_H

#include "CommonGraphics.h"

class NDC {
    Pointer * maxWindow;
    Pointer * minWindow;
    Pointer * maxOrtho;
    Pointer * minOrtho;
public:
    NDC();
    ~NDC();
    
    void update(float _width, float _height);
    void calcNDCCoordinates(float * _x, float * _y);
};

#endif
