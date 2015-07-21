#pragma once
#include "Box2DPhysicsSystemConfig.h"

class Box2DContactListener : public b2ContactListener
{
	virtual void BeginContact(b2Contact* contact) override;

	virtual void EndContact(b2Contact* contact) override;

};