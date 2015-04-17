#pragma once

#include "Engine.h"
#include "SceneComponent.h"


class CameraComponent : public SceneComponent
{
public:
	explicit ENGINE_API CameraComponent(Actor* owner = nullptr, Transform trans = Transform{}, mat4 viewMat = mat4{})
		: SceneComponent(owner, trans),
		viewMat(viewMat)
	{
	}


protected:
	
	mat4 viewMat;
};

