//
//  MainGraphic.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 12/06/12.
//  
//

#include "MainGraphic.h"

MainGraphic::MainGraphic()
{
    this->shader = NULL;
    this->simulatedObjectDrawn = NULL;
    this->initializeShader();
}

MainGraphic::~MainGraphic()
{
    if (this->shader) {
        delete this->shader;
    }
    if (this->simulatedObjectDrawn) {
        delete this->simulatedObjectDrawn;
    }

    this->shader = NULL;
    this->simulatedObjectDrawn = NULL;
}

void MainGraphic::initializeShader()
{
    this->shader = new Shader();
    glEnable(GL_DEPTH_TEST);
}

Shader * MainGraphic::getShader()
{
    return this->shader;
}

void MainGraphic::MainGraphic::draw(World * _world)
{    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(this->shader->getProgram());
    
    GLuint matrixView = this->shader->getVar(UNIFORM_MODELVIEWPROJECTION_MATRIX);
#if defined (_3D_)
    GLuint matrixLookAt = this->shader->getVar(UNIFORM_LOOKAT_MATRIX);
    GLuint matrixPerspective = this->shader->getVar(UNIFORM_PERSPECTIVE_MATRIX);
#else
    GLuint matrixOrtho = this->shader->getVar(UNIFORM_ORTHO_MATRIX);
#endif

    for (int i=0; i< _world->getSimulatedObjects()->size(); i++) {
        this->simulatedObjectDrawn = _world->getSimulatedObjects()->at(i);
        
        // define color for simulated
        glVertexAttribPointer(ATTRIB_COLOR, CHANNEL_COLOR, GL_UNSIGNED_BYTE, 1, 0, this->simulatedObjectDrawn->getColor());
        glEnableVertexAttribArray(ATTRIB_COLOR);
        
        glVertexAttribPointer(ATTRIB_VERTEX, COUNT_COORD, GL_FLOAT, 0, 0, this->simulatedObjectDrawn->getVectors());
        glEnableVertexAttribArray(ATTRIB_VERTEX);

        glUniformMatrix4fv(matrixView, 1, 0, this->simulatedObjectDrawn->getMatrixTransformation().data);
    #if defined (_3D_)
        glUniformMatrix4fv(matrixLookAt, 1, 0, _world->getLookAtMatrix().data);
        glUniformMatrix4fv(matrixPerspective, 1, 0, _world->getPerspectiveMatrix().data);
    #else
        glUniformMatrix4fv(matrixOrtho, 1, 0, _world->getOrthoMatrix());
    #endif
        glDrawArrays(this->simulatedObjectDrawn->getMode(), 0, this->simulatedObjectDrawn->getVectorsAux()->size());
        
                
        if (this->simulatedObjectDrawn->isSelected()) {
            // define color for points
            glVertexAttribPointer(ATTRIB_COLOR, CHANNEL_COLOR, GL_UNSIGNED_BYTE, 1, 0, this->simulatedObjectDrawn->getColorVectors());
            glEnableVertexAttribArray(ATTRIB_COLOR);
            
            glVertexAttribPointer(ATTRIB_VERTEX, COUNT_COORD, GL_FLOAT, 0, 0, this->simulatedObjectDrawn->getVectors());
            glEnableVertexAttribArray(ATTRIB_VERTEX);
            
            glUniformMatrix4fv(matrixView, 1, 0, this->simulatedObjectDrawn->getMatrixTransformation().data);
        #if defined (_3D_)
            glUniformMatrix4fv(matrixLookAt, 1, 0, _world->getLookAtMatrix().data);
            glUniformMatrix4fv(matrixPerspective, 1, 0, _world->getPerspectiveMatrix().data);
        #else
            glUniformMatrix4fv(matrixOrtho, 1, 0, _world->getOrthoMatrix());
        #endif
            glDrawArrays(GL_POINTS, 0, this->simulatedObjectDrawn->getVectorsAux()->size());
        }
        
        if (this->simulatedObjectDrawn->isShowBBox()) {
            // define color for points
            glVertexAttribPointer(ATTRIB_COLOR, CHANNEL_COLOR, GL_UNSIGNED_BYTE, 1, 0, this->simulatedObjectDrawn->getBBox()->color);
            glEnableVertexAttribArray(ATTRIB_COLOR);

            // Update attribute values.
            glVertexAttribPointer(ATTRIB_VERTEX, COUNT_COORD, GL_FLOAT, 0, 0, this->simulatedObjectDrawn->getBBox()->ptr);
            glEnableVertexAttribArray(ATTRIB_VERTEX);
            
            glUniformMatrix4fv(matrixView, 1, 0, this->simulatedObjectDrawn->getMatrixTransformation().data);
        #if defined (_3D_)
            glUniformMatrix4fv(matrixLookAt, 1, 0, _world->getLookAtMatrix().data);
            glUniformMatrix4fv(matrixPerspective, 1, 0, _world->getPerspectiveMatrix().data);
        #else
            glUniformMatrix4fv(matrixOrtho, 1, 0, _world->getOrthoMatrix());
        #endif

            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }
    }
}