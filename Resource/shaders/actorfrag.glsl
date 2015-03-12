#version 330 core

uniform sampler2D tex;

in vec2 UV;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(tex, UV);
}