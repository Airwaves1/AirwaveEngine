#version 460

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

layout(location = 0) out vec3 v_normal;
layout(location = 1) out vec2 v_uv;
layout(location = 2) out vec3 v_worldPosition;


uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main() {
    vec4 worldPosition = u_modelMatrix * vec4(a_position, 1.0);
    gl_Position = u_projectionMatrix * u_viewMatrix * worldPosition;

    v_uv = a_uv;
    v_normal = normalize((u_modelMatrix * vec4(a_normal, 0.0)).xyz); // 将法线转换到世界空间
    v_worldPosition = worldPosition.xyz; // 传递世界空间位置
}
