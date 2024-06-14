#version 460 core
layout (location = 0) in vec3 a_Pos;   // 顶点位置
layout (location = 1) in vec3 a_Normal; // 顶点法线
layout (location = 2) in vec2 a_TexCoords; // 纹理坐标

out vec3 v_FragPos; // 片段的世界坐标
out vec3 v_Normal;  // 片段的法线
out vec2 v_TexCoords; // 纹理坐标

uniform mat4 u_Transform; 
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    
    v_Normal = a_Normal;
    v_TexCoords = a_TexCoords;

    gl_Position =  u_Projection * u_View * u_Transform * vec4(a_Pos, 1.0); // 应用变换矩阵
}
