#version 460 core

in vec3 v_Normal;
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

out vec4 FragColor; // 片段的最终颜色

void main() {
    FragColor = texture(u_Texture, v_TexCoords);    
}
