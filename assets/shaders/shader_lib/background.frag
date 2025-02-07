#version 460 core

out vec4 fragColor;
in vec3 v_localPosition;

uniform samplerCube u_backgroundMap;

void main(){
    vec3 env_color = texture(u_backgroundMap, normalize(v_localPosition)).rgb;

    env_color = env_color / (env_color + vec3(1.0));
    env_color = pow(env_color, vec3(1.0/2.2));

    fragColor = vec4(env_color, 1.0);
}