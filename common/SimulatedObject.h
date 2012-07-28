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
    std::vector<SimulatedObject *> * joinsSimulatedObject;
    BBox * bbox;
    float * matrixTransformation;
    
    bool showBBox;
    bool selected;
    
    void loadBbox();
protected:
    
public:
    SimulatedObject();
    ~SimulatedObject();
    
    void initialize();
    
    void addPointer(Pointer * _pointer);
    void deletePointer(Pointer * _pointer);
    void addAllPointers(std::vector<Pointer *> * _pointers);
    Pointers * getPointers();
    
    std::vector<Pointer *> * getPointersAux();

    Color * getColor();
    void setColor(Color * _color);
    
    PhysicalFeature * getPhysicalFeature();
    void setPhysicalFeature(PhysicalFeature * _physicalFeature);
    
    void setMode(unsigned int _mode);
    unsigned int getMode();
    
    BBox * getBBox();
    void setBBox(BBox * _bbox);
    
    bool isShowBBox();
    void setShowBBox(bool _show);
    
    bool isSelected();
    void setSelected(bool _selected);
    
    float * getMatrixTransformation();
    void setMatrixTransformation(float * _matrix);
};

#endif
