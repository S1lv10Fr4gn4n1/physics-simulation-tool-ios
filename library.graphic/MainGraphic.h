//
//  MainGraphic.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 12/06/12.
//  
//

#ifndef MAINGRAPHIC_H
#define MAINGRAPHIC_H

#include <vector>

#include "Shader.h"
#include "Commons.h"

class MainGraphic {
    Shader * shader;
    SimulatedObject * simulatedObjectDrawn; 
public:
    MainGraphic();
    ~MainGraphic();
    
    void initializeShader(const char* _vertShaderSource, const char* _fragShaderSource, const char* _geomShaderSource);
    Shader * getShader();
    void loadImage();
    void draw(World * _world);
};

#endif
