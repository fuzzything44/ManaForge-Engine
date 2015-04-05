#include "Engine.h"


struct Transform
{
	explicit Transform(vec2 location = vec2(0.f), float rotation = 0.f, vec2 scale = vec2(0.f))
		: location(location),
		rotation(rotation),
		scale(scale) { }

	vec2 location;
	float rotation;
	vec2 scale;

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


