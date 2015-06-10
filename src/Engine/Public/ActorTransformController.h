#pragma once
#include "Engine.h"

#include "Transform.h"

#include <boost/noncopyable.hpp>

class ActorTransformController : boost::noncopyable
{
public:
	virtual const Transform& getTransform() const = 0 ;
	virtual void setTransform() = 0;

	virtual ~ActorTransformController(){ };

};
