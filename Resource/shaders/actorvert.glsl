#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_UV;

uniform mat4 viewMat;

out vec2 UV;

void main()
{

	gl_Position = vec4(in_pos, 1.f) * viewMat;

	// pass UV data to be interpolated to the fragment shader
	UV = in_UV;
}