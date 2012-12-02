//
//  NDC.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 14/06/12.
//  
//

#ifndef NDC_H
#define NDC_H

#include "CommonStructures.h"

//!Class responsible for standardization of values
class NDC {
    Vector3 * maxWindow;
    Vector3 * minWindow;
    Vector3 * maxOrtho;
    Vector3 * minOrtho;
    
    real aspect;
    real width;
    real height;
public:
    NDC();
    ~NDC();
    
    void update(real _width, real _height);
    void calcNDCCoordinates(real * _x, real * _y);
    
    real getAspect();

    real getLeft();
    void setLeft(real _left);
    real getRight();
    void setRight(real _right);
    real getTop();
    void setTop(real _top);
    real getBottom();
    void setBottom(real _bottom);

    Vector3 * getMaxWindow();
};

#endif
