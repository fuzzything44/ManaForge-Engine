#pragma once

#include "Engine.h"

#include <boost/noncopyable.hpp>

class PhysicsBody;
class PhysicsShape;
class ActorTransformController;
class PhysicsComponent;
class Actor;

class PhysicsSystem : boost::noncopyable
{
public:

	virtual ~PhysicsSystem() = default;

	virtual void setGravity(vec2 newGravity) = 0;

	virtual std::unique_ptr<PhysicsBody> newPhysicsBody(PhysicsShape& shape, PhysicsComponent& owner) = 0;
	virtual std::unique_ptr<PhysicsShape> newPhysicsShape() = 0;
	virtual std::unique_ptr<ActorTransformController> newActorTransformController(Actor& actor) = 0;
	virtual void drawDebugPoints() = 0;
};
