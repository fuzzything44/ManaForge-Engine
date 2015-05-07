#pragma once

#include "Engine.h"

#include <boost/serialization/nvp.hpp>


struct Transform
{
	explicit Transform(vec2 location = vec2(0.f), float rotation = 0.f, vec2 scale = vec2(0.f))
		: location(location),
		rotation(rotation),
		scale(scale) { }

	vec2 location;
	float rotation;
	vec2 scale;

	template <typename Archive>
	void serialize(Archive& ar, const uint32 version)
	{
		ar & BOOST_SERIALIZATION_NVP(location);
		ar & BOOST_SERIALIZATION_NVP(rotation);
		ar & BOOST_SERIALIZATION_NVP(scale);
	}

	inline Transform& operator+=(Transform& other)
	{
		location += other.location;
		rotation += other.rotation;
		scale += other.scale;

		return *this;
	}


	inline Transform operator+(Transform& other)
	{
		return Transform(location + other.location, rotation + other.rotation, scale + other.scale);
	}

	inline Transform operator-(Transform& other)
	{
		return Transform(location - other.location, rotation - other.rotation, scale - other.scale);
	}

	inline Transform& operator-=(Transform& other)
	{
		location -= other.location;
		rotation -= other.rotation;
		scale -= other.scale;

		return *this;
	}

	inline Transform& operator*=(Transform& other)
	{
		location *= other.location;
		rotation *= other.rotation;
		scale *= other.scale;

		return *this;
	}


	inline Transform operator*(Transform& other)
	{
		return Transform(location * other.location, rotation * other.rotation, scale * other.scale);
	}

};


