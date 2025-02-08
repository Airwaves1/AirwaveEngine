// Reinhard 色调映射（带曝光控制）
// 这是一种简单的色调映射算法，能够平滑地压缩高亮区域
vec3 ReinhardToneMapping(vec3 color, float exposure) {
    color *= exposure;  // 应用曝光控制
    return color / (color + vec3(1.0));  // Reinhard 映射公式
}

// Uncharted 2 电影级色调映射
// 该算法来自《神秘海域 2》，用于创造电影般的视觉效果
vec3 Uncharted2ToneMapping(vec3 color) {
    // 预定义常数参数
    const float A = 0.15;
    const float B = 0.50;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.02;
    const float F = 0.30;
    const float W = 11.2; // 归一化白点

    // 应用 Uncharted 2 电影色调映射公式
    color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;

    // 计算白点缩放系数，确保正确的曝光平衡
    vec3 whiteScale = ((vec3(W) * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    
    return color / whiteScale;  // 归一化颜色
}

// ACES 电影级色调映射
// ACES（Academy Color Encoding System）是电影和游戏行业常用的标准
vec3 ACESFilmToneMapping(vec3 color) {
    // 输入变换矩阵（将 RGB 转换到 ACES 颜色空间）
    const mat3 ACESInputMat = mat3(
        0.59719, 0.07600, 0.02840,
        0.35458, 0.90834, 0.13383,
        0.04823, 0.01566, 0.83777
    );

    // 输出变换矩阵（将 ACES 颜色转换回标准 RGB）
    const mat3 ACESOutputMat = mat3(
        1.60475, -0.10208, -0.00327,
        -0.53108,  1.10813, -0.07276,
        -0.07367, -0.00605,  1.07602
    );

    // 转换到 ACES 颜色空间
    color = ACESInputMat * color;
    
    // 应用 ACES 色调映射公式
    color = (color * (color + 0.0245786)) / (color * (0.983729 * color + 0.4329510) + 0.238081);
    
    // 转换回标准 RGB 颜色空间
    color = ACESOutputMat * color;

    // 限制颜色范围，确保值在 0.0 - 1.0 之间
    return clamp(color, 0.0, 1.0);
}

// Uchimura 色调映射
// 适用于游戏，能够在亮暗区域之间提供平滑过渡，并可调整参数
// UchimuraToneMapping(color, 1.0, 1.0, 0.22, 0.4, 1.33, 0.0);
vec3 UchimuraToneMapping(vec3 color, float P, float a, float m, float l, float c, float b) {
    float l0 = ((P - m) * l) / a; // 低亮度过渡点
    float L0 = m - m / a; // 暗部过渡区域
    float L1 = m + (1.0 - m) / a; // 高光过渡区域
    float S0 = m + l0; // 开始高光压缩
    float S1 = m + a * l0; // 结束高光压缩
    float C2 = (a * P) / (P - S1); // 压缩因子
    float CP = -C2 / P; // 预计算参数

    // 不同区域的权重
    vec3 w0 = vec3(1.0 - smoothstep(0.0, m, color)); // 阴影区域
    vec3 w2 = vec3(step(m + l0, color)); // 高光区域
    vec3 w1 = vec3(1.0 - w0 - w2); // 中间过渡区域

    // 在不同的亮度区域应用不同的色调映射
    vec3 T = m * pow(color / m, vec3(c)) + b; // 暗部色调映射
    vec3 S = P - (P - S1) * exp(CP * (color - S0)); // 高光色调映射
    vec3 L = m + a * (color - m); // 线性过渡区域

    // 按照区域混合最终颜色
    return T * w0 + L * w1 + S * w2;
}

// 电影色调映射
// 这个算法模拟电影胶片的色彩曲线，能够提供自然的亮暗过渡
vec3 FilmicToneMapping(vec3 color) {
    color = max(vec3(0.0), color - vec3(0.004)); // 避免曝光过低导致的失真
    color = (color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06); // 电影色调映射曲线
    return color;
}
