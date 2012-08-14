//
//  SimulatedObject.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 17/06/12.
//  
//

#ifndef SIMULATEDOBJECT_H
#define SIMULATEDOBJECT_H

#include <vector>
#include "CommonStruct.h"
#include "Transform.h"

class SimulatedObject {
private:
    real * vectors;
    std::vector<Vector3 *> * vectorsAux;
    PhysicalFeature * physicalFeature;
    unsigned char * color;
    unsigned char * colorVectors;
    Color * colorAux;
    unsigned int mode; // GLenum
    std::vector<SimulatedObject *> * joinsSimulatedObject;
    BBox * bbox;
    real * matrixTransformation;
    
    bool showBBox;
    bool selected;
    bool immovable;
    
    void makeVectors();
    void makeColorObject();
    void makeColorVectors();
    void makeBBox();
protected:
    
public:
    SimulatedObject();
    ~SimulatedObject();
    
    void initialize();
    void initBBox(real * _matrix);
    
    void addVector3(Vector3 * _vector);
    void deleteVector3(Vector3 * _vector);
    void addAllVectors(std::vector<Vector3 *> * _vectors);
    real * getVectors();

    std::vector<Vector3 *> * getVectorsAux();

    unsigned char * getColor();
    unsigned char * getColorVectors();
    
    void setColorAux(Color * _color);
    Color * getColorAux();
    
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
    
    real * getMatrixTransformation();
    void setMatrixTransformation(real * _matrix);
    
    bool isImmovable();
    void setImmovable(bool _immovable);
};

#endif
