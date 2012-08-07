//
//  CommonGraphics.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 14/06/12.
//  
//

#ifndef COMMONGRAPHICS_H
#define COMMONGRAPHICS_H

/// COMMON INCLUDES
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "CommonStruct.h"

/// COMMON ENUN FOR LIBRARY
enum
{
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    ATTRIB_NORMAL
};

#define UNIFORM_MODELVIEWPROJECTION_MATRIX "UNIFORM_MODELVIEWPROJECTION_MATRIX"
#define UNIFORM_ORTHO_MATRIX "UNIFORM_ORTHO_MATRIX"
#define UNIFORM_NORMAL_MATRIX "UNIFORM_NORMAL_MATRIX"

#endif
