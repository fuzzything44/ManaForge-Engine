#pragma once 

#include "Engine.h"
#include "SceneComponent.h"

class Camera : public SceneComponent
{
public:
	explicit Camera(Actor* owner, vec2 relativeLocation);
	virtual void setRelativeLocation(vec2 newLocation) = 0;
	virtual vec2 getRelativeLocation() = 0;

};
