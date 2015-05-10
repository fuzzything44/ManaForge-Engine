#version 330 core

uniform sampler2D textures[32];

in vec2 fragTexCoord;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(fragTexCoord, textures[0]);
}