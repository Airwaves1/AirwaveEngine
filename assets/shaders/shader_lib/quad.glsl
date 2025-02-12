#shader vert
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;

out vec2 v_uv;

void main() {
    gl_Position = vec4(a_position, 1.0);
    v_uv = a_texcoord;
}

#shader frag
#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D u_diffuseMap;

void main() {
    frag_color = texture(u_diffuseMap, v_uv);
}
