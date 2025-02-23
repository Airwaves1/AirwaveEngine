#shader vert
#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec3 v_localPosition;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

void main(){
    v_localPosition = a_position;

    mat4 rotView = mat4(mat3(u_viewMatrix)); // remove translation
    vec4 clipPos = u_projectionMatrix * rotView * vec4(a_position, 1.0);

    gl_Position = clipPos.xyww;
}

#shader frag
#version 460 core

out vec4 fragColor;
in vec3 v_localPosition;

uniform samplerCube u_backgroundMap;

#include "shader_chunk/tone_mapping.glsl"

void main(){
    vec3 env_color = texture(u_backgroundMap, normalize(v_localPosition)).rgb;

    env_color = env_color / (env_color + vec3(1.0));
    env_color = pow(env_color, vec3(1.0/2.8));

    fragColor = vec4(env_color, 1.0);    
}