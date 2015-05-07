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

	Color&& operator+(Color& other)
	{
		Color ret;

		ret.red = red + other.red;
		ret.green = green + other.green;
		ret.blue = blue + other.blue;
		ret.alpha = alpha + other.alpha;

		return std::move(ret);
	}

	// rvalue refrence for add
	Color&& operator+(Color&& other)
	{
		other.red += red;
		other.green += green;
		other.blue += blue;
		other.alpha += alpha;

		return std::move(other);
	}

	bool operator <(const Color& other) const
	{
		uint16 suma = red + green + blue + alpha;
		uint16 sumb = other.red + other.green + other.blue + other.alpha;

		return suma < sumb;
	}

	bool operator >(const Color& other) const
	{
		uint16 suma = red + green + blue + alpha;
		uint16 sumb = other.red + other.green + other.blue + other.alpha;

		return suma > sumb;
	}
};
