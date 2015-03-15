// define the version of GLSL to use
#version 330 core

// the texture object we can query from
uniform sampler2D tex;

// the UV that we interpolated and sent here in the vertex shader
in vec2 UV;

// the output
out vec4 color;


void main()
{
	// use the texture2D method to get the color at the corresponding texture location using the filtering method provided.
	// http://www.arcsynthesis.org/gltut/Texturing/Tut15%20Magnification.html	
	vec4 intColor = texture2D(tex, UV);	

	if (intColor.a == 0.f)
	{
		discard;
	}
	else
	{
		color = intColor;
	}
	
}