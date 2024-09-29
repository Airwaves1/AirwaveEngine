#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;

void main() {
    vec3 normal = normalize(v_normal);
    normal = normal * 0.5 + 0.5;

    vec3 texColor_0 = texture(u_texture_0, v_uv).rgb;
    vec3 texColor_1 = texture(u_texture_1, v_uv).rgb;

    vec3 color = mix(texColor_0, texColor_1, 0.4);

    o_fragColor = vec4(color, 1.0);
}