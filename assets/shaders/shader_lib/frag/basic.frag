#version 460 core

out vec4 FragColor;

in vec2 v_uv;
in vec3 v_normal;

uniform vec3 u_cameraPosition;

uniform sampler2D u_map;
uniform vec3 u_color;

void main() {

    vec4 texColor = texture(u_map, v_uv);

    FragColor = vec4(vec3(texColor) + u_color, 1.0);
}