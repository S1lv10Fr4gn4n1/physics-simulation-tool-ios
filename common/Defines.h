//
//  Defines.h
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani on 13/08/12.
//
//

#include <float.h>

#ifndef DEFINES_H
#define PDEFINES_H

// 3D
#define _3D_

// FRAME PER SECOND
#define FRAME_PER_SECOND 60

// USE LAST TIME DRAW OF GLKIT
// else duration to be 0.02f
#define USE_LAST_TIME_GLKIT 0

// TREE
#define USE_TREE 0
#define DEPTH_TREE 3
#define WIDTH_SCENE 3.0f

// MINIMUM MOTION
#define SLEEP_EPSILON 0.01f

// DEFINE CONTROLLER
#define ZOOM_IN_LIMIT 4.0f
#define ZOOM_OUT_LIMIT 0.1f
#define ZOOM_SCALE 0.05f
#define ZOOM_INIT 1.0f
#define PAN_SCALE 0.005f

#define CHANNEL_COLOR 4
#define COUNT_COORD 3



// DEFINE PRECISION
typedef float real;

#define real_pow powf
#define real_sqrt sqrtf
#define real_abs fabsf

#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#define real_tan tanf

#define REAL_MAX FLT_MAX
#define R_PI 3.14159f


#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)


#endif
