#include "Box2DPhysicsBody.h"

#include "Box2DPhysicsSystem.h"

#include <Helper.h>

Box2DPhysicsBody::Box2DPhysicsBody(Box2DPhysicsShape* shape, PhysicsComponent* owner, Box2DPhysicsSystem* system)
	:system(system),
	ownerComponent(owner)
{
	check(owner);
	check(system);

	auto iter = system->bodies.find(owner->getOwner());

	b2Body* body;

	// if it hasn't been created, create it.
	if (iter == system->bodies.end())
	{

		b2BodyDef bodyDef;
		bodyDef.position = convertVec(owner->getOwner()->getWorldLocation());
		bodyDef.angle = owner->getOwner()->getWorldRotation();
		bodyDef.userData = owner->getOwner();
		bodyDef.type = b2BodyType::b2_dynamicBody; // TODO: set this

		body = system->world->CreateBody(&bodyDef);
		system->addActor(owner->getOwner(), body);
	}
	else
	{
		body = iter->second;
	}

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape->shape;
	fixtureDef.userData = owner;
	// TODO: params

	fixture = body->CreateFixture(&fixtureDef);

}

Box2DPhysicsBody::~Box2DPhysicsBody()
{

}
