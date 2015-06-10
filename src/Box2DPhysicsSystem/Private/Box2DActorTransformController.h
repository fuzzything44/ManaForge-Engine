#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include <ActorTransformController.h>

class Box2DActorTransformController : public ActorTransformController
{

	virtual const Transform& getTransform() const override;
	virtual void setTransform() override;
};
