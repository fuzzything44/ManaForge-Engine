#pragma once

#include "Engine.h"


/// <summary> An uv data.</summary>
struct UVData
{
	/// <summary> Constructor.</summary>
	///
	/// <param name="bottomLeftIn">  The bottom left UVs.</param>
	/// <param name="topLeftIn">	 The top left UVs.</param>
	/// <param name="bottomRightIn"> The bottom right UVs.</param>
	/// <param name="topRightIn">    The top right UVs.</param>
	explicit UVData(vec2 bottomLeftIn = vec2(0.f, 0.f), vec2 topLeftIn = vec2(0.f, 0.f),
		vec2 bottomRightIn = vec2(0.f, 0.f), vec2 topRightIn = vec2(0.f, 0.f))
		: bottomLeft(bottomLeftIn),
		topLeft(topLeftIn),
		bottomRight(bottomRightIn),
		topRight(topRightIn) {
		STACK
	}


	vec2 bottomLeft;
	vec2 topLeft;
	vec2 bottomRight;
	vec2 topRight;
};

