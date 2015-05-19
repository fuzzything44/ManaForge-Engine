#version 330 core

layout(location = 0) in vec2 vertLocationIn;
layout(location = 1) in vec2 vertTexCoordIn;

uniform mat4 MVPmat;
uniform float renderOrder;

out vec2 fragTexCoord;

void main()
{
	gl_Position = vec4(vertLocationIn, 0.f, 1.f) * MVPmat;

	fragTexCoord = vertTexCoordIn;
}