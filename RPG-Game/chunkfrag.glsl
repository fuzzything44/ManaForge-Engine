// define the version of GLSL to use
#version 330 core

// the texture object we can query from
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform sampler2D tex8;
uniform sampler2D tex9;
uniform sampler2D tex10;
uniform sampler2D tex11;
uniform sampler2D tex12;
uniform sampler2D tex13;
uniform sampler2D tex14;
uniform sampler2D tex15;
uniform sampler2D tex16;

// the UV that we interpolated and sent here in the vertex shader
in vec2 UV;

// the output
out vec4 color;


void main()
{
	// use the texture2D method to get the color at the corresponding texture location using the filtering method provided.
	// http://www.arcsynthesis.org/gltut/Texturing/Tut15%20Magnification.html
	// also filps Y coordinate of UVs

	int texID = int(floor(UV.x));

	switch (texID)
	{
	case 0:
		color = texture2D(tex0, UV);
	case 1:
		color = texture2D(tex1, vec2(UV.x - texID, UV.y - texID));
	case 2:
		color = texture2D(tex2, vec2(UV.x - texID, UV.y - texID));
	case 3:
		color = texture2D(tex3, vec2(UV.x - texID, UV.y - texID));
	case 4:
		color = texture2D(tex4, vec2(UV.x - texID, UV.y - texID));
	case 5:
		color = texture2D(tex5, vec2(UV.x - texID, UV.y - texID));
	case 6:
		color = texture2D(tex6, vec2(UV.x - texID, UV.y - texID));
	case 7:
		color = texture2D(tex7, vec2(UV.x - texID, UV.y - texID));
	case 8:
		color = texture2D(tex8, vec2(UV.x - texID, UV.y - texID));
	case 9:
		color = texture2D(tex9, vec2(UV.x - texID, UV.y - texID));
	case 10:
		color = texture2D(tex10, vec2(UV.x - texID, UV.y - texID));
	case 11:
		color = texture2D(tex11, vec2(UV.x - texID, UV.y - texID));
	case 12:
		color = texture2D(tex12, vec2(UV.x - texID, UV.y - texID));
	case 13:
		color = texture2D(tex13, vec2(UV.x - texID, UV.y - texID));
	case 14:
		color = texture2D(tex14, vec2(UV.x - texID, UV.y - texID));
	case 15:
		color = texture2D(tex15, vec2(UV.x - texID, UV.y - texID));
	case 16:
		color = texture2D(tex16, vec2(UV.x - texID, UV.y - texID));

	default:
		break;
	}

}