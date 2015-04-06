#pragma once 

#include "Engine.h"
#include "ActorComponent.h"
#include "Transform.h"

class SceneComponent : ActorComponent
{
public:
	virtual ~SceneComponent()
	{
	}

public:
	Transform getTransform() const
	{
		return transform;
	}

	void setTransfrom(const Transform& transform1)
	{
		transform = transform1;
	}

private:

	Transform transform;


	explicit SceneComponent(Actor* owner);
};
