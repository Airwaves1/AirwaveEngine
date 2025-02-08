#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out vec2 v_uv;
out vec3 v_normal;

uniform mat4 u_worldMatrix;
uniform mat4 u_cameraWorldInverse;
uniform mat4 u_projectionMatrix;

void main() {
    vec4 worldPosition = u_worldMatrix * vec4(a_position, 1.0);

    gl_Position = u_projectionMatrix * u_cameraWorldInverse * worldPosition;

    v_uv = a_texcoord;
    v_normal = mat3(transpose(inverse(u_worldMatrix))) * a_normal;
}