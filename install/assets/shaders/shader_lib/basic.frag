#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;

uniform vec4 u_color;         // 基础颜色


uniform sampler2D u_texture_0;

void main() {

    vec4 finalColor = u_color;

    o_fragColor = finalColor;
}
