#version 330 core

layout(location = 0) in vec2 vertLocationIn;
layout(location = 1) in vec2 vertTexCoordIn;

uniform mat4 camera;
uniform mat3 model;
uniform float renderOrder;

out vec2 fragTexCoord;

void main()
{
	vec3 locationBeforeCamera = model * vec3(vertLocationIn, 1.f);
	
	gl_Position = camera * vec4(locationBeforeCamera.x, locationBeforeCamera.y, -1.f, 1.f);
	
	fragTexCoord = vertTexCoordIn;
}