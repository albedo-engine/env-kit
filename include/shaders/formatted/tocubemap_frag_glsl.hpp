#include <GL/gl.h>
static const GLchar *shader_source_tocubemap_frag_glsl = {
"#version 330 core\n"
"in vec3 fragPos;\n"
"out vec4 fragColor;\n"
"uniform sampler2D uMap;\n"
"const vec2 invAtan = vec2(0.1591, 0.3183);\n"
"void main() {\n"
"    vec3 fetch = normalize(fragPos);\n"
"    vec2 uv = vec2(atan(fetch.z, fetch.x), asin(fetch.y));\n"
"    uv *= invAtan;\n"
"    uv += 0.5;\n"
"    fragColor = vec4(texture(uMap, uv).rgb, 1.0);\n"
"}\n"
};
