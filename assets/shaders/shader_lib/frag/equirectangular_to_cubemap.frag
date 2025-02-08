#version 460 core

out vec4 fragColor;
in vec3 v_localPosition;

const vec2 invAtan = vec2(0.1591, 0.3183);

uniform sampler2D u_equirectangularMap;
vec2 equirectUv(vec3 dir) {
    vec2 uv = vec2(atan(dir.z, dir.x), asin(dir.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = equirectUv(normalize(v_localPosition));
    vec3 color = texture(u_equirectangularMap, uv).rgb;

    fragColor = vec4(color, 1.0);
}