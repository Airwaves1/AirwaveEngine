#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;

uniform vec4 u_color;         // 基础颜色

uniform sampler2D u_t_diffuse; // 漫反射纹理

void main() {

    vec4 finalColor = u_color;

    finalColor = texture(u_t_diffuse, v_uv);

    o_fragColor = finalColor;
}
