#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;
uniform mat3 u_normalMatrix;

void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * vec4(a_position, 1.0);
    v_uv = a_texCoords;
    v_normal = u_normalMatrix * a_normal;
    v_fragPos = vec3(u_worldMatrix * vec4(a_position, 1.0));
}