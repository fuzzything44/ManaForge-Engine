#pragma once

#include <Engine.h>

#pragma comment(lib, "Box2D.lib")

#ifdef Box2DPhysicsSystem_Source
#	define Box2DPhysicsSystem_API __declspec(dllexport)
#else
#	define Box2DPhysicsSystem_API __declspec(dllimport)
#	pragma comment(lib, BOOST_PP_CAT("modules\\", BOOST_PP_CAT(Box2DPhysicsSystem_Name, ".lib")))
#endif

#include <Box2D/Box2D.h>

inline b2Vec2 convertVec(const vec2& other)
{
	return b2Vec2(other.x, other.y);
}

inline vec2 convertVec(const b2Vec2& other)
{
	return vec2(other.x, other.y);
}
