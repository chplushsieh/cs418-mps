#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

uniform mat4 transform;
uniform float time;
#define M_PI 3.1415926535897932384626433832795

void main()
{
    float isClockwise = (gl_VertexID % 2)==0 ? 1: -1;
    
    float degree = time * 3.0 * isClockwise;
    float scale = ( 2 + gl_VertexID % 3 ) * 0.03 * (2 + sin( time * 0.7 * -isClockwise ));

    vec3 dance = vec3( scale * sin( degree ), scale * cos( degree ), 0);
    gl_Position = transform * vec4( dance + position, 1.0f);
    
    ourColor = color;
}
