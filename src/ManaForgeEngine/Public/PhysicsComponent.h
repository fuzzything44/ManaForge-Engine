#pragma once
#include "Engine.h"

#include "SceneComponent.h"

class PhysicsBody;
class PhysicsShape;

class PhysicsComponent : public SceneComponent
{
public:

	inline PhysicsComponent(Actor& owner, PhysicsShape& shape, const Transform& trans = Transform());
	ENGINE_API virtual ~PhysicsComponent();

	inline void setRestitution(float newRestitution);
	inline float getRestitution() const;

	inline void setDensity(float newDensity);
	inline float getDensity() const;

	inline void setFriction(float newFriction);
	inline float getFriction() const;

	inline void setIsSensor(bool newIsSensor);
	inline bool getIsSensor() const;


private:
	std::unique_ptr<PhysicsBody> body;

};


#include "Runtime.h"
#include "PhysicsBody.h"
#include "PhysicsShape.h"
#include "PhysicsSystem.h"

/////////// INLINE DEFINITIONS /////////

inline PhysicsComponent::PhysicsComponent(Actor& owner, PhysicsShape& shape, const Transform& trans)
	:SceneComponent(owner, trans)
{
	body = Runtime::get().moduleManager.getPhysicsSystem().newPhysicsBody(shape, *this);
}



inline void PhysicsComponent::setRestitution(float newRestitution)
{
	body->setRestitution(newRestitution);
}
inline float PhysicsComponent::getRestitution() const
{
	return body->getRestitution();
}

inline void PhysicsComponent::setDensity(float newDensity)
{
	body->setDensity(newDensity);
}
inline float PhysicsComponent::getDensity() const
{
	return body->getDensity();
}

inline void PhysicsComponent::setFriction(float newFriction)
{
	body->setFriction(newFriction);
}
inline float PhysicsComponent::getFriction() const
{
	return body->getFriction();
}

inline void PhysicsComponent::setIsSensor(bool newIsSensor)
{
	body->setIsSensor(newIsSensor);
}
inline bool PhysicsComponent::getIsSensor() const
{
	return body->getIsSensor();
}
