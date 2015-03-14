#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_UV;

uniform mat4 viewMat;

out vec2 UV;

void main()
{
	vec4 pos = vec4(in_pos.x, in_pos.y, in_pos.z, 1.f) * viewMat;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.f);

	// pass UV data to be interpolated to the fragment shader
	UV = in_UV;
}