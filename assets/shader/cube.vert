#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()    
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));

    //正规矩阵，用于法向量的变换,防止不等比例变换后，法向量不垂直
    Normal = mat3(transpose(inverse(model))) * normal;

    TexCoords = texCoords;

}