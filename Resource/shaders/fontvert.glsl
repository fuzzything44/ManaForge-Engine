#version 330 core

layout(location = 0) in vec2 location;
layout(location = 1) in vec2 texCoords;

uniform float size = 1.0;

out vec2 fragUV;

void main()
{

	gl_Position.xy = location * size;
	gl_Position.z = 0.f;
	gl_Position.w = 1.f;

	fragUV = texCoords;
}

