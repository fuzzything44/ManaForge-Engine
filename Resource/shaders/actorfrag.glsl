#version 330 core

uniform sampler2D tex;

in vec2 UV;

out vec4 fragColor;

void main()
{
	vec4 intfragColor = texture2D(tex, UV);

	if (intfragColor.a < .3f)
	{
		discard;
	}
	else
	{
		fragColor = intfragColor;
	}
}