#include "Box2DPhysicsBody.h"

#include "Box2DPhysicsSystem.h"
#include "Box2DActorTransformController.h"

#include <Helper.h>

Box2DPhysicsBody::Box2DPhysicsBody(Box2DPhysicsShape& shape, PhysicsComponent& owner, Box2DPhysicsSystem& system)
	:system(system),
	ownerComponent(owner),
	shouldCallStartContact(false),
	shouldCallEndContact(false)
{
	auto iter = system.bodies.find(&(owner.getOwner()));

	if (iter == system.bodies.end()) logger<Fatal>() << "could not find actor in ActorTransformController map";
	ownerController = iter->second;
	

	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape.shape.get();
	fixtureDef.userData = this;
	
	
	check(ownerController->body);
	check(ownerController->body->GetWorld())
	fixture = ownerController->body->CreateFixture(&fixtureDef);

}

Box2DPhysicsBody::~Box2DPhysicsBody()
{
	ownerController->body->DestroyFixture(fixture);
}


void Box2DPhysicsBody::setRestitution(float newRestitution)
{
	fixture->SetRestitution(newRestitution);
	ownerController->body->ResetMassData();
}
float Box2DPhysicsBody::getRestitution() const
{
	return fixture->GetRestitution();
}

void Box2DPhysicsBody::setDensity(float newDensity)
{
	fixture->SetDensity(newDensity);
	ownerController->body->ResetMassData();
	
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

void Box2DPhysicsBody::setStartContactCallback(const std::function<void(PhysicsComponent&)>& callback)
{
	startContactCallback = callback;
}

void Box2DPhysicsBody::setEndContactCallback(const std::function<void(PhysicsComponent&)>& callback)
{
	endContactCallback = callback;
}
