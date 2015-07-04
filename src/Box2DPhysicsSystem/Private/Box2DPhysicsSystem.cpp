#include "Box2DPhysicsSystem.h"

#include "Box2DPhysicsShape.h"
#include "Box2DPhysicsBody.h"
#include "Box2DActorTransformController.h"
#include "Box2DDebugDraw.h"

Box2DPhysicsSystem::Box2DPhysicsSystem()
{
	world = new b2World(gravity);

	DebugDraw* dd = new DebugDraw();
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_aabbBit;

	dd->SetFlags(flags);
	world->SetDebugDraw(dd);

	
}

Box2DPhysicsSystem::~Box2DPhysicsSystem()
{

}

void Box2DPhysicsSystem::setGravity(vec2 newGravity)
{
	gravity = b2Vec2(newGravity.x, newGravity.y);
	world->SetGravity(gravity);

}


PhysicsBody* Box2DPhysicsSystem::newPhysicsBody(PhysicsShape& shape, PhysicsComponent& owner)
{
	return new Box2DPhysicsBody(static_cast<Box2DPhysicsShape&>(shape), owner, *this);
}

std::unique_ptr<PhysicsShape> Box2DPhysicsSystem::newPhysicsShape()
{
	return std::make_unique<Box2DPhysicsShape>();
}

ActorTransformController* Box2DPhysicsSystem::newActorTransformController(Actor& actor)
{
	return new Box2DActorTransformController(actor, *this);
}


bool Box2DPhysicsSystem::update(float deltaTime)
{
	world->Step(deltaTime, 8, 3); // step once

	return true;
}


void Box2DPhysicsSystem::drawDebugPoints()
{
	world->DrawDebugData();
}