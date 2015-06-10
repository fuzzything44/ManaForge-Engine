#pragma once
#include "Engine.h"

#include "SceneComponent.h"
#include "PhysicsShape.h"
#include "Runtime.h"


class PhysicsComponent : public SceneComponent
{
public:

	inline PhysicsComponent(Actor* owner = nullptr, const Transform& trans = Transform(), PhysicsShape* shape = nullptr);

private:
	PhysicsBody* body;

};



inline PhysicsComponent::PhysicsComponent(Actor* owner, const Transform& trans, PhysicsShape* shape)
	:SceneComponent(owner, trans)
{
	body = Runtime::get().moduleManager.getPhysicsSystem().newPhysicsBody(shape, this);
}
