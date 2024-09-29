#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;

void main() {
    vec3 normal = normalize(v_normal);
    normal = normal * 0.5 + 0.5;

    // o_fragColor = vec4(normal, 1.0);
    o_fragColor = vec4(normal, 1.0);
}