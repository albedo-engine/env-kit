#include <GL/gl.h>
static const GLchar *shader_source_test_frag_glsl = {
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 WorldPos;\n"
"uniform samplerCube uMap;\n"
"//uniform sampler2D uTest;\n"
"void main()\n"
"{\n"
"    vec3 envColor = texture(uMap, WorldPos).rgb;\n"
"    // HDR tonemap and gamma correct\n"
"    envColor = envColor / (envColor + vec3(1.0));\n"
"    envColor = pow(envColor, vec3(1.0/2.2));\n"
"    FragColor = vec4(envColor, 1.0);\n"
"    //FragColor = vec4(texture(uTest, vec2(0.1, 0.1)).rgb, 1.0);\n"
"    //FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);\n"
"}\n"
};
