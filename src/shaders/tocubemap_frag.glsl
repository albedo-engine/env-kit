#version 330 core

in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D uMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

void main() {

    vec3 fetch = normalize(fragPos);
    vec2 uv = vec2(atan(fetch.z, fetch.x), asin(fetch.y));
    uv *= invAtan;
    uv += 0.5;

    fragColor = vec4(texture(uMap, uv).rgb, 1.0);

}