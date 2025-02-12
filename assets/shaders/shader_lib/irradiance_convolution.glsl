#shader vert
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

out vec3 v_localPosition;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

void main()
{
    v_localPosition = a_position;

    gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position, 1.0);
}

#shader frag
#version 460 core

out vec4 fragColor;
in vec3 v_localPosition;

#include "shader_chunk/common.glsl"

uniform samplerCube u_environmentMap;

void main()
{
    vec3 N = normalize(v_localPosition);

    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += texture(u_environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }

    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    fragColor = vec4(irradiance, 1.0);
}