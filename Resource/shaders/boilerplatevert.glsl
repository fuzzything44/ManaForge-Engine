#version 330 core

layout(location = 0) in vec2 vertLocationIn;
layout(location = 1) in vec2 vertTexCoordIn;

uniform mat4 cameraMat;
uniform float renderOrder;

out vec2 fragTexCoord;

void main()
{
	gl_Position = vec4(vertLocationIn, renderOrder, 1.0) * cameraMat;

	fragTexCoord = vertTexCoordIn;
}