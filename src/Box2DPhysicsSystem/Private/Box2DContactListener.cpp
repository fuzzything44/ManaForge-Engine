#include "Box2DContactListener.h"
#include "Box2DPhysicsBody.h"

#include <PhysicsComponent.h>

void Box2DContactListener::BeginContact(b2Contact * contact)
{

	Box2DPhysicsBody* phyBodyA = static_cast<Box2DPhysicsBody*>(contact->GetFixtureA()->GetUserData());
	Box2DPhysicsBody* phyBodyB = static_cast<Box2DPhysicsBody*>(contact->GetFixtureB()->GetUserData());

	phyBodyA->callStartContactCallback(phyBodyB->getOwner());
	phyBodyB->callStartContactCallback(phyBodyA->getOwner());
	
}

void Box2DContactListener::EndContact(b2Contact * contact)
{

	Box2DPhysicsBody* phyBodyA = static_cast<Box2DPhysicsBody*>(contact->GetFixtureA()->GetUserData());
	Box2DPhysicsBody* phyBodyB = static_cast<Box2DPhysicsBody*>(contact->GetFixtureB()->GetUserData());

	phyBodyA->callEndContactCallback(phyBodyB->getOwner());
	phyBodyB->callEndContactCallback(phyBodyA->getOwner());
}
