#pragma once
#include "Engine.h"

#include "Transform.h"

#include <boost/noncopyable.hpp>

class Actor;

enum class PhysicsType : uint8
{
	UNKNOWN		= 0,
	DYNAMIC		= 1,
	STATIC		= 2,
	KINEMATIC	= 3,
};

class ActorTransformController : boost::noncopyable
{
public:
	virtual Transform getTransform() const = 0 ;
	virtual vec2 getVelocity() const = 0;

	virtual void setTransform(const Transform& newTrans) = 0;
	virtual void setVelocity(vec2 newVelocity) = 0;

	virtual Actor* getOwner() const = 0;

	virtual PhysicsType getType() = 0;
	virtual void setType(PhysicsType) = 0;

	virtual void applyForce(vec2 force, vec2 point) = 0;

	virtual ~ActorTransformController(){ };

};
