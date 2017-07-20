#version 330 core

in vec3 fragPos;
out vec4 fragColor;

uniform samplerCube uMap;
uniform float uNbSamples;

const float PI = 3.14159265359;
const float TWO_PI = 2.0 * PI;
const float PI_4 = 0.5 * PI;

void main() {

    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 normal = normalize(fragPos);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = (TWO_PI) / uNbSamples;
    float nbSamples = 0.0;
    for(float phi = 0.0; phi < TWO_PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < PI_4; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nbSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    fragColor = vec4(irradiance, 1.0);

}