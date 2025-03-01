struct Light
{
    int type; // 0: directional, 1: point, 2: spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;

    float shadowBias;
    float shadowRadius;
    float shadowStrength;
    float lightSize;
    mat4 lightSpaceMatrix;

};
uniform int u_lightCount;
uniform Light u_lights[MAX_LIGHT_COUNT]; // max lights
uniform sampler2D u_shadowMaps[MAX_LIGHT_COUNT]; // max lights