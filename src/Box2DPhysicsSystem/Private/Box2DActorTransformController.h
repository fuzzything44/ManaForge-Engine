#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include <ActorTransformController.h>

class Actor;
class Box2DPhysicsSystem;

class Box2DActorTransformController : public ActorTransformController
{
	friend class Box2DPhysicsBody;
public:

	explicit Box2DActorTransformController(Actor& owner, Box2DPhysicsSystem& system);
	virtual ~Box2DActorTransformController();

	virtual Transform getTransform() const override;
	virtual vec2 getVelocity() const override;

	virtual void setTransform(const Transform& newTrans) override;
	virtual void setVelocity(vec2 newVelocity) override;

	virtual PhysicsType getType() override;
	virtual void setType(PhysicsType) override;

	virtual void applyLocalForce(vec2 localForce, vec2 localPoint) override;
	virtual void applyWorldForce(vec2 localForce, vec2 localPoint) override;

	virtual void applyTorque(float magnituede) override;

	virtual void setAngularVelocity(float newVelocity) override;
	virtual float getAngularVelocity() override;

	virtual Actor& getOwner() const override;



	Box2DPhysicsSystem& system;

	static void bodyDeleter(b2Body* ptr)
	{
		ptr->GetWorld()->DestroyBody(ptr);
	}

	std::unique_ptr<b2Body, decltype(&bodyDeleter)> body;
	Actor& owner;


};
