#pragma once
#include "Box2DPhysicsSystemConfig.h"

namespace detail
{
	inline void bodyDeleter(b2Body* body)
	{
		body->GetWorld()->DestroyBody(body);
	}
}



struct CBox2DCollision
{

	CBox2DCollision(b2World* world, b2BodyDef bodyDef)
		: body{ world->CreateBody(&bodyDef), &detail::bodyDeleter }
	{
	}

	std::unique_ptr<b2Body, void(*)(b2Body*)> body;

};
