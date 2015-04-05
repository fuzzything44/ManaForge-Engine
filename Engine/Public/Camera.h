#pragma once 

#include "Engine.h"
#include "ActorComponent.h"

class Camera : public ActorComponent
{
public:
	Camera(Actor* owner, vec2 relativeLocation);
	virtual void setRelativeLocation(vec2 newLocation) = 0;
	virtual vec2 getRelativeLocation() = 0;

};
