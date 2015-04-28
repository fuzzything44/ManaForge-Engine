#pragma once
#include "Engine.h"
struct Color
{
	uint8 red;
	uint8 green;
	uint8 blue;
	uint8 alpha;

	explicit  Color(uint8 r = 0, uint8 g = 0, uint8 b = 0, uint8 a = 0) : red(r), green(g), blue(b), alpha(a) {}

	explicit Color(uint8* color) 
	{
		red = color[0];
		green = color[1];
		blue = color[2];
		alpha = color[3];
	}

	template<typename Archive>
	void serialize(Archive& ar, const uint32 version)
	{
		ar & BOOST_SERIALIZATION_NVP(red);
		ar & BOOST_SERIALIZATION_NVP(green);
		ar & BOOST_SERIALIZATION_NVP(blue);
		ar & BOOST_SERIALIZATION_NVP(alpha);
	}


	bool operator <(const Color& other) const
	{
		int16 suma = red + green + blue;
		int16 sumb = other.red + other.green + other.blue;

		return suma < sumb;
	}

	bool operator >(const Color& other) const
	{
		int16 suma = red + green + blue;
		int16 sumb = other.red + other.green + other.blue;

		return suma > sumb;
	}
};
