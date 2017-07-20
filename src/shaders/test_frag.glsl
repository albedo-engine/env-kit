#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube uMap;
//uniform sampler2D uTest;

void main()
{
    vec3 envColor = texture(uMap, WorldPos).rgb;

    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    FragColor = vec4(envColor, 1.0);
    //FragColor = vec4(texture(uTest, vec2(0.1, 0.1)).rgb, 1.0);
    //FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}
