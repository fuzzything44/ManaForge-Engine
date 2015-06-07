#include "Box2DPhysicsSystem.h"

#include "Box2DRectangleCollisionBody.h"

Box2DPhysicsSystem::Box2DPhysicsSystem()
{
	world = new b2World(gravity);
}

Box2DPhysicsSystem::~Box2DPhysicsSystem()
{

}

void Box2DPhysicsSystem::setGravity(vec2 newGravity)
{
	gravity = b2Vec2(newGravity.x, newGravity.y);
	world->SetGravity(gravity);

}

RectangleCollisionBody* Box2DPhysicsSystem::newRectangeCollisonBody(RectangleCollisionComponent* owner)
{
	return new Box2DRectangleCollisionBody(owner, this);
}

bool Box2DPhysicsSystem::update(float deltaTime)
{
	world->Step(deltaTime, 1, 1); // step once

	for (auto& elem : bodies)
	{
		elem->update();
	}

	return true;
}


void Box2DPhysicsSystem::addBody(Box2DRectangleCollisionBody* newBody)
{
	bodies.push_back(newBody);
}

b2World* Box2DPhysicsSystem::getWorld()
{
	return world;
}