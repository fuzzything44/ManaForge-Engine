#pragma once
#include "Engine.h"
struct Color
{
	uint8 red;
	uint8 green;
	uint8 blue;
	uint8 alpha;

	Color(uint8 r = 0, uint8 g = 0, uint8 b = 0, uint8 a = 0) : red(r), green(g), blue(b), alpha(a) {}
	Color(uint8* color = 0) {
		red = color[0];
		green = color[1];
		blue = color[2];
		alpha = color[3];
	}
};