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

	// Inherited via PhysicsBody
	virtual void setRestitution(float newRestitution) override;
	virtual float getRestitution() const override;

	virtual void setDensity(float newDensity) override;
	virtual float getDensity() const override;

	virtual void setFriction(float newFriction) override;
	virtual float getFriction() const override;

	virtual void setIsSensor(bool newIsSensor) override;
	virtual bool getIsSensor() const override;

	virtual void setStartContactCallback(const std::function<void(PhysicsComponent&)>&) override;

	virtual void setEndContactCallback(const std::function<void(PhysicsComponent&)>&) override;

	inline void callStartContactCallback(PhysicsComponent& other)
	{
		startCallOther = &other;
		shouldCallStartContact = true;
	}
	inline void callEndContactCallback(PhysicsComponent& other)
	{
		endCallOther = &other;
		shouldCallEndContact = true;
	}
	inline PhysicsComponent& getOwner() const { return ownerComponent; }
	inline void postStep()
	{

		if (shouldCallStartContact && startContactCallback && startCallOther)
			startContactCallback(*startCallOther);
		if (shouldCallEndContact && endContactCallback && endCallOther) endContactCallback(*endCallOther);
		shouldCallStartContact = false;
		shouldCallEndContact = false;

		startCallOther = nullptr;
		endCallOther = nullptr;
	}

  private:
	PhysicsComponent& ownerComponent;
	Box2DPhysicsSystem& system;
	Box2DActorTransformController*
		ownerController; // needs to be ptr because we find it afterwards -- and it might not
						 // exist

	b2Fixture* fixture;

	bool shouldCallStartContact, shouldCallEndContact;
	PhysicsComponent* endCallOther, *startCallOther;
	std::function<void(PhysicsComponent&)> startContactCallback;
	std::function<void(PhysicsComponent&)> endContactCallback;
};
