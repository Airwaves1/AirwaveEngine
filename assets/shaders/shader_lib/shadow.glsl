#shader vert
#version 460
layout(location = 0) in vec3 a_position;

uniform mat4 u_worldMatrix;
uniform mat4 u_lightSpaceMatrix;

void main()
{
    gl_Position = u_lightSpaceMatrix * u_worldMatrix * vec4(a_position, 1.0);
}

#shader frag
#version 460

void main()
{
    gl_FragDepth = gl_FragCoord.z;
}