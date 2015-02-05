// define the version of GLSL to use
#version 330 core

// the texture object we can query from
uniform sampler2D tex0;

// the UV that we interpolated and sent here in the vertex shader
in vec2 UV;

// the output fragment color.
out vec4 fragColor;

void main()
{
	// use the texture2D method to get the color at the corresponding texture location using the filtering method provided.
	// http://www.arcsynthesis.org/gltut/Texturing/Tut15%20Magnification.html
	// also filps Y coordinate of UVs
	fragColor = texture2D(tex0, vec2(UV.s, 1 - UV.t));
}