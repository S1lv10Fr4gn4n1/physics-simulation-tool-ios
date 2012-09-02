//
//  Shader.cpp
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 10/06/12.
//  
//

#include "Shader.h"

const char * vertexShader2D =
                                "attribute vec4 position;"
                                "attribute vec4 color;"
                                "varying lowp vec4 colorVarying;"
                                "uniform mat4 modelViewProjectionMatrix;"
                                "uniform mat4 orthoMatrix;"
                                "void main()"
                                "{"
                                "    gl_Position = orthoMatrix * modelViewProjectionMatrix * position;"
                                "    gl_PointSize = 4.0;"
                                "    colorVarying = color;"
                                "}";

const char * fragmentShader2D =
                                "varying lowp vec4 colorVarying;"
                                "void main()"
                                "{"
                                "    gl_FragColor = colorVarying;"
                                "}";


const char * vertexShader3D =
                                "attribute vec4 position;"
                                "attribute vec4 color;"
                                "varying lowp vec4 colorVarying;"
                                "uniform mat4 modelViewProjectionMatrix;"
                                "uniform mat4 lookAtMatrix;"
                                "uniform mat4 perspectiveMatrix;"
                                "void main()"
                                "{"
                                "    gl_Position = perspectiveMatrix * lookAtMatrix * modelViewProjectionMatrix * position;"
                                "    gl_PointSize = 4.0;"
                                "    colorVarying = color;"
                                "}";

const char * fragmentShader3D =
                                "varying lowp vec4 colorVarying;"
                                "void main()"
                                "{"
                                "    gl_FragColor = colorVarying;"
                                "}";

using namespace std;

Shader::Shader()
{
    this->mapGLSLVars = new map<string, GLuint>();
    
#if defined (_3D_)
    this->vertShaderSource = vertexShader3D;
    this->fragShaderSource = fragmentShader3D;
#else
    this->vertShaderSource = vertexShader2D;
    this->fragShaderSource = fragmentShader2D;
#endif
    
    this->loadShaders();
}

Shader::~Shader()
{
    glDeleteProgram(this->program);
    
    if (this->mapGLSLVars) {
        this->mapGLSLVars->clear();
        delete this->mapGLSLVars;
    }

    this->mapGLSLVars = NULL;
}

#pragma mark -  OpenGL ES 2 shader compilation
bool Shader::loadShaders()
{
    GLuint vertShader, fragShader;
    string vertShaderPathname, fragShaderPathname;
    
    // Create shader program.
    this->program = glCreateProgram();
    
    // Create and compile vertex shader.
    if (!this->compileShader(&vertShader, GL_VERTEX_SHADER, this->vertShaderSource)) {
        printf("Failed to compile vertex shader\n");
        return false;
    }
    
    // Create and compile fragment shader.
    if (!this->compileShader(&fragShader, GL_FRAGMENT_SHADER, this->fragShaderSource)) {
        printf("Failed to compile fragment shader\n");
        return false;
    }
    
    // Attach vertex shader to program.
    glAttachShader(this->program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(this->program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(this->program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(this->program, ATTRIB_COLOR, "color");
    
    // Link program.
    if (!this->linkProgram(this->program)) {
        printf("Failed to link program: %c\n", this->program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (this->program) {
            glDeleteProgram(this->program);
            this->program = 0;
        }
        
        return false;
    }

    this->mapGLSLVars->insert(std::pair<string, GLuint>(UNIFORM_MODELVIEWPROJECTION_MATRIX, glGetUniformLocation(this->program, "modelViewProjectionMatrix")));
    
#if defined (_3D_)
    this->mapGLSLVars->insert(std::pair<string, GLuint>(UNIFORM_LOOKAT_MATRIX, glGetUniformLocation(this->program, "lookAtMatrix")));
    this->mapGLSLVars->insert(std::pair<string, GLuint>(UNIFORM_PERSPECTIVE_MATRIX, glGetUniformLocation(this->program, "perspectiveMatrix")));
#else
    this->mapGLSLVars->insert(std::pair<string, GLuint>(UNIFORM_ORTHO_MATRIX, glGetUniformLocation(this->program, "orthoMatrix")));
#endif
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(this->program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(this->program, fragShader);
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
        printf("Failed to load vertex shader\n");
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
        printf("Shader compile log: %s\n",log);
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
        printf("Program link log:\n%s\n", log);
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
        printf("Program validate log:\n%s\n",log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return false;
    }
    
    return true;
}

GLuint Shader::getProgram()
{
    return this->program;
}

GLuint Shader::getVar(std::string _var)
{
    if (!this->mapGLSLVars) {
        return -1;
    }

    map<string, GLuint>::iterator iter;
    
    iter = this->mapGLSLVars->find(_var);
    if (iter != this->mapGLSLVars->end()) {
        return iter->second;
    }
    
    return -1;
}