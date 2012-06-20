//
//  MainGraphic.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 12/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MAINGRAPHIC_H
#define MAINGRAPHIC_H

#include <vector>

#include "Shader.h"
#include "NDC.h"
#include "SimulatedObject.h"

class MainGraphic {
    Shader * shader;
    NDC * ndc;
    SimulatedObject * simulatedObjectDrawn; 
public:
    MainGraphic();
    ~MainGraphic();
    
    void initializeShader(const char* _vertShaderSource, const char* _fragShaderSource, const char* _geomShaderSource);
    void initializeNDC(float _width, float _height);
    
    void updateInformation();
    void rotatedScreen(float _width, float _heigth);
    
    Shader * getShader();
    NDC * getNdc();
    
    void loadImage();
    
    void draw(std::vector<SimulatedObject *> * _simulatedObjects);
};

#endif
