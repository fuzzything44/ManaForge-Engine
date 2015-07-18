#include "Box2DPhysicsSystem.h"

#include "Box2DPhysicsShape.h"
#include "Box2DPhysicsBody.h"
#include "Box2DActorTransformController.h"
#include "Box2DDebugDraw.h"

Box2DPhysicsSystem::Box2DPhysicsSystem()
{
	world = std::make_unique<b2World>(gravity);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_aabbBit;

}

Box2DPhysicsSystem::~Box2DPhysicsSystem()
{
}

void Box2DPhysicsSystem::setGravity(vec2 newGravity)
{
	gravity = b2Vec2(newGravity.x, newGravity.y);
	world->SetGravity(gravity);

}


std::unique_ptr<PhysicsBody> Box2DPhysicsSystem::newPhysicsBody(PhysicsShape& shape, PhysicsComponent& owner)
{
	return std::make_unique<Box2DPhysicsBody>(static_cast<Box2DPhysicsShape&>(shape), owner, *this);
}

std::unique_ptr<PhysicsShape> Box2DPhysicsSystem::newPhysicsShape()
{
	return std::make_unique<Box2DPhysicsShape>();
}

std::unique_ptr<ActorTransformController> Box2DPhysicsSystem::newActorTransformController(Actor& actor)
{
	return std::make_unique<Box2DActorTransformController>(actor, *this);
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