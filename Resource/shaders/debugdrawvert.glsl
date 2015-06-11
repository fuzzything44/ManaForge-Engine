#version 330 core

layout(location = 0) in vec2 vert_pos;

uniform mat3 MVPmat;

void main()
{
	gl_Position.xyz = MVPmat * vec3(vert_pos, 1.f);
	gl_Position.z = 0.f;
	gl_Position.w = 1.f;
}
