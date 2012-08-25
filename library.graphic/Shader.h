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

class Shader {
private:
    GLuint program;
    std::map<std::string, GLuint> * mapGLSLVars;
    const char * vertShaderSource;
    const char * fragShaderSource;
public:
    Shader(const char * _vertShaderSource, const char * _fragShaderSource);
    ~Shader();
    
    bool loadShaders();
    bool compileShader(GLuint * shader, GLenum type, const char * file);
    bool linkProgram(GLuint prog);
    bool validateProgram(GLuint prog);
    
    GLuint getProgram();
    GLuint getVar(std::string _var);
};

#endif
