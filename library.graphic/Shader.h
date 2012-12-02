//
//  Shader.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 10/06/12.
//  
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include "CommonGraphics.h"

//!Class responsible for managing the shaders used in the application
class Shader {
private:
    GLuint program;
    const char * vertShaderSource;
    const char * fragShaderSource;
    std::map<std::string, GLuint> * mapGLSLVars;

    bool loadShaders();
    bool compileShader(GLuint * shader, GLenum type, const char * file);
    bool linkProgram(GLuint prog);
    bool validateProgram(GLuint prog);
public:
    Shader();
    ~Shader();
    GLuint getProgram();
    GLuint getVar(std::string _var);
};

#endif
