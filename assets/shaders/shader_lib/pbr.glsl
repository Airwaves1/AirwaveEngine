#shader vert
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;
uniform mat3 u_normalMatrix;

void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * vec4(a_position, 1.0);
    v_uv = a_texCoords;
    v_normal = u_normalMatrix * a_normal;
    v_fragPos = vec3(u_worldMatrix * vec4(a_position, 1.0));
}

// ############################################################################

#shader frag
#version 460 core

out vec4 FragColor;

in vec2 v_uv;
in vec3 v_fragPos;
in vec3 v_normal;

#include "shader_chunk/common.glsl"
const int MAX_LIGHT_COUNT = 16;

struct PBRMaterial
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicRoughnessMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;

    samplerCube irradianceMap;
    samplerCube prefilterMap;
    sampler2D brdf_lut;
};

struct Light
{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform vec3 u_cameraPosition;

uniform int u_lightCount;
uniform Light u_lights[MAX_LIGHT_COUNT]; // max lights
uniform PBRMaterial u_material;

//-----------------------------------------------------------
vec3 getNormalFromMap(sampler2D normalMap, vec2 uv, vec3 fragPos, vec3 normal)
{
    vec3 tangentNormal = texture(normalMap, uv).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(fragPos);
    vec3 Q2 = dFdy(fragPos);
    vec2 st1 = dFdx(uv);
    vec2 st2 = dFdy(uv);

    vec3 N = normalize(normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// ----------------------------------------------------------------------------
float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   
// ----------------------------------------------------------------------------

void main()
{
    vec3 albedo = pow(texture(u_material.albedoMap, v_uv).rgb, vec3(2.2)) * u_material.albedo;

    vec3 mr = texture(u_material.metallicRoughnessMap, v_uv).rgb; 

    float roughness = texture(u_material.roughnessMap, v_uv).r * u_material.roughness * mr.g;
    float metallic = texture(u_material.metallicMap, v_uv).r * u_material.metallic * mr.b;
    float ao = texture(u_material.aoMap, v_uv).r * u_material.ao;

    // vec3 N = normalize(v_normal);
    vec3 N = getNormalFromMap(u_material.normalMap, v_uv, v_fragPos, v_normal);
    vec3 V = normalize(u_cameraPosition - v_fragPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_lightCount; ++i)
    {
        // Light light = u_lights[i];
        Light light;
        light.position = vec3(0.0, 0.0, 10.0);
        light.color = vec3(1.0);
        light.intensity = 100.0;

        // calculate per-light radiance
        vec3 L = normalize(light.position - v_fragPos);
        vec3 H = normalize(V + L);
        float distance = length(light.position - v_fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.color * light.intensity * attenuation;

        // Cook-Torrance BRDF
        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.01;
        vec3 specular = numerator / denominator;

        vec3 kS = F;

        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(u_material.irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(u_material.prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(u_material.brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.9));

    color = Lo;

    FragColor = vec4(color, 1.0);
}