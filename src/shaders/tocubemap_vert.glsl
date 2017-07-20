#version 330 core

layout (location = 0) in vec3 vPos;

out vec3 fragPos;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    fragPos = vPos;
    gl_Position = uProjection * uView * vec4(vPos, 1.0);
}
