#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D u_diffuseMap;

void main() {
    frag_color = texture(u_diffuseMap, v_uv);
}
