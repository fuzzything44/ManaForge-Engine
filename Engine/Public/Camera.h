#pragma once

#include "Engine.h"
#include "Component.h"

#pragma message("Camera Defined")
class ENGINE_API Camera : public Component
{
	explicit Camera(Actor* owner)
		: Component(owner)
	{
	}

public:
	
};
