#include <GL/gl.h>
static const GLchar *shader_source_test_glsl = {
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 uProjection;\n"
"uniform mat4 uView;\n"
"out vec3 WorldPos;\n"
"void main()\n"
"{\n"
"    WorldPos = aPos;\n"
"	vec4 clipPos = uProjection * uView * vec4(WorldPos, 1.0);\n"
"	gl_Position = clipPos.xyww;\n"
"}\n"
};
