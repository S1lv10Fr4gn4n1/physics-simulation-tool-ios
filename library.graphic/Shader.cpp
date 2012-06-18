//
//  Shader.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 10/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Shader.h"

using namespace std;

Shader::Shader(const char * _vertShaderSource, const char * _fragShaderSource, const char * _geomShaderSource)
{
    vertShaderSource = _vertShaderSource;
    fragShaderSource = _fragShaderSource;
    geomShaderSource = _geomShaderSource;
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

#pragma mark -  OpenGL ES 2 shader compilation
bool Shader::loadShaders()
{
    GLuint vertShader, fragShader; //geomShader;
    string vertShaderPathname, fragShaderPathname;
    
    // Create shader program.
    program = glCreateProgram();
    
    // Create and compile vertex shader.
    if (!compileShader(&vertShader, GL_VERTEX_SHADER, vertShaderSource)) {
        cout << "Failed to compile vertex shader" << endl;
        return false;
    }
    
    // Create and compile fragment shader.
    if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, fragShaderSource)) {
        cout << "Failed to compile fragment shader" << endl;
        return false;
    }

//    if (!compileShader(&geomShader, GL_ge, geomShaderSource)) {
//        cout << "Failed to compile fragment shader" << endl;
//        return false;
//    }
    
    // Attach vertex shader to program.
    glAttachShader(program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(program, ATTRIB_COLOR, "color");
    
    // Link program.
    if (!linkProgram(program)) {
        cout << "Failed to link program: " << program << endl;;
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }
        
        return false;
    }
    
    //    TODO
    // Get uniform locations.
    //    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    //    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(program, fragShader);
        glDeleteShader(fragShader);
    }
    
    return false;
}

bool Shader::compileShader(GLuint * shader, GLenum type, const char * file)
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *) file;
    if (!source) {
        cout << "Failed to load vertex shader" << endl;
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        cout << "Shader compile log:\n" << log << endl;
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    
    return true;
}

bool Shader::linkProgram(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        cout << "Program link log:\n%s" << log << endl;
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}

bool Shader::validateProgram(GLuint prog)
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        cout << "Program validate log:\n%s" << log << endl;
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}

string Shader::loadFile(string _pathFile) 
{
    string result;
    
	ifstream infile;
    
    char* str = new char[_pathFile.length()];
    strcpy(str, _pathFile.c_str());
    
	infile.open(str);
    
    if (!infile) {
        std::cerr << "File not open" << endl;
        return NULL;
    }
    
	while(!infile.eof()) {
		getline(infile, result);
		cout << result << endl;
	}
    
	infile.close();
    
    return result;
    
}

GLuint Shader::getProgram()
{
    return program;
}