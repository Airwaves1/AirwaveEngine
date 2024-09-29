#version 450

layout(location = 0) in vec3 v_color;

out vec4 fragColor;

void main() {
    fragColor = vec4(vec3(v_color), 1.0);
}