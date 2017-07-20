#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;
	vec4 clipPos = uProjection * uView * vec4(WorldPos, 1.0);

	gl_Position = clipPos.xyww;
}
