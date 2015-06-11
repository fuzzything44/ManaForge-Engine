#include "Box2DPhysicsBody.h"

#include "Box2DPhysicsSystem.h"
#include "Box2DActorTransformController.h"

#include <Helper.h>

Box2DPhysicsBody::Box2DPhysicsBody(Box2DPhysicsShape* shape, PhysicsComponent* owner, Box2DPhysicsSystem* system)
	:system(system),
	ownerComponent(owner)
{
	check(owner);
	check(system);

	auto iter = system->bodies.find(owner->getOwner());

	if (iter == system->bodies.end()) FATAL_ERR("could not find actor in ActorTransformController map");

	

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape->shape;
	fixtureDef.userData = owner;
	

	fixture = iter->second->body->CreateFixture(&fixtureDef);

}

Box2DPhysicsBody::~Box2DPhysicsBody()
{

}


void Box2DPhysicsBody::setRestitution(float newRestitution)
{
	fixture->SetRestitution(newRestitution);
}
float Box2DPhysicsBody::getRestitution() const
{
	return fixture->GetRestitution();
}

void Box2DPhysicsBody::setDensity(float newDensity)
{
	fixture->SetDensity(newDensity);
}
float Box2DPhysicsBody::getDensity() const
{
	return fixture->GetDensity();
}

void Box2DPhysicsBody::setFriction(float newFriction)
{
	fixture->SetFriction(newFriction);
}
float Box2DPhysicsBody::getFriction() const
{
	return fixture->GetFriction();
}

void Box2DPhysicsBody::setIsSensor(bool newIsSensor)
{
	fixture->SetSensor(newIsSensor);
}
bool Box2DPhysicsBody::getIsSensor() const
{
	return fixture->IsSensor();
}