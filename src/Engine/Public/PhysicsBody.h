#pragma once
#include "Engine.h"

#include <boost/noncopyable.hpp>

class PhysicsBody : boost::noncopyable
{
public:
	virtual ~PhysicsBody(){ }

	virtual void setRestitution(float newRestitution) = 0;
	virtual float getRestitution() const = 0;

	virtual void setDensity(float newDensity) = 0;
	virtual float getDensity() const = 0;

	virtual void setFriction(float newFriction) = 0;
	virtual float getFriction() const = 0;

	virtual void setIsSensor(bool newIsSensor) = 0;
	virtual bool getIsSensor() const = 0;
};
