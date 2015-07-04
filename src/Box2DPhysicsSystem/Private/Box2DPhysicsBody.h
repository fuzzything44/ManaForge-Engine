#pragma once
#include "Box2DPhysicsSystemConfig.h"
#include "Box2DPhysicsShape.h"

#include <PhysicsBody.h>
#include <PhysicsComponent.h>

class Box2DActorTransformController;
class Box2DPhysicsSystem;

class Box2DPhysicsBody : public PhysicsBody
{
	
public:
	Box2DPhysicsBody(Box2DPhysicsShape& shape, PhysicsComponent& owner, Box2DPhysicsSystem& system);
	virtual ~Box2DPhysicsBody();

	virtual void setRestitution(float newRestitution) override;
	virtual float getRestitution() const override;

	virtual void setDensity(float newDensity) override;
	virtual float getDensity() const override;

	virtual void setFriction(float newFriction) override;
	virtual float getFriction() const override;

	virtual void setIsSensor(bool newIsSensor) override;
	virtual bool getIsSensor() const override;

private:
	PhysicsComponent& ownerComponent;
	Box2DPhysicsSystem& system; 
	Box2DActorTransformController* ownerController; // needs to be ptr because we find it afterwards -- and it might not exist
	
	b2Fixture* fixture;

};
