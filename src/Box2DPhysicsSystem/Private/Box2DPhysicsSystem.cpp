#include "Box2DPhysicsSystem.h"

#include "Box2DPhysicsShape.h"
#include "Box2DPhysicsBody.h"

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


PhysicsBody* Box2DPhysicsSystem::newPhysicsBody(PhysicsShape* shape, PhysicsComponent* owner)
{
	return new Box2DPhysicsBody(static_cast<Box2DPhysicsShape*>(shape), owner, this);
}

PhysicsShape* Box2DPhysicsSystem::newPhysicsShape()
{
	return new Box2DPhysicsShape();
}


bool Box2DPhysicsSystem::update(float deltaTime)
{
	world->Step(deltaTime, 1, 1); // step once


	return true;
}


