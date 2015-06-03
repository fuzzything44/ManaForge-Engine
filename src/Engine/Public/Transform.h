#pragma once

#include "Engine.h"

#include <SaveData.h>


struct Transform
{
	explicit Transform(const vec2& location = vec2(0.f), float rotation = 0.f, const vec2& scale = vec2(1.f, 1.f))
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

	inline Transform& operator+=(const Transform& other)
	{
		location += other.location;
		rotation += other.rotation;
		scale += other.scale;

		return *this;
	}


	inline Transform operator+(const Transform& other) const 
	{
		return Transform(location + other.location, rotation + other.rotation, scale + other.scale);
	}

	inline Transform operator-(const Transform& other) const
	{
		return Transform(location - other.location, rotation - other.rotation, scale - other.scale);
	}

	inline Transform& operator-=(const Transform& other) 
	{
		location -= other.location;
		rotation -= other.rotation;
		scale -= other.scale;

		return *this;
	}

	inline Transform& operator*=(const Transform& other)
	{
		location *= other.location;
		rotation *= other.rotation;
		scale *= other.scale;

		return *this;
	}


	inline Transform operator*(const Transform& other) const
	{
		return Transform(location * other.location, rotation * other.rotation, scale * other.scale);
	}

};


