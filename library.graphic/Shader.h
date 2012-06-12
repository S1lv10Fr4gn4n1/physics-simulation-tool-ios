//
//  Shader.h
//  Physics.Tool
//
//  Created by Silvio Fragnani da Silva on 10/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <iostream>
#include <fstream>
#include <string>


class Shader {
private:
    GLuint program;
    const char* vertShaderSource;
    const char* fragShaderSource;
    const char* geomShaderSource;
    
    std::string loadFile(std::string pathFile);
public:
    Shader(const char* _vertShaderSource, const char* _fragShaderSource, const char* _geomShaderSource);
    ~Shader();
    bool loadShaders();
    bool compileShader(GLuint *shader, GLenum type, const char* file);
    bool linkProgram(GLuint prog);
    bool validateProgram(GLuint prog);
    
    GLuint getProgram();
};



#endif
