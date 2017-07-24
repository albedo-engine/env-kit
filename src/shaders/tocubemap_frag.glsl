#version 330 core

#define PI 3.1415926535897932384626433832795

in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D uMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

void main() {

    vec3 fetch = normalize(fragPos);
    vec2 uv = vec2(atan(fetch.x, fetch.z), acos(fetch.y));

    uv.x = (PI + uv.x) * (0.5f / PI);
    uv.y = uv.y * invAtan.y;

    fragColor = vec4(texture(uMap, uv).rgb, 1.0);

}