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

float calculateShadowFactor(sampler2D shadowMap, vec3 projCoords, float bias)
{
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;

    return shadow;
}

// 基础版3x3 PCF（9次采样）
float calculateShadowFactor_PCF(sampler2D shadowMap, vec3 projCoords, float bias)
{
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            float closestDepth = texture(shadowMap, projCoords.xy + offset).r;
            shadow += (projCoords.z - bias) > closestDepth ? 0.0 : 1.0;
        }
    }
    return shadow / 9.0;
}

// 优化版5x5旋转采样（13次采样实现25次效果）
float calculateShadowFactor_PCF_Optimized(sampler2D shadowMap, vec3 projCoords, float bias)
{
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;
    const vec2 offsets[25] = vec2[](
        vec2(-0.92, -0.92), vec2(-0.58, -0.58), vec2(-0.24, -0.24), // 旋转采样模式
        vec2( 0.09,  0.09), vec2( 0.43,  0.43), vec2( 0.77,  0.77),
        vec2(-0.92,  0.09), vec2(-0.58,  0.43), vec2(-0.24,  0.77),
        vec2( 0.09, -0.92), vec2( 0.43, -0.58), vec2( 0.77, -0.24),
        vec2(-0.92,  0.77), vec2(-0.58,  0.43), vec2(-0.24,  0.09),
        vec2( 0.09, -0.24), vec2( 0.43, -0.58), vec2( 0.77, -0.92),
        vec2(-0.92, -0.24), vec2(-0.58, -0.58), vec2(-0.24, -0.92),
        vec2( 0.09,  0.77), vec2( 0.43,  0.43), vec2( 0.77,  0.09),
        vec2( 0.00,  0.00) // 中心采样
    );
    
    for(int i = 0; i < 13; ++i) { // 利用对称性减少采样次数
        float sample1 = texture(shadowMap, projCoords.xy + offsets[i]*texelSize).r;
        float sample2 = texture(shadowMap, projCoords.xy - offsets[i]*texelSize).r;
        shadow += (projCoords.z - bias) > sample1 ? 0.0 : 1.0;
        shadow += (projCoords.z - bias) > sample2 ? 0.0 : 1.0;
    }
    return shadow / 25.0;
}

float calculateShadowFactor_PCSS(sampler2D shadowMap, vec3 projCoords, float bias, float lightSize, float minSamples)
{
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    // Step 1: Blocker Search - 在小范围内找到遮挡者的平均深度
    float avgBlockerDepth = 0.0;
    int blockerCount = 0;
    const int blockerSampleCount = 16;
    
    for(int i = 0; i < blockerSampleCount; i++)
    {
        vec2 sampleOffset = texelSize * vec2(sin(float(i)), cos(float(i))) * 3.0;
        float sampleDepth = texture(shadowMap, projCoords.xy + sampleOffset).r;
        
        if (sampleDepth < projCoords.z - bias)
        {
            avgBlockerDepth += sampleDepth;
            blockerCount++;
        }
    }

    if (blockerCount == 0)
        return 1.0; // 如果没有找到遮挡者，直接返回无阴影

    avgBlockerDepth /= float(blockerCount);

    // Step 2: Penumbra Size Calculation - 计算软阴影核大小
    float penumbraRatio = (projCoords.z - avgBlockerDepth) / avgBlockerDepth;
    float filterRadius = lightSize * penumbraRatio;

    // Step 3: PCF Filtering - 使用自适应核大小进行阴影过滤
    float shadow = 0.0;
    int filterSampleCount = clamp(int(minSamples + filterRadius * 8.0), 1, 16);
    float totalSamples = float(filterSampleCount * filterSampleCount);

    for(int x = -filterSampleCount/2; x <= filterSampleCount/2; x++)
    {
        for(int y = -filterSampleCount/2; y <= filterSampleCount/2; y++)
        {
            vec2 offset = vec2(x, y) * texelSize;
            float sampleDepth = texture(shadowMap, projCoords.xy + offset).r;
            shadow += (projCoords.z - bias) > sampleDepth ? 0.0 : 1.0;
        }
    }

    return shadow / totalSamples;
}


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
        switch(light.type)
        {
            case 0:
            {
                vec3 L = normalize(-light.direction);
                vec3 H = normalize(V + L);

                vec3 radiance = light.color * light.intensity;

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

                // shadow
                vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(position, 1.0);
                vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
                projCoords = projCoords * 0.5 + 0.5;

                float bias = max(light.shadowBias * 2.0 * (1.0 - dot(N, L)), light.shadowBias);
                // float shadow = calculateShadowFactor(u_shadowMaps[i], projCoords, bias);
                // float shadow = calculateShadowFactor_PCF_Optimized(u_shadowMaps[i], projCoords, bias);
                float shadow = calculateShadowFactor_PCSS(u_shadowMaps[i], projCoords, bias, 0.01, 16.0);
                Lo *= shadow;
            }
            break;
            case 1:
            {
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
            break;
            case 2:
            {

            }
            break;
            default:
                break;
        }

    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
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
