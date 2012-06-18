//
//  MainGraphic.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 12/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MainGraphic.h"

MainGraphic::MainGraphic()
{
    /// TODO - implementation
}

MainGraphic::~MainGraphic()
{
    delete ndc;
    delete shader;
}

void MainGraphic::initializeShader(const char* _vertShaderSource, const char* _fragShaderSource, const char* _geomShaderSource)
{
    shader = new Shader(_vertShaderSource, _fragShaderSource, _geomShaderSource);
    shader->loadShaders();
}

void MainGraphic::initializeNDC(float _width, float _height)
{
    ndc = new NDC();
    ndc->update(_width, _height);
}

Shader * MainGraphic::getShader()
{
    return shader;
}

NDC * MainGraphic::getNdc()
{
    return ndc;
}

void MainGraphic::updateInformation()
{
    ///TODO
}

void MainGraphic::rotatedScreen(float _width, float _height)
{
    ndc->update(_width, _height);
}

void MainGraphic::MainGraphic::draw(std::vector<SObject *> * _sObjects)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the object again with ES2
    glUseProgram(shader->getProgram());
    
    SObject * sObject = NULL; 
    for (int i=0; i<_sObjects->size(); i++) {
        sObject = _sObjects->at(i);
        
        // define color for square
        glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, 0, sObject->getColor());
        glEnableVertexAttribArray(ATTRIB_COLOR);
        
        // Update attribute values.
        glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, sObject->getPointers()->ps);
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, sObject->getPointers()->count);
    }
}