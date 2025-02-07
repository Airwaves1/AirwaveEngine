#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec3 v_localPosition;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;


void main(){
    v_localPosition = a_position;
    gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position, 1.0);
}