#version 330 core

layout(location = 0) in vec2 location;
layout(location = 1) in vec2 texCoords;

uniform mat3 viewMat;

out vec2 fragUV;

void main()
{

	gl_Position.xyw = viewMat * vec3(location, 1.f);
	gl_Position.z = 0.f;

	fragUV = texCoords;
}

