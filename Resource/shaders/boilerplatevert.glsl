#version 330 core
layout(location = 0) in vec2 vertLocationIn;
layout(location = 1) in vec2 vertTexCoordIn;

uniform mat3 MVPmat;
uniform float renderOrder;

out vec2 fragTexCoord;

void main()
{
	
	gl_Position.xyw = MVPmat * vec3(vertLocationIn, 1.f);
	gl_Position.z = float(renderOrder - 256) / 256;
	
	fragTexCoord = vertTexCoordIn;
}