#pragma once

#include "Engine.h"
#include "PhysicsBody.h"
#include "PhysicsShape.h"
#include "ActorTransformController.h"

#include <boost/noncopyable.hpp>



class PhysicsComponent;

class PhysicsSystem : boost::noncopyable
{
public:

	virtual ~PhysicsSystem(){ };

	virtual void setGravity(vec2 newGravity) = 0;

	virtual PhysicsBody* newPhysicsBody(PhysicsShape* shape, PhysicsComponent* owner) = 0;
	virtual PhysicsShape* newPhysicsShape() = 0;
	virtual ActorTransformController* newActorTransformController(Actor* actor) = 0;

};
