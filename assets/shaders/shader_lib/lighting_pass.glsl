#shader vert
#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;

out vec2 v_uv;

void main()
{
    v_uv = a_texcoord;
    gl_Position = vec4(a_position, 1.0);
}

#shader frag
#version 460 core

out vec4 fragColor;

in vec2 v_uv;

#include "shader_chunk/common.glsl"
#include "shader_chunk/light_parm.glsl"
#include "shader_chunk/brdf.glsl"
#include "shader_chunk/tbn.glsl"

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo;
uniform sampler2D g_parameters;

uniform samplerCube u_irradianceMap;
uniform samplerCube u_prefilterMap;
uniform sampler2D u_brdf_lut;

uniform vec3 u_cameraPosition;

void main()
{
    // retrieve data from g-buffer
    vec3 position = texture(g_position, v_uv).xyz;

    vec3 normal = texture(g_normal, v_uv).xyz;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 albedo = texture(g_albedo, v_uv).rgb;
    vec4 parameters = texture(g_parameters, v_uv); // roughness, ao, 0, 0

    float roughness = parameters.r;
    float metallic = parameters.g;

    float ao = 1.0;

    vec3 V = normalize(u_cameraPosition - position);
    vec3 N = normalize(normal);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflection equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_lightCount; ++i)
    {
        Light light = u_lights[i];

        // calculate per-light radiance
        vec3 L = normalize(light.position - position);
        vec3 H = normalize(V + L);
        float distance = length(light.position - position);
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
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(u_irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(u_prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(u_brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    // vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    // color = vec3(N);

    fragColor = vec4(color, 1.0);
}
