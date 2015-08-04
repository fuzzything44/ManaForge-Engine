#version 330 core
layout(location = 0) in vec2 vertLocationIn;
layout(location = 1) in vec2 vertTexCoordIn;

uniform mat3 MVPmat;
uniform float renderOrder;

out vec2 fragTexCoord;

void main()
{
	vec3 vec3location = MVPmat * vec3(vertLocationIn, 1.f);
	
	gl_Position = vec4(vec3location.x, vec3location.y, -renderOrder, vec3location.z);
	
	fragTexCoord = vertTexCoordIn;
}