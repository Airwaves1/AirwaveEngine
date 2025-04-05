#shader vertex

#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

out vec2 v_uv;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    v_uv = a_uv;
}

#shader fragment
#version 460 core

out vec4 fragColor;
in vec2 v_uv;

uniform sampler2D u_texture_0;

void main()
{
    // fragColor = vec4(v_uv, 0.0, 1.0);
    vec4 texColor = texture(u_texture_0, v_uv);
    fragColor = texColor;
}

