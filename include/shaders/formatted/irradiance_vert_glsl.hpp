#include <GL/gl.h>
static const GLchar *shader_source_irradiance_vert_glsl = {
"#version 330 core\n"
"layout (location = 0) in vec3 vPos;\n"
"out vec3 fragPos;\n"
"uniform mat4 uProjection;\n"
"uniform mat4 uView;\n"
"void main() {\n"
"    fragPos = vPos;\n"
"    gl_Position = uProjection * uView * vec4(vPos, 1.0);\n"
"}\n"
};
