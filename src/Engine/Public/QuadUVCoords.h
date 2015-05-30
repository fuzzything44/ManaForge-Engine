#pragma once

#include "Engine.h"

struct QuadUVCoords
{

	vec2* toPtr()
	{
		return reinterpret_cast<vec2*>(this);
	}

	vec2 lowerLeft;
	vec2 upperLeft;

	vec2 lowerRight;
	vec2 upperRight;
};