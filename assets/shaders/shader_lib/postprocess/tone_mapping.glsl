#shader vert
#version 460 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texcoord;

out vec2 v_texcoord;

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    v_texcoord = a_texcoord;
}

#shader frag

#version 460 core

in vec2 v_texcoord;
out vec4 frag_color;

uniform sampler2D u_texture;

// 新增Uniform控制参数
uniform float u_exposure = 1.0;    // 曝光值
uniform int u_tonemapMode = 0;     // 色调映射算法选择

// Reinhard基础算法
vec3 reinhardToneMapping(vec3 hdrColor)
{
    return hdrColor / (hdrColor + vec3(1.0));
}

// ACES影视级算法（近似实现）
vec3 acesToneMapping(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// Uncharted2电影风格算法
vec3 uncharted2Tonemap(vec3 x)
{
    float A = 0.15;  // 阴影强度
    float B = 0.50;  // 亮度过渡
    float C = 0.10;  // 高光强度 
    float D = 0.20;  // 高光衰减
    float E = 0.02;  // 亮度偏移
    float F = 0.30;  // 白点调整
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

void main()
{
    vec2 uv = v_texcoord;
    vec3 hdrColor = texture(u_texture, uv).rgb * u_exposure;

    // 算法选择逻辑
    switch(u_tonemapMode)
    {
        case 1:  // ACES
            hdrColor = acesToneMapping(hdrColor);
            break;
        case 2:  // Uncharted2
            hdrColor = uncharted2Tonemap(hdrColor * 2.0) * 1.5;
            break;
        case 3:  // 线性压缩
            hdrColor = 1.0 - exp(-hdrColor * u_exposure);
            break;
        default: // Reinhard
            hdrColor = reinhardToneMapping(hdrColor);
    }

    // Gamma校正（sRGB空间转换）
    const float gamma = 2.2;
    hdrColor = pow(hdrColor, vec3(1.0 / gamma));

    frag_color = vec4(hdrColor, 1.0);
}
