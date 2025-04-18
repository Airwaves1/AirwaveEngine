#shader vertex

#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

out vec2 v_uv;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
    v_uv = a_uv;
    
    v_normal = mat3(transpose(inverse(u_model))) * a_normal;
}

#shader fragment
#version 460 core

out vec4 fragColor;
in vec2 v_uv;
in vec3 v_normal;

uniform sampler2D u_texture_0;

void main()
{
    vec4 texColor = texture(u_texture_0, v_uv);
    fragColor = texColor;
}
