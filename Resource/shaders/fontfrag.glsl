#version 330 core

in vec2 fragUV;


uniform sampler2D tex;
uniform vec3 forgroundColor = vec3(1.f, 0.f, 0.f);
uniform float cutoff;

out vec4 fragColor;

void main()
{
	float col = texture(tex, vec2(fragUV.x, fragUV.y)).r;

	if (col < (cutoff + .05f) && col > (cutoff - .05f))
	{
		fragColor = vec4(forgroundColor, .5f);
	}
	else if(col < cutoff)
	{
		discard;
	}
	else
	{
		fragColor = vec4(forgroundColor, 1.f);
	}
}

