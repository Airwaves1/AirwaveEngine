#version 460 core

const float PI = 3.14159265359;

out vec4 FragColor;

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_fragPosition;

struct PBRMaterial {
    float roughness;
    float metallic;
    float ao;

    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
};

struct Light {
    int type; // 0: directional, 1: point, 2: spot
    vec3 position;
    vec3 direction;

    vec3 color;
    float intensity;
};

uniform vec3 u_cameraPosition;
uniform vec3 u_color;

uniform PBRMaterial u_material;
uniform Light u_lights[16]; // 16 lights max
uniform int u_lightCount;

/*
*   法线分布函数
*/
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0));
    denom = PI * denom * denom;

    return nom / denom;
}

/*
*   几何函数
*/
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

/*
*   几何项
*/
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

/*
*   菲涅尔方程
*/
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    vec3 diffuse = u_color + texture(u_material.diffuseMap, v_uv).rgb;

    vec3 N = normalize(v_normal);

    vec3 V = normalize(u_cameraPosition - v_fragPosition);

    // 反射率
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, diffuse, u_material.metallic);

    // 光照计算
    vec3 Lo = vec3(0.0);

    vec3 testColor = vec3(0.0);

    for(int i = 0; i < u_lightCount; i++) {
        vec3 L = normalize(u_lights[i].position - v_fragPosition);
        vec3 H = normalize(V + L);
        float distance = length(u_lights[i].position - v_fragPosition);

        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_lights[i].color * attenuation;

        float NDF = distributionGGX(N, H, u_material.roughness);
        float G = geometrySmith(N, V, L, u_material.roughness);
        vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - u_material.metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * diffuse / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.1) * diffuse;

    vec3 color = ambient + Lo;

    FragColor = vec4(color, 1.0);
    // FragColor = vec4(testColor, 1.0);
    // FragColor = vec4(u_lights[0].color, 1.0);
}