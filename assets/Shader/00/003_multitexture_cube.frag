#version 460 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

out vec4 FragColor;

void main()
{
	FragColor = texture(u_Texture0, v_TexCoord);  
	FragColor = mix(FragColor, texture(u_Texture1, v_TexCoord), 0.60);
}