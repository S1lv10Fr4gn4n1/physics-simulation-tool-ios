//
//  Shader.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 10/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H


#include <iostream>
#include <fstream>
#include <string>

#include "CommonGraphics.h"

class Shader {
private:
    GLuint program;
    const char * vertShaderSource;
    const char * fragShaderSource;
    const char * geomShaderSource;
    
    std::string loadFile(std::string _pathFile);
public:
    Shader(const char * _vertShaderSource, const char * _fragShaderSource, const char * _geomShaderSource);
    ~Shader();
    bool loadShaders();
    bool compileShader(GLuint * shader, GLenum type, const char * file);
    bool linkProgram(GLuint prog);
    bool validateProgram(GLuint prog);
    
    GLuint getProgram();
};

#endif
