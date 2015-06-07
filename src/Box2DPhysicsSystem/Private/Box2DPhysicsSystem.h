#pragma once
#include "Box2DPhysicsSystemConfig.h"
#include "Box2DRectangleCollisionBody.h"

#include <PhysicsSystem.h>
#include <RectangleCollisionBody.h>

#include <list>

class Box2DPhysicsSystem : public PhysicsSystem
{
public:

	Box2DPhysicsSystem();
	virtual ~Box2DPhysicsSystem();

	virtual void setGravity(vec2 newGravity) override;

	virtual RectangleCollisionBody* newRectangeCollisonBody(RectangleCollisionComponent* owner) override;
	
	bool update(float deltaTime);

	b2World* getWorld();
	void addBody(Box2DRectangleCollisionBody* newBody);

private:
	
	std::list<Box2DRectangleCollisionBody*> bodies;

	b2World* world;
	b2Vec2 gravity;
};
