#version 330 core

unform sampler2D tex;

in vec2 UV;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(UV, tex);
}