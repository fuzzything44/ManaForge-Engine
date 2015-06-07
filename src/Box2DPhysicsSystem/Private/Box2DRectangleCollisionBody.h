#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include <RectangleCollisionBody.h>
#include <RectangleCollisionComponent.h>

class Box2DPhysicsSystem;

class Box2DRectangleCollisionBody : public RectangleCollisionBody
{
public:
	Box2DRectangleCollisionBody(RectangleCollisionComponent* owner, Box2DPhysicsSystem* system);
	virtual ~Box2DRectangleCollisionBody();

	void update();

private:

	RectangleCollisionComponent* ownerComp;
	Box2DPhysicsSystem* physicsSystem;
	b2Body* body;
	b2Fixture* fixture;

};
