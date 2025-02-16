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
    v_uv = a_texCoords;

    v_normal = u_normalMatrix * a_normal;

    v_fragPos = vec3(u_worldMatrix * vec4(a_position, 1.0));

    gl_Position = u_projectionMatrix * u_viewMatrix * u_worldMatrix * vec4(a_position, 1.0);
}

// ############################################################################

#shader frag
#version 460 core

in vec2 v_uv;
in vec3 v_fragPos;
in vec3 v_normal;


out vec4 FragColor;


#include "shader_chunk/common.glsl"
#include "shader_chunk/tbn.glsl"
#include "shader_chunk/pbr_parm.glsl"
#include "shader_chunk/light_parm.glsl"


uniform vec3 u_cameraPosition;


#include "shader_chunk/brdf.glsl"

void main()
{

#ifdef USE_ALBEDO_MAP
    
        vec3 albedo = pow(texture(u_material.albedoMap, v_uv).rgb, vec3(2.2)) * u_material.albedo;
        
#else

        vec3 albedo = pow(u_material.albedo, vec3(2.2));

#endif  // USE_ALBEDO_MAP


#ifdef USE_METALLIC_ROUGHNESS_MAP

    vec3 mr = texture(u_material.metallicRoughnessMap, v_uv).rgb;

    #ifdef USE_METALLIC_MAP

        float metallic = texture(u_material.metallicMap, v_uv).r * mr.b * u_material.metallic;

    #else

        float metallic = mr.b * u_material.metallic;

    #endif  // USE_METALLIC_MAP

    #ifdef USE_ROUGHNESS_MAP

        float roughness = texture(u_material.roughnessMap, v_uv).r * mr.g * u_material.roughness;

    #else
    
        float roughness = mr.g * u_material.roughness;

    #endif

#else

    #ifdef USE_METALLIC_MAP

        float metallic = texture(u_material.metallicMap, v_uv).r * u_material.metallic;

    #else
    
        float metallic = u_material.metallic;

    #endif  // USE_METALLIC_MAP

    #ifdef USE_ROUGHNESS_MAP

        float roughness = texture(u_material.roughnessMap, v_uv).r * u_material.roughness;

    #else

        float roughness = u_material.roughness;

    #endif  // USE_ROUGHNESS_MAP

#endif  // USE_METALLIC_ROUGHNESS_MAP


#ifdef USE_AO_MAP

    float ao = texture(u_material.aoMap, v_uv).r * u_material.ao;

#else

    float ao = u_material.ao;

#endif

#ifdef USE_NORMAL_MAP

    vec3 N = getNormalFromMap(u_material.normalMap, v_uv, v_fragPos, v_normal);

#else

    vec3 N = normalize(v_normal);

#endif

    vec3 V = normalize(u_cameraPosition - v_fragPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    vec3 debug_color = vec3(0.0);
    for(int i = 0; i < u_lightCount; ++i)
    {
        Light light = u_lights[i];

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


#ifdef USE_IBL

    vec3 irradiance = texture(u_material.irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(u_material.prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(u_material.brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

#else

    vec3 ambient = vec3(0.03) * albedo * ao;

#endif  // USE_IBL

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.9));

    // color = vec3(N);

    FragColor = vec4(color, 1.0);
}