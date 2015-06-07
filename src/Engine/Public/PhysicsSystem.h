#pragma once

#include "Engine.h"

#include <boost/noncopyable.hpp>

class RectangleCollisionComponent;
class RectangleCollisionBody;

class PhysicsSystem : boost::noncopyable
{
public:

	virtual ~PhysicsSystem(){ };

	virtual void setGravity(vec2 newGravity) = 0;

	virtual RectangleCollisionBody* newRectangeCollisonBody(RectangleCollisionComponent* owner) = 0;

};
