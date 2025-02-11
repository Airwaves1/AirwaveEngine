#shader vert
#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec3 v_localPosition;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

void main(){
    v_localPosition = a_position;

    gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position, 1.0);
}

#shader frag
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