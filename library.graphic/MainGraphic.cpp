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
    this->shader = 0;
    this->simulatedObjectDrawn = 0;
}

MainGraphic::~MainGraphic()
{
    delete this->shader;
    delete this->simulatedObjectDrawn;
}

void MainGraphic::initializeShader(const char* _vertShaderSource, const char* _fragShaderSource, const char* _geomShaderSource)
{
    this->shader = new Shader(_vertShaderSource, _fragShaderSource, _geomShaderSource);
    this->shader->loadShaders();
}

Shader * MainGraphic::getShader()
{
    return this->shader;
}

void MainGraphic::MainGraphic::draw(World * _world)
{    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the object again with ES2
    glUseProgram(this->shader->getProgram());
    
    GLuint matrixView = this->shader->getVar(UNIFORM_MODELVIEWPROJECTION_MATRIX);
    
    for (int i=0; i< _world->getSimulatedObjects()->size(); i++) {
        this->simulatedObjectDrawn = _world->getSimulatedObjects()->at(i);
        
        // define color for simulated
        glVertexAttribPointer(ATTRIB_COLOR, CHANNEL_COLOR, GL_UNSIGNED_BYTE, 1, 0, this->simulatedObjectDrawn->getColor()->color);
        glEnableVertexAttribArray(ATTRIB_COLOR);
        
        // Update attribute values.
        glVertexAttribPointer(ATTRIB_VERTEX, COUNT_COORD, GL_FLOAT, 0, 0, this->simulatedObjectDrawn->getPointers()->p);
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        
        glUniformMatrix4fv(matrixView, 1, 0, this->simulatedObjectDrawn->getMatrixTransformation());
        
        glDrawArrays(this->simulatedObjectDrawn->getMode(), 0, this->simulatedObjectDrawn->getPointers()->count);
        
                
        if (this->simulatedObjectDrawn->isSelected()) {
            // define color for points
            glVertexAttribPointer(ATTRIB_COLOR, CHANNEL_COLOR, GL_UNSIGNED_BYTE, 1, 0, this->simulatedObjectDrawn->getColor()->color);
            glEnableVertexAttribArray(ATTRIB_COLOR);
            
            // Update attribute values.
            glVertexAttribPointer(ATTRIB_VERTEX, COUNT_COORD, GL_FLOAT, 0, 0, this->simulatedObjectDrawn->getPointers()->p);
            glEnableVertexAttribArray(ATTRIB_VERTEX);
            
            glUniformMatrix4fv(matrixView, 1, 0, this->simulatedObjectDrawn->getMatrixTransformation());
            
            glDrawArrays(GL_POINTS, 0, this->simulatedObjectDrawn->getPointers()->count);
        }
        
        if (this->simulatedObjectDrawn->isShowBBox()) {
            // define color for points
            glVertexAttribPointer(ATTRIB_COLOR, CHANNEL_COLOR, GL_UNSIGNED_BYTE, 1, 0, this->simulatedObjectDrawn->getColor()->color);
            glEnableVertexAttribArray(ATTRIB_COLOR);

            // Update attribute values.
            glVertexAttribPointer(ATTRIB_VERTEX, COUNT_COORD, GL_FLOAT, 0, 0, this->simulatedObjectDrawn->getBBox()->pointers->p);
            glEnableVertexAttribArray(ATTRIB_VERTEX);
            
            glUniformMatrix4fv(matrixView, 1, 0, this->simulatedObjectDrawn->getMatrixTransformation());

            glDrawArrays(GL_LINE_LOOP, 0, this->simulatedObjectDrawn->getBBox()->pointers->count);
        }
    }
}