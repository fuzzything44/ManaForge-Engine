#pragma once
#include "Box2DPhysicsSystemConfig.h"
#include "Box2DPhysicsShape.h"

#include <PhysicsBody.h>
#include <PhysicsComponent.h>

class Box2DPhysicsSystem;

class Box2DPhysicsBody : public PhysicsBody
{
	
public:
	Box2DPhysicsBody(Box2DPhysicsShape* shape, PhysicsComponent* owner, Box2DPhysicsSystem* system);
	~Box2DPhysicsBody();

private:
	PhysicsComponent* ownerComponent;
	Box2DPhysicsSystem* system;
	
	b2Fixture* fixture;

};
