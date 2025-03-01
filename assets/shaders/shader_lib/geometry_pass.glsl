#shader vert
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;

uniform mat4 u_worldMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_normalMatrix;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_fragPos;
out mat3 v_tbn;

void main()
{

    v_uv = a_texCoords;

    vec3 T = normalize(u_normalMatrix * a_tangent);
    vec3 N = normalize(u_normalMatrix * a_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    v_tbn = mat3(T, B, N);

    v_normal = N;

    vec4 worldPos = u_worldMatrix * vec4(a_position, 1.0);
    v_fragPos = worldPos.xyz;

    gl_Position = u_projectionMatrix * u_viewMatrix * worldPos;
}

// ############################################################################

#shader frag
#version 460 core

#define USE_ALBEDO_MAP
#define USE_NORMAL_MAP
#define USE_METALLIC_ROUGHNESS_MAP
#define USE_METALLIC_MAP
#define USE_ROUGHNESS_MAP
#define USE_AO_MAP

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_fragPos;
in mat3 v_tbn;

layout(location = 0) out vec3 g_position;
layout(location = 1) out vec4 g_normal;
layout(location = 2) out vec4 g_albedo;
layout(location = 3) out vec4 g_parameters;

#include "shader_chunk/common.glsl"
#include "shader_chunk/tbn.glsl"
#include "shader_chunk/pbr_parm.glsl"

void main()
{
    // position
    g_position = v_fragPos;

    vec2 uv = v_uv * u_material.uvScale;

    // normal
    vec3 N = getNormalFromMap(u_material.normalMap, uv, v_fragPos, v_normal);
    N = (N + 1.0) * 0.5;
    g_normal = vec4(N, 1.0);

    // albedo, metallic, roughness
    vec3 albedo = texture(u_material.albedoMap, uv).rgb * u_material.albedo;

    vec3 mr = texture(u_material.metallicRoughnessMap, uv).rgb;

    float metallic = texture(u_material.metallicMap, uv).r * mr.b * u_material.metallic;
    float roughness = texture(u_material.roughnessMap, uv).r * mr.g * u_material.roughness;

    g_albedo = vec4(albedo, 1.0);

    // parameters (roughness)
    g_parameters = vec4(roughness, metallic, 0.0, 1.0);
}
