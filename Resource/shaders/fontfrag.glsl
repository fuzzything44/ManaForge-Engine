#version 330 core

in vec2 fragUV;


uniform sampler2D tex;
uniform vec4 forgroundColor = vec4(1.f, 0.f, 0.f, 1.f);
uniform float cutoff;

out vec4 fragColor;

void main()
{
	float col = texture(tex, vec2(fragUV.x, fragUV.y)).r;

	if(col < cutoff)
	{
		discard;
	}
	else
	{
		fragColor = forgroundColor;
	}
}

