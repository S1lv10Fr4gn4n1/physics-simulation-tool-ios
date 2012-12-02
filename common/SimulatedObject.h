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
#include "CommonStructures.h"
#include "RigidBody.h"

//!Class inherited from RigidBody, responsible for keeping informations on the shape of the object to be drawn
class SimulatedObject : public RigidBody {
private:
    real * vectors;
    std::vector<Vector3> * vectorsAux;
    unsigned char * color;
    unsigned char * colorVectors;
    Color * colorAux;
    unsigned int mode;
//    BBox * bbox;
    Matrix4x4 matrixTransformation;
    
    bool showBBox;
    bool selected;
    
    void makeVectors();
    void makeColorObject();
    void makeColorVectors();
//    void makeBBox();
protected:
    
public:
    SimulatedObject();
    ~SimulatedObject();
    
    void initialize();
//    void initBBox(real * _matrix);

    void addVector3(const Vector3 &_vector);
    void deleteVector3(const Vector3 &_vector);
    void addAllVectors(std::vector<Vector3> * _vectors);
    real * getVectors();

    std::vector<Vector3> * getVectorsAux();

    unsigned char * getColor();
    unsigned char * getColorVectors();
    
    void setColorAux(Color * _color);
    void setColorAux(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);
    Color * getColorAux();
    
    void setMode(unsigned int _mode);
    unsigned int getMode();
    
//    BBox * getBBox();

//    bool isShowBBox();
//    void setShowBBox(bool _show);

    bool isSelected();
    void setSelected(bool _selected);
    
    Matrix4x4 getMatrixTransformation();
    void setMatrixTransformation(Matrix4x4 _matrix);
    void updateMatrixTransformation();
};

#endif
