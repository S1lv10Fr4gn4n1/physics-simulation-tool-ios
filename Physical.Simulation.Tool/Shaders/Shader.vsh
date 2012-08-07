//
//  Shader.vsh
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 06/05/12.
//  
//

attribute vec4 position;
attribute vec4 color;
varying lowp vec4 colorVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 orthoMatrix;

void main()
{
    gl_Position = orthoMatrix * modelViewProjectionMatrix * position;
    gl_PointSize = 4.0;
    colorVarying = color;
}