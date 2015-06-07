#pragma once
#include "Engine.h"

#include "SceneComponent.h"
#include "RectangleCollisionBody.h"
#include "Runtime.h"

class RectangleCollisionComponent : public SceneComponent
{
public:
	RectangleCollisionComponent(Actor* owner = nullptr, const Transform& trans = Transform())
		:SceneComponent(owner, trans)
	{
		body = Runtime::get().moduleManager.getPhysicsSystem().newRectangeCollisonBody(this);

	}
	virtual ~RectangleCollisionComponent(){ };

private:
	RectangleCollisionBody* body;

};



