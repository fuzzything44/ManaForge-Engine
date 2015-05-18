#version 330 core

uniform sampler2D textures[32];

in vec2 fragTexCoord;

out vec4 fragColor;

void main()
{
	//fragColor = texture2D(textures[0], fragTexCoord);
	fragColor = vec4(.2f, 1.f, 0.f, 1.f); // temp
}