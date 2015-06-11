#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include <ActorTransformController.h>

class Actor;
class Box2DPhysicsSystem;

class Box2DActorTransformController : public ActorTransformController
{
	friend class Box2DPhysicsBody;
public:

	explicit Box2DActorTransformController(Actor* owner, Box2DPhysicsSystem* system);

	virtual Transform getTransform() const override;
	virtual vec2 getVelocity() const override;

	virtual void setTransform(const Transform& newTrans) override;
	virtual void setVelocity(vec2 newVelocity) override;

	virtual PhysicsType getType() override;
	virtual void setType(PhysicsType) override;

	virtual void applyForce(vec2 force, vec2 point) override;

	virtual Actor* getOwner() const override;



private:

	Box2DPhysicsSystem* system;

	b2Body* body;
	Actor* owner;

};
