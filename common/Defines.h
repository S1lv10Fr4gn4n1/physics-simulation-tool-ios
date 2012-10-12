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
#define USE_LAST_TIME_GLKIT 1

// TREE
#define USE_TREE 0
#define DEPTH_TREE 3
#define WIDTH_SCENE 3.0f

// MINIMUM MOTION
#define SLEEP_EPSILON 0.01f

// DEFINE CONTROLLER
#define ZOOM_IN_LIMIT 5.0f
#define ZOOM_OUT_LIMIT 0.7f
#define ZOOM_SCALE 0.03f
#define ZOOM_INIT 2.0f
#define PAN_SCALE 0.005f

#define CHANNEL_COLOR 4
#define COUNT_COORD 3


// DEFAULT VALUES FOR OBJECTS
#define ACCELERATION_GRAVITY 9.8f
#define COEF_FRICTION 0.9f
#define COEF_RESTITUTION 0.4f
#define COEF_LINEAR_DAMPING 0.99f
#define COEF_ANGULAR_DAMPING 0.8f
#define MASS 4.0f
#define CAN_AWAKE 1
#define POSITION_X 0.0f
#define POSITION_Y 0.5f
#define POSITION_Z 0.0f


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
