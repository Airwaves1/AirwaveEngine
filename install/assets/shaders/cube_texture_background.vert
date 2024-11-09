#version 450
layout(location = 0) in vec3 a_position;

out vec3 v_texcoord;

uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;


void main()
{
    v_texcoord = a_position;
    gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position, 1.0);
    gl_Position = gl_Position.xyww;
}